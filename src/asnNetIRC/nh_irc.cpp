
#include "nh_irc.h"
#include "IRC_replay.h"

#include "ILog.h"
#include "ASN_utils.h"

#include "INetSupervisor.h"
#include "IEngine.h"

#include "IFileSystem.h"

//#include <stdio.h>
#include <vector>
#include <time.h>

#define ENGINE Engine //у нас уже есть указатель на Engine

NH_IRC::NH_IRC(IEngine *_Engine):CBaseObject(_Engine){
Engine=_Engine;
CustomHandler=NULL;
NumTryConnect=0;

SetParam("BaseClass","NetProtocolHandler");
SetParam("Type","IRC_Client");
SetParam("Name","-");

SetParam("NickName","UserX");
SetParam("UserName","GuestUser");
SetParam("HostName","*");
SetParam("ServerName","*");
SetParam("RealName","User Of AmberSkynetVR");

SetParam("USERINFO","Powerful user of AmberChat, based on AmberSkyNet VR Engine (http://ambernet.sf.net)");

//Field 1: Client name and version
//Field 2: Operating system name and version
//Field 3: Contact for individual/organization responsible for client.
//These will be combined as follows:
//<marker> VERSION <field 1> <space> <field 2> <space> <field 3> <marker>

SetParam("VERSION","AmberChat v0.1 (http://ambernet.sf.net)");

FillReplay();

LOGGER->LogMsg("NH_IRC: Open");
SupervisorId=0;
cut_buffer="";
ExtLogName="irc_log.txt";
};

NH_IRC::~NH_IRC(){

LOGGER->LogMsg("NH_IRC: Close");

};

void NH_IRC::SetPtrParam(const char* param_name, void *param_value){
    if ( std::string(param_name)=="Handler") CustomHandler=param_value;
    CBaseObject::SetPtrParam(param_name, param_value);
};

void NH_IRC::Init(const char* Init_String){
//SupervisorId=supervisor_id;

int pos,pos2=0;

char param[80];
char value[80];

/*
CParamList ParamList;

	  std::string Parse_String=Init_String;
      std::string Parse_String_Part;
	  while(pos!=-1) {
	     pos=Parse_String.find_first_of('|',0);
	     Parse_String_Part=Parse_String.substr(0,pos);
	     pos2=Parse_String_Part.find_first_of(' ',0);
	     if (pos2!=0)
	     ParamList.SetParam(Parse_String_Part.substr(0,pos2),
                            Parse_String_Part.substr(pos2+1,Parse_String_Part.size())
                            );
	     Parse_String=Parse_String.substr(pos+1);
	     }
*/


//asn_List myList;

StrToList(Init_String,Content);

//printf("try init %s\n",Init_String);

if ( Content.find("ExtLogName")!=Content.end() ) ExtLogName=Content["ExtLogName"];
    else ExtLogName="irc_log.txt";

//if ((ExtLogName=ParamList.GetStrParam("ExtLogName"))=="N/A") ExtLogName="irc_log.txt";

//if ( myList.find("UserName")!=myList.end() ) SetParam("NickName",myList["UserName"].c_str());//LoginName=myList["UserName"];
//    else SetParam("NickName","UserX");

//if ((UpdateScript=ParamList.GetStrParam("UpdateScript"))=="N/A") UpdateScript="";

};

/*
void NH_IRC::SendMsg(const char *To, const char *Message){
SendMessage( std::string(To), std::string(Message) );
};
*/

/*
void NH_IRC::SendMessage(std::string To, std::string Message){

if (To==";") { // команда. в IRC ; символ вроде бы не может использоваться в имени
    NETTER->SendMsg(SupervisorId,(char *)(Message+"\n").c_str(),Message.length()+1);
    printf("IRC Send message: %s %s\n",To.c_str(),Message.c_str());
}
else {
//  Say( "IRC_loc", ";", "PRIVMSG "..GetParam("GUIManager.L_CurrentChannel","Text").." :"..GetParam("Text") )
    std::string Msg="PRIVMSG "+To+" :"+Message+"\n";
    NETTER->SendMsg(SupervisorId,(char *)Msg.c_str(),Msg.length());
    printf("IRC Send message: %s \n",Msg.c_str());
}

};
*/

