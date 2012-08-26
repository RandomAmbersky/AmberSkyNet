
#include "CNetSupervisor.h"
#include "ILog.h"
#include "ASN_utils.h"

#include <iostream>
#include <fstream>

using namespace std;

//#include "CNodeFactory.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

//char message[1024];

CNetSupervisor::CNetSupervisor(IEngine *_Engine):CBaseObject(_Engine){

SetParam("BaseClass","NetSupervisor");
SetParam("Type","NetSupervisorSDL");

num_open_socket=0;
message = NULL;

std::string BufSize = ENGINE->GetEnvParam("[NetManager]BufferSize");
if (BufSize!=ASN_NOT_FOUND_PARAM) SetParam("BufferSize",BufSize.c_str());
else
SetParam("BufferSize","1024");
//BufferSize=1024;
cout << "CNetSupervisor::CNetSupervisor " << BufSize << endl;
//Engine=_Engine;

OpenSocketPack=NULL;
set=NULL;
	// initialize SDL_net
	if(SDLNet_Init()==-1)
	{
LOGGER->ErrMsgStr(2, "NET SUPERVISOR: SDLNet_Init: %s\n"+std::string(SDLNet_GetError()) );
	}

LOGGER->LogMsg("+CNetSupervisor");

};

void CNetSupervisor::InitManager(){

};

int CNetSupervisor::Release(){
    printf("Release supervisor...\n");
    delete this;
    return 0;
};

void CNetSupervisor::Update(float tms){
    if (!num_open_socket) return;
    Check();
};

CNetSupervisor::~CNetSupervisor(){

	if (message) free(message);

	while (num_open_socket>0) {
	    printf("num open socket: %d\n", num_open_socket);
	// пока еще есть активные соединения
            CloseListener(num_open_socket-1); //удаляем последнее
	    }

	// shutdown SDL_net
	SDLNet_Quit();
	LOGGER->LogMsg("-CNetSupervisor");
};

INetProtocolHandler *CNetSupervisor::OpenServerListener(const char* _HandlerName, const char* _Protocol, int port, const char* Init_string){

INetProtocolHandler *NewHandler=FindHandler(_HandlerName);
if (NewHandler!=NULL) CloseListener(_HandlerName); // если такой листенер уже найден - закрываем его

NewHandler = dynamic_cast<INetProtocolHandler*>(ENGINE->CreateObject( ASN_Str(ASN_Str(_Protocol)+"_Server").c_str() )); // создаём сервер указанного протокола

    if (!NewHandler)
    {
        LOGGER->ErrMsgStr(2,ASN_Str("NET SUPERVISOR: Сan't open ")+_Protocol+" "+IntToStr( port )+" server."  );
        return NULL;
    };

IPaddress _new_ip;

	if(SDLNet_ResolveHost(&_new_ip,NULL,port)==-1)
	{
		printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		NewHandler->Release();
		return NULL;
	}

TCPsocket _new_socket=SDLNet_TCP_Open(&_new_ip);

	if(!_new_socket)
	{
		printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
		NewHandler->Release();
		return NULL;
	};

NewHandler->SupervisorId=num_open_socket;
NewHandler->Init( Init_string );
AddConnection(_new_socket,NewHandler,true);
HandlerPack[_HandlerName]=NewHandler;

return NewHandler;
};

INetProtocolHandler *CNetSupervisor::OpenClientListener(const char* _HandlerName, const char* _Protocol, const char* Server_Name, int port, const char* Init_string){

INetProtocolHandler *NewHandler=FindHandler(_HandlerName);
if (NewHandler!=NULL) CloseListener(_HandlerName);

NewHandler = dynamic_cast<INetProtocolHandler*>( ENGINE->CreateObject( ASN_Str(std::string(_Protocol)+"_Client").c_str() ) );
if (!NewHandler) {
LOGGER->ErrMsgStr(2,ASN_Str("NET SUPERVISOR: can't open ")+_Protocol+":"+IntToStr( port )+" client."  );
return NULL;};

IPaddress _new_ip;

	if(SDLNet_ResolveHost(&_new_ip,(char*)Server_Name,port)==-1)
	{
		printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		NewHandler->Release();
		return NULL;
	}

TCPsocket _new_socket=SDLNet_TCP_Open(&_new_ip);

	if(!_new_socket)
	{
		printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
		NewHandler->Release();
		return NULL;
	}

    printf("Add client listener...\n");

NewHandler->SupervisorId=num_open_socket;

//UserName = NickName

std::string ClientParams = ASN_Str("ExtLogName=")+_Protocol+"_"+Server_Name+";"+Init_string;

NewHandler->Init( ClientParams.c_str() );

//printf("Add client listener...\n");

NewHandler->SetParam("ServerName",Server_Name );

AddConnection(_new_socket,NewHandler,false);
HandlerPack[_HandlerName]=NewHandler;

return NewHandler;
};