bool NH_IRC::Send(const char* Operation, const char* To, const char* Value){
std::string send;

if ( ASN_Str(To)=="server" ) {
// служебная команда серверу

send=ASN_Str(Operation)+" "+ASN_Str(Value)+"\n";
NETTER->SendMsg(SupervisorId,(char *)send.c_str(),send.length());

}
else if ( ASN_Str(Operation)=="SENDFILE" ) { //отправляем файл

printf("try send...\n");

IFile *myFile=FILER->GetFile( Value );
if (!myFile) return true; //файла нет такого
myFile->FileOpen();
myFile->LoadInMem();
if (myFile->GetState()!=ASN_FILE_DISK_IN_MEM) return true;
printf("File size: %d",myFile->GetSizeInBytes());


         INetProtocolHandler *tmp= new NH_IRC_DCC_SENDFILE( Engine );
         NETTER->AddCustomListener( tmp, "-" , 14000 ); // TODO

         printf("AddCustomListener...\n");

         tmp->SetPtrParam("Handler",CustomHandler);
         tmp->SetPtrParam("FilePTR",myFile);

         tmp->SetParam("UserName",To);
         tmp->SetParam("FileName",Value);

//std::string SizeInBytes= myFile->GetSizeInBytes()

         tmp->SetParam("FileSize", IntToStr( myFile->GetSizeInBytes() ).c_str() ); //TODO
         tmp->SetParam("LoadSize", "0" );

         printf("Set Param OK...\n");

      char spec=0x1;
      std::string mess=spec+std::string("DCC SEND ")+Value+" 2886730392 14000 "+IntToStr( myFile->GetSizeInBytes() )+spec+"\n";
      Send( "PRIVMSG", To, mess.c_str() );

}
else {
// будем считать что это приватное сообщение
send=ASN_Str("PRIVMSG ")+ASN_Str(To)+" :"+ASN_Str(Value)+"\n";
NETTER->SendMsg(SupervisorId,(char *)send.c_str(),send.length());
};

printf("IRC Send message: %s %s\n",To,send.c_str());
return false;
};