bool CNetSupervisor::AddCustomListener(INetProtocolHandler* handler, const char* Server_Name, int port){

IPaddress _new_ip;
bool BoolServer=false;

    if (Server_Name=="-") {

    	if(SDLNet_ResolveHost(&_new_ip,NULL,port)==-1){
		 printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
	 	 return true; //низя. чего-то напутали мы
         }
         BoolServer=true; // мы сами себе сервер
    }
    else {

    	if(SDLNet_ResolveHost(&_new_ip,(char*)Server_Name,port)==-1){
		 printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
	 	 return true; //низя. чего-то напутали мы
         }

	}

TCPsocket _new_socket=SDLNet_TCP_Open(&_new_ip);

	if(!_new_socket)
	{
		printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
		return true; // низя. чего-то напутали мы
	};

handler->SupervisorId=num_open_socket;
handler->Init("");
handler->SetParam("ServerName",Server_Name);

AddConnection(_new_socket,handler,BoolServer); //TODO
return false;

};

void CNetSupervisor::Check(){
int i;
int numready;
long int len=0;

//printf("check...\n");

ReCreateSockSet();

if (!set) return;

numready=SDLNet_CheckSockets(set, 0 /*0(Uint32)-1*/);

if(numready==-1)
			LOGGER->ErrMsgStr(2,std::string("SDLNet_CheckSockets: ")+SDLNet_GetError());


if(numready==0) return;

//printf("NUMREADY: %d\n",numready);

		for(i=0; /*numready && */ i<num_open_socket; i++) {

		if(SDLNet_SocketReady(OpenSocketPack[i].socket)){ //если сокет что-то хочет нам передать или готов к отправке в него...
		  numready--;

		  printf("Socket Type: %s\n", OpenSocketPack[i].handler->GetParam("Type") );

          if ( OpenSocketPack[i].ServerSocket ) { //TODO

          int hndlr=OpenSocketPack[i].handler->HandleMessage(OpenSocketPack[i].handler_id,message,0);
          if (hndlr!=100) OpenSocketPack[i].handler_id=hndlr; // 100 - удаление данного сокета

          /*
                    TCPsocket new_tcpsock;
                    new_tcpsock=SDLNet_TCP_Accept(OpenSocketPack[i].socket);
                    SDLNet_TCP_Close(OpenSocketPack[i].socket); //закрываем сокет
                    OpenSocketPack[i].socket=new_tcpsock;
                    OpenSocketPack[i].ServerSocket=false;

          if(!new_tcpsock) {
              printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError()); CloseListener(i); printf("CloseListener!!!\n"); continue;}

              printf("socket num %d\n",i);
          int hndlr=OpenSocketPack[i].handler->HandleMessage(OpenSocketPack[i].handler_id,message,len);
          if (hndlr!=100) OpenSocketPack[i].handler_id=hndlr; // 100 - удаление данного сокета
          */

          }
          else {
          len=0;

            len=GetMsg(OpenSocketPack[i].socket,message,BufferSize/*1024*/); // читаем сокет клиента
            printf("Len read: %ld\n", len);
            if (len<0) { CloseListener(i);
                printf("CloseListener!!!\n");continue;} //если ошибка - удаляем соединение
//          current_socket=OpenSocketPack[i].socket;

          int hndlr=OpenSocketPack[i].handler->HandleMessage(OpenSocketPack[i].handler_id,message,len);
          if (hndlr!=100) OpenSocketPack[i].handler_id=hndlr; // 100 - удаление данного сокета
          else printf("Listener Destroyed");

//            len=GetMsg(OpenSocketPack[i].socket,message,1024); // читаем сокет клиента
//            printf("Len read2: %d\n", len);
//          hndlr=OpenSocketPack[i].handler->HandleMessage(OpenSocketPack[i].handler_id,message,len);
//          if (hndlr!=100) OpenSocketPack[i].handler_id=hndlr; // 100 - удаление данного сокета
//          else printf("Listener Destroyed");

          }

		 }

		};

};

void CNetSupervisor::AddConnection(TCPsocket in_socket,INetProtocolHandler *handler, bool Server){

	OpenSocketPack=(COpenSocket*)realloc(OpenSocketPack, (num_open_socket+1)*sizeof(COpenSocket));
	OpenSocketPack[num_open_socket].ServerSocket=Server;
	OpenSocketPack[num_open_socket].socket=in_socket;
	OpenSocketPack[num_open_socket].handler=handler;
	OpenSocketPack[num_open_socket].handler_id=0;
	num_open_socket++;

	ReCreateSockSet();

};

void CNetSupervisor::CloseListener(const char* _HandlerName){
// NOT RELEASED :)
//    INetProtocolHandler *NewHandler=FindHandler(_HandlerName);
//    if (NewHandler!=NULL) CloseListener(_HandlerName); // если такой листенер уже найден - закрываем его
};