unsigned int NH_IRC::HandleMessage(unsigned int handler_id, char *buffer, long int buf_size){

if (!buffer) return handler_id;

char message[256];

std::string AllMessages;
std::string OutString;

if (buffer) buffer[buf_size]='\0';
AllMessages=cut_buffer+buffer;

std::string Message;
int pos=0; //главный цикл

std::string Nick="";
std::string User="";
std::string Host="";
std::string Action="";

std::string Other="";

std::vector<std::string> Params;
//std::string Sender="";
//std::string Params="";
//std::string Params2="";

bool automated = true; // надо ли вызывать Custom-обработчик или обошлись своими силами

while (pos!=-1) {
    pos=AllMessages.find_first_of("\n");
    if (pos!=-1) {
        Message = AllMessages.substr(0,pos-1);
        AllMessages=AllMessages.substr(pos+1,AllMessages.length()-pos+1);
        } else Message="";

    if (Message.length()==0) continue;

//    ((CEventLog *)MegaBase->GetVoidParam("EventLog"))->ExtLogMsg(ExtLogName,"---Message---");
//    ((CEventLog *)MegaBase->GetVoidParam("EventLog"))->ExtLogMsg(ExtLogName,(char*)(">"+Message+"<").c_str());
//    ((CEventLog *)MegaBase->GetVoidParam("EventLog"))->ExtLogMsg(ExtLogName,"---End Message---");

printf("---Message---\n");
printf("%s\n",Message.c_str());
printf("---End Message---\n");

// первоначальная стадия
if (handler_id==0) {
//SendMessage(";","NICK "+LoginName+"\nUSER AmberNetVR Amber server :"+GetParam("UserName")+"\n");
std::string send="NICK "+std::string(GetParam("NickName"))+"\nUSER "+std::string(GetParam("UserName"))+" "+std::string(GetParam("HostName"))+" "+GetParam("ServerName")+"  :"+GetParam("RealName")+"\n";

//SendMessage(";",send);
Send("NICK","server", std::string(GetParam("NickName")).c_str() );
Send("USER","server", ASN_Str( std::string(GetParam("UserName"))+" "+std::string(GetParam("HostName"))+" "+GetParam("ServerName")+"  :"+GetParam("RealName")).c_str() );

handler_id=1;
//continue;
};

if (Message.substr(0,4) == "PING"){ // получили ПИНГ
Send("PONG","server",Message.substr(6,Message.length()-6).c_str());
//SendMessage(";","PONG "+Message.substr(6,Message.length()-6)+"\n");
std::string OutText="NH_IRC: PING-PONG...";
//LOGGER->ExtLogMsg(ExtLogName, IntToStr( Message.length()) + OutText );
automated=false;
continue;
};

if (Message.at(0)==':'){
    // (nick!user@host)
    int tmp_pos=Message.find_first_of(" !",1);
    if (tmp_pos==-1) {continue;}
    else {
    Nick=Message.substr(1,tmp_pos-1);
    Message=Message.substr(tmp_pos);
//    printf("Nick: >%s<\n",Nick.c_str());
    Params.push_back( Nick ); // Params[0] - Nick

        if(Message.at(0)=='!') { //только если это не просто ник, а nick!user@host - имеет смысл разбирать дальше
                int tmp_pos=Message.find_first_of(" @",1);
                if (tmp_pos==-1) {}
                else {
                User=Message.substr(1,tmp_pos-1);
                Message=Message.substr(tmp_pos);
//                printf("User: >%s<\n",User.c_str());

                if(Message.at(0)=='@') { //только если это не просто ник, а nick!user@host - имеет смысл разбирать дальше
                                int tmp_pos=Message.find_first_of(" ",1);
                                if (tmp_pos==-1) {}
                                else {
                                Host=Message.substr(1,tmp_pos-1);
                                Message=Message.substr(tmp_pos);
//                                printf("Host: >%s<\n",Host.c_str());
                                };
                                };
                };
         }; //if(Message.at(0)=="!")
    };

}; //if (Message.at(0)==':'){

Message=Message.substr(1);

int tmp_pos=Message.find_first_of(" ",1);
if (tmp_pos!=-1){
Action=Message.substr(0,tmp_pos);
if (irc_cd.find(Action)!=irc_cd.end()) Action=irc_cd[Action];
Message=Message.substr(tmp_pos+1);
//printf("Action: >%s<\n",Action.c_str());
Params.push_back( Action ); // Params[1] - Action
}

Other=Message;

while (Message.size()>0){

    if (Message.at(0)==':'){
    Params.push_back( Message.substr(1) );
//    printf("Params >%s<\n",Params.c_str());
    Message="";//Message.substr(tmp_pos+1);
    break;
    }

    else {
    int tmp_pos=Message.find_first_of(" ",1);
    if (tmp_pos==-1) {
        Params.push_back( Message );
        Message="";
        break;
        };
    Params.push_back( Message.substr(0,tmp_pos) );
    Message=Message.substr(tmp_pos+1);
    };

//printf("Params >%s<\n",Params.c_str());
}; //while (Message.size>0)

// если уже зашли под таким ником - приделываем палочки к нику,
// а если и это не помогло - прибавляем к нику цифру
if (Action=="ERR_NICKNAMEINUSE"){
    NumTryConnect++;
    std::string NickName=GetParam("NickName");
    if (NumTryConnect==1) {
    NickName=NickName+"_";
    } else
    if (NumTryConnect==2) {
    NickName="_"+NickName;
    } else {
    NickName=NickName+IntToStr( NumTryConnect );
    };

    SetParam("NickName", NickName.c_str() );
    Send("NICK","server", NickName.c_str() );

}
else
// проверка на CTCP
if ( Params[1]=="PRIVMSG" ) {
std::string par1=Params[3];
printf("{%s}\n",par1.c_str());

if (par1.at(0)=='\001') {
    /// это ж у нас CTCP !
    par1=par1.substr(1); // отделяем признак CTCP в начале
    int tmp_pos=par1.find_first_of('\001',1);
    if (tmp_pos==-1) break;

    par1=par1.substr(0,tmp_pos); // отделяем признак CTCP в конце

    std::string CTCP_type=par1;
    tmp_pos=par1.find_first_of(' ',1);

    std::string par2=par1.substr(tmp_pos+1);
    if (tmp_pos!=-1) CTCP_type=par1.substr(0,tmp_pos); // если это например PING, отделяем всё остальное

    printf("{CTCP: %s}\n",CTCP_type.c_str());

    Params.pop_back();

    Params.push_back( CTCP_type );
    Params.push_back( par2 );

//    if (CTCP_type=="ACTION") {
//        printf("AC-Ti-i-i-iooonn!!!!\n");
//        Send("NOTICE","server",Nick+" :\001ACTION внимательно наблюдает \001");
//        automated=false;
//   }
//    else
    if (CTCP_type=="USERINFO") {
//        SendMessage(";","NOTICE "+Nick+" :\001USERINFO "+GetParam("USERINFO")+"\001");
          Send("NOTICE","server",ASN_Str(Nick+" :\001USERINFO "+GetParam("USERINFO")+"\001").c_str());
        automated=false;
        }
    else if (CTCP_type=="VERSION") {
//        SendMessage(";","NOTICE "+Nick+" :\001VERSION "+GetParam("VERSION")+"\001");
          Send("NOTICE","server",ASN_Str( Nick+" :\001VERSION "+GetParam("VERSION")+"\001" ).c_str() );
        automated=false;
        }
    else if (CTCP_type=="PING") {
    // хотя в документации написано, что ответ должен быть PONG !!!! :/
    // <marker> PONG <space> <arg> <marker>
        tmp_pos=par1.find_first_of(' ',1);
//        SendMessage(";","NOTICE "+Nick+" :\001PING "+par1.substr(tmp_pos+1,Message.length()-tmp_pos-1)+"\001");
           Send("NOTICE","server",ASN_Str(Nick+" :\001PING "+par1.substr(tmp_pos+1,Message.length()-tmp_pos-1)+"\001").c_str() );
        automated=false;
        }
    else if (CTCP_type=="TIME") {
        static char time_buffer[ 1024 ];
        time_t time_of_day=time(NULL);
        strftime( time_buffer, 80, "%d %H:%M:%S %Z %Y", localtime( &time_of_day ) );
//        SendMessage(";","NOTICE "+Nick+" :\001TIME "+std::string(time_buffer)+"\001");
            Send("NOTICE","server",ASN_Str(Nick+" :\001TIME "+std::string(time_buffer)+"\001").c_str());
        automated=false;
   }
    else if (CTCP_type=="DCC") {
    std::string DCC_type;
    tmp_pos=par1.find_first_of(' ',1);
    par1=par1.substr(tmp_pos+1); // отрезаем команду DCC
    tmp_pos=par1.find_first_of(' ',1);
    if (tmp_pos!=-1) DCC_type=par1.substr(0,tmp_pos);
    par1=par1.substr(tmp_pos+1);
    printf("DCC type: {%s}\n",DCC_type.c_str());

     if (DCC_type=="CHAT") {
     tmp_pos=par1.find_first_of(' ',1);
     std::string dcc_Protocol=par1.substr(0,tmp_pos);
     par1=par1.substr(tmp_pos+1);
     printf("chat protocol: {%s}\n",dcc_Protocol.c_str());
     if (dcc_Protocol=="chat") { // обычный dcc чат-протокол
        tmp_pos=par1.find_first_of(' ',1);
        std::string rawIP=par1.substr(0,tmp_pos);
//        printf( "Raw IP: %s\n",rawIP.c_str() );
        unsigned int t;
        sscanf(rawIP.c_str(),"%u",&t);
//        printf( "Start IP: %u\n", t);
        std::string inetIP = IntToIP ( t /*StrToInt(rawIP)*/ );

        par1=par1.substr(tmp_pos+1);
        tmp_pos=par1.find_first_of(' ',1);
        std::string Port=par1.substr(0,tmp_pos);

        printf("{%s}:{%d}\n", inetIP.c_str() , StrToInt(Port) );

//      INetProtocolHandler *tmp=NETTER->OpenClientListener( std::string(GetParam("ServerName"))+"_"+Nick+"_DCC","IRC_DCC_CHAT",inetIP,StrToInt(Port),"","");

        INetProtocolHandler *tmp = new NH_IRC_DCC_CHAT( Engine );
//
        NETTER->AddCustomListener( tmp, inetIP.c_str() , StrToInt(Port) );
        tmp->SetPtrParam("Handler",CustomHandler);
        tmp->SetParam("UserName",Nick.c_str());
//        printf("dcc: %s", std::string( std::string(GetParam("ServerName"))+"_"+Nick+"_DCC" ).c_str() );
//        SetPtrParam(");
        };
    }; //    if (DCC_type=="CHAT")

    if (DCC_type=="SEND") {

       std::string SendFile;
//       printf("par1=%s\n",par1.c_str() );
       if (par1[0]=='"') {
              tmp_pos=par1.find_first_of('"',1);
              SendFile=par1.substr(1,tmp_pos-1);
              }
       else {
              tmp_pos=par1.find_first_of(' ',1);
              SendFile=par1.substr(0,tmp_pos);
            };

       printf("SendFile: %s\n",SendFile.c_str());

       par1=par1.substr(tmp_pos+1);
       tmp_pos=par1.find_first_of(' ',1);
       std::string rawIP=par1.substr(0,tmp_pos);

       printf("rawIP: %s\n",rawIP.c_str());

       unsigned int t;
       sscanf(rawIP.c_str(),"%u",&t);
       std::string inetIP = IntToIP ( t );

       par1=par1.substr(tmp_pos+1);
       tmp_pos=par1.find_first_of(' ',1);
       std::string Port=par1.substr(0,tmp_pos);

       par1=par1.substr(tmp_pos+1);
       tmp_pos=par1.find_first_of(' ',1);
       std::string FileSize=par1.substr(0,tmp_pos);

       printf("%s try send file {%s} with size %s from %s port: %s\n",
               Nick.c_str(), SendFile.c_str(), FileSize.c_str(), inetIP.c_str() , Port.c_str() );

         INetProtocolHandler *tmp= new  NH_IRC_DCC_GETFILE( Engine );
         NETTER->AddCustomListener( tmp, inetIP.c_str() , StrToInt(Port) );

         tmp->SetPtrParam("Handler",CustomHandler);
         tmp->SetParam("UserName",Nick.c_str());
         tmp->SetParam("FileName",SendFile.c_str());
         tmp->SetParam("FileSize",FileSize.c_str());
         tmp->SetParam("Port",Port.c_str());
         tmp->SetParam("LoadSize","0");

    }; // if (DCC_type=="SEND")

//        std::vector<std::string>::iterator _it=Params.begin();
//        while(_it!=Params.end()){
//        printf("Param:{%s}\n",(*_it).c_str());
//        _it++;
//        };


        automated=false;
   };

   };

};

if (automated)
if (CustomHandler) {
//SetParam("Nick",Nick.c_str());
//SetParam("Action",Action.c_str());

CALLBACK_PROTOCOL_HANDLER myCustomHandler = (CALLBACK_PROTOCOL_HANDLER)CustomHandler;
myCustomHandler(this, &Params);
}

Params.clear();

}; // while (pos!=-1)


cut_buffer=AllMessages.substr(0,AllMessages.length()-1);
return handler_id;
};



NH_IRC_DCC_CHAT::NH_IRC_DCC_CHAT(IEngine *_Engine):CBaseObject(_Engine){

SetParam("BaseClass","NetProtocolHandler");
SetParam("Type","IRC_DCC_Client");
SetParam("Name","-");

SupervisorId=0;
Engine=_Engine;
CustomHandler=NULL;

LOGGER->LogMsg("NH_IRC: Open");

};

NH_IRC_DCC_CHAT::~NH_IRC_DCC_CHAT(){
    LOGGER->LogMsg("NH_IRC_DCC_CHAT: Close");
};

void NH_IRC_DCC_CHAT::SetPtrParam(const char* param_name, void *param_value){
    if ( std::string(param_name)=="Handler") CustomHandler=param_value;
    CBaseObject::SetPtrParam(param_name, param_value);
};

void NH_IRC_DCC_CHAT::Init(const char* Init_String){
//SupervisorId=supervisor_id;

};

unsigned int NH_IRC_DCC_CHAT::HandleMessage(unsigned int handler_id, char *buffer, long int buf_size){
if (buffer) buffer[buf_size]='\0';
std::string Message=buffer;
//Message=Message.substr(1);

printf("DCC Chat message: %s\n",Message.c_str());

std::vector<std::string> Params;
Params.push_back( std::string(GetParam("UserName")) );
Params.push_back("PRIVMSG");
Params.push_back(Message);

if (CustomHandler) {
CALLBACK_PROTOCOL_HANDLER myCustomHandler = (CALLBACK_PROTOCOL_HANDLER)CustomHandler;
myCustomHandler(this,&Params);
}

//SendMsg("","Ok. Я почитаю ваше приватное сообщение позже.");

return handler_id;
};