void CNetSupervisor::CloseListener(int Open_Socket_Id){

//    printf("try close listener %d\n", Open_Socket_Id);

	if(Open_Socket_Id<0 && Open_Socket_Id>=num_open_socket) return;

/// последний выдох господина ПЖ - вызываем HandleMessage с буфером NULL и длиной принятой информации 0
/// этим мы информируем что сокет закрыт

    OpenSocketPack[Open_Socket_Id].handler->HandleMessage(OpenSocketPack[Open_Socket_Id].handler_id,NULL,0);

    SDLNet_TCP_Close(OpenSocketPack[Open_Socket_Id].socket); //закрываем сокет клиента

//    printf("close listener3\n");

    OpenSocketPack[Open_Socket_Id].handler->Release();
//    printf("close listener4\n");

    num_open_socket--;

//    printf("close listener5\n");

	if(num_open_socket>Open_Socket_Id)
		memmove(&OpenSocketPack[Open_Socket_Id], &OpenSocketPack[Open_Socket_Id+1], (num_open_socket-Open_Socket_Id)*sizeof(COpenSocket));
	OpenSocketPack=(COpenSocket*)realloc(OpenSocketPack, num_open_socket*sizeof(COpenSocket));

//    printf("close listener6\n");

	ReCreateSockSet();

};

INetProtocolHandler *CNetSupervisor::FindHandler(const char* _HandlerName){
    std::map<std::string,INetProtocolHandler *>::iterator v_it=HandlerPack.find(_HandlerName);
    if (v_it!=HandlerPack.end()) return (*v_it).second;
    return NULL;
};

/*
int CNetSupervisor::SendMsg(std::string _HandlerName, std::string To, std::string Message){

INetProtocolHandler *NewHandler=FindHandler(_HandlerName);
if (NewHandler==NULL) return -1;
printf("Supervisor try saying...\n");
NewHandler->SendMsg(To.c_str(),Message.c_str());
return 0;
};
*/


bool CNetSupervisor::AcceptSocket(unsigned int Open_Socket_Id,unsigned int handler_id){

              TCPsocket new_tcpsock;
              new_tcpsock=SDLNet_TCP_Accept(OpenSocketPack[Open_Socket_Id].socket);

              if(!new_tcpsock) {
                            printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());  return true;
                            }

//                    SDLNet_TCP_Close(OpenSocketPack[Open_Socket_Id].socket); //закрываем сокет
//                    OpenSocketPack[Open_Socket_Id].socket=new_tcpsock;
//                    OpenSocketPack[Open_Socket_Id].ServerSocket=false;
//                    OpenSocketPack[Open_Socket_Id].handler_id=handler_id;

                    AddConnection(new_tcpsock,OpenSocketPack[Open_Socket_Id].handler,false);
                    OpenSocketPack[num_open_socket-1].handler_id=handler_id;
                    return false;
};

int CNetSupervisor::SendMsg(unsigned int Open_Socket_Id, char *buffer, int buf_size){
printf("try send on %d socket\n",Open_Socket_Id);
if(Open_Socket_Id<0 && Open_Socket_Id>=num_open_socket) return -1;
return PutMsg(OpenSocketPack[Open_Socket_Id].socket,buffer,buf_size);
};

// **************** функция создания набора сокетов *******************************
// создаем набор сокетов из всех сокетов в массиве + listener
void CNetSupervisor::ReCreateSockSet(){
int i;

if(set)	SDLNet_FreeSocketSet(set); // если набор был ранее уже создан - освобождаем

set=SDLNet_AllocSocketSet(num_open_socket);
	if(!set) {
		printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		set=NULL; return;
	}

	for(i=0;i<num_open_socket;i++) {
		SDLNet_TCP_AddSocket(set,OpenSocketPack[i].socket);
		OpenSocketPack[i].handler->SupervisorId=i;
  }
};

// **************** функция приема сообщения из сокета **************************
// на входе - сокет, буфер, размер буфера
// на выходе - размер принятого сообщения или -1 в случае ошибки
int CNetSupervisor::GetMsg(TCPsocket socket, char *buffer, int buf_size){
int len=SDLNet_TCP_Recv(socket,buffer,buf_size);
printf("%d GET--> %d", buf_size, len );
		if(!len) return (-1);
//FILE *f=fopen ("log.txt","a+");
//buffer[len]='\0';
//printf("%s \n",buffer);
//fprintf(f,"%s \n",buffer);
//fclose(f);
return len;
};

// *************** функция отправки сообщения в сокет ****************************
// на входе - сокет, буфер, размер передаваемого сообщения
// на выходе - размер переданного сообщения или -1 в случае ошибки
int CNetSupervisor::PutMsg(TCPsocket socket, char *buffer, int buf_size){
int len=SDLNet_TCP_Send(socket,buffer,buf_size);
printf("%d <--PUT %d", buf_size, len );
		if(!len) return (-1);
return len;
};

void CNetSupervisor::SetParam(const char* param_name, const char* param_value){

    if ( strcmp(param_name,"BufferSize")==0) {
		if (message) free(message);
                BufferSize=StrToInt(param_value);
		message = new char[BufferSize];
		};

    CBaseObject::SetParam(param_name, param_value);
};