bool NH_IRC_DCC_CHAT::Send(const char* Operation, const char* To, const char* Value){
std::string Msg=Value;
NETTER->SendMsg(SupervisorId,(char*)ASN_Str(Msg+"\n").c_str(),Msg.length()+1);
return false;
};

 NH_IRC_DCC_GETFILE:: NH_IRC_DCC_GETFILE(IEngine *_Engine):CBaseObject(_Engine){

SupervisorId=0;
Engine=_Engine;
CustomHandler=NULL;

SetParam("BaseClass","NetProtocolHandler");
SetParam("Type","NH_IRC_DCC_GETFILE");
SetParam("Name","-");

};

 NH_IRC_DCC_GETFILE::~ NH_IRC_DCC_GETFILE(){
    LOGGER->LogMsg(" NH_IRC_DCC_GETFILE: Close");
};

void  NH_IRC_DCC_GETFILE::Init(const char* Init_String){
//SupervisorId=supervisor_id;
};

unsigned int  NH_IRC_DCC_GETFILE::HandleMessage(unsigned int handler_id, char *buffer, long int buf_size){

//NETTER->CloseListener(SupervisorId);
//return 100;

std::string Path = ASN_Str( ENGINE->GetEnvParam("[Paths]LogPath") ) + GetParam("FileName");

FILE *f=fopen(Path.c_str(),"ab+");
//buffer[buf_size]='\0';
//printf("%s \n",buffer);
//fprintf(f,buffer,buf_size);
fwrite(buffer,buf_size,1,f);
fclose(f);

int LoadSize=StrToInt( GetParam("LoadSize") );
LoadSize+=buf_size;

printf("\n [%s] incoming .. %ld\n",GetParam("LoadSize"),buf_size);

//     std::string mess=spec+std::string("DCC SEND index.php 2886730392 80 21016")+spec+"\n";

SetParam ("LoadSize", IntToStr( LoadSize ).c_str() );
//LoadSize++;

int *t_ptr = &LoadSize;
char spec[4];
char *num_ptr=(char*)t_ptr;

//int *num=&LoadSize;
//char *num_ptr=(char*)&LoadSize;

spec[0]=num_ptr[3];
spec[1]=num_ptr[2];
spec[2]=num_ptr[1];
spec[3]=num_ptr[0];

//int out_size=buf_size;

//NETTER->SendMsg( SupervisorId,(char *)&out_size,sizeof(int) );

NETTER->SendMsg( SupervisorId, spec, 4 );
//char spec=0x1;

if ( LoadSize >= StrToInt( GetParam("FileSize") ) ) {
NETTER->CloseListener(SupervisorId);
return 100;
};

//std::string mess=":Test_User PRIVMSG "+std::string (GetParam("UserName") )+" :"+spec+"DCC RESUME "+std::string( GetParam("FileName") )+" "+GetParam("Port")+" "+GetParam("LoadSize")+spec+"\n";
//    NETTER->SendMsg( SupervisorId, (char *)mess.c_str(), mess.length() );
//Send("","",mess);

return handler_id;

};

bool  NH_IRC_DCC_GETFILE::Send(const char* Operation, const char* To, const char* Value){
    printf( "DCC_SEND: %s\n", Value );
    NETTER->SendMsg( SupervisorId,(char *)Value, strlen(Value)+1 );
    return false;
};


void  NH_IRC_DCC_GETFILE::SetPtrParam(const char* param_name, void *param_value){
    if ( std::string(param_name)=="Handler") CustomHandler=param_value;
    CBaseObject::SetPtrParam(param_name, param_value);
};


/*
void NH_IRC_DCC_CHAT::SendMsg(const char *To, const char *Message){
std::string Msg=Message;
NETTER->SendMsg(SupervisorId,(char *)(Msg+"\n").c_str(),Msg.length()+1);
};
*/



NH_IRC_DCC_SENDFILE::NH_IRC_DCC_SENDFILE(IEngine *_Engine):CBaseObject(_Engine){

SupervisorId=0;
Engine=_Engine;
CustomHandler=NULL;

SetParam("BaseClass","NetProtocolHandler");
SetParam("Type","NH_IRC_DCC_SENDFILE");
SetParam("Name","-");

};

NH_IRC_DCC_SENDFILE::~NH_IRC_DCC_SENDFILE(){
    LOGGER->LogMsg("NH_IRC_DCC_SENDFILE: Close");
};

void NH_IRC_DCC_SENDFILE::Init(const char* Init_String){
//SupervisorId=supervisor_id;
};

unsigned int NH_IRC_DCC_SENDFILE::HandleMessage(unsigned int handler_id, char *buffer, long int buf_size){

// первоначальная стадия
//if (handler_id==0) {
    NETTER->AcceptSocket(SupervisorId,1);
    NETTER->CloseListener(SupervisorId);
//return 100;
//};

int prim=0;
int *prim_ptr=&prim;

if (buf_size!=0) {
printf("BYTE0 %c\n",buffer[0]);
printf("BYTE1 %c\n",buffer[1]);
printf("BYTE2 %c\n",buffer[2]);
printf("BYTE3 %c\n",buffer[3]);
printf("number: %d\n",(int)&buffer);
};

// if (buf_size==0) { // свежий сокет, пошлём туда чего-нибудь :-)

 printf("lets file fly-y-y-y-y!!!\n");
 IFile *myFile=(IFile *) GetPtrParam("FilePTR");
 myFile->FileOpen();
 myFile->LoadInMem();
 if (myFile->GetState()!=ASN_FILE_DISK_IN_MEM) return handler_id;
 int send_len=NETTER->SendMsg( SupervisorId,(char *)( myFile->LoadInMem() ), myFile->GetSizeInBytes() );
 printf("bytes send : %d\n",send_len);
 NETTER->CloseListener(SupervisorId);
 return 100;

// };

return handler_id;

};

bool NH_IRC_DCC_SENDFILE::Send(const char* Operation, const char* To, const char* Value){

    return true; // ничего извне отправлять не буду !
};


void NH_IRC_DCC_SENDFILE::SetPtrParam(const char* param_name, void *param_value){
    if ( std::string(param_name)=="Handler") CustomHandler=param_value;
    CBaseObject::SetPtrParam(param_name, param_value);
};

