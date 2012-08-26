#include "INetSupervisor.h"
#include "HTTP_Client.h"
#include "ILog.h"
#include "ASN_utils.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

#define CR '\r'
#define LF '\n'
#define CRLF "\r\n"

HTTP_Client::HTTP_Client(IEngine *_Engine):CBaseObject(_Engine){

    SetParam("BaseClass","NetProtocolHandler");
    SetParam("Type","HTTP_Client");
    SetParam("Name","-");

    filesize=0;
    header_on=true;

    CustomHandler=NULL;

    LOGGER->LogMsg("HTTP_Client: Open");

};

HTTP_Client::~HTTP_Client(){

    LOGGER->LogMsg("HTTP_Client: Close");

//    if (CustomHandler) {
//	SetPtrParam("Buffer", char_ptr);
//	SetParam("BufferSize", IntToStr(buffer_size).c_str() );
//	CALLBACK_PROTOCOL_HANDLER myCustomHandler = (CALLBACK_PROTOCOL_HANDLER)CustomHandler;
//	myCustomHandler(this, NULL);
//        }

//    printf("Close listener...\n");

};

void HTTP_Client::Init(const char* Init_String){

};


void HTTP_Client::ParseParams(const std::string& InpString){

    std::string Param;
    std::string Value;

    if ( StrToParamValue(InpString,':',Param,Value) ) {
    Value = Trim( Value );
    printf("%s=%s\n",Param.c_str(),Value.c_str());
    Param = std::string("[Header]")+Param; // а то мало ли что...
    SetParam(Param.c_str(),Value.c_str());
    };

}

void HTTP_Client::Parse_HTTP_Header(char* ptr){

    int cnt = 0;
    char* ptr_start = ptr;
    
    std::string InpString;

/// разбираем ответ сервера HTTP/1.1 200 OK \r\n ///////////////////
/// присваиваем имена переменных согласно описанию RFC 2068

    int out_num = 0;
    
    while ( (ptr[cnt]!='\r')&&( ptr[cnt+1]!='\n') ) {

	if ( ptr[cnt]==' ' ) {
	    ptr[cnt]='\0';		    
	    printf("header:{%s}\n", ptr_start);
            if (out_num==0) SetParam("[Header]HTTP-Version",ptr_start);
	    if (out_num==1) SetParam("[Header]Status-Code",ptr_start); // если HTTP/1.версия уже разобрали
	    out_num++;
	    ptr_start=ptr+cnt+1;
	}
	
	cnt++;
    }

    ptr[cnt]='\0';
    
    printf("header:{%s}\n", ptr_start);
    SetParam("[Header]Reason-Phrase", ptr_start);

    cnt++;
    cnt++;
    ptr_start= ptr+cnt;

    while (ptr[cnt]!='\0')
    {
	if ( (ptr[cnt]=='\r')&&( ptr[cnt+1]=='\n') ) { 
	    ptr[cnt]='\0';

	    InpString = ptr_start;
	    ParseParams( InpString );	    
	    		    
	    cnt++;
	    cnt++;

//	    printf("string: {%s}\n",ptr_start);

	    ptr_start=ptr+cnt;
	    }
	cnt++;
    }


    InpString = ptr_start;
    ParseParams( InpString );	    

};

unsigned int HTTP_Client::HandleMessage(unsigned int handler_id, char *buffer, long int buf_size){

if ( (!buffer) && (buf_size==0) ) return handler_id;

////  сокет закрыт 
if (!buffer) {

    if (CustomHandler) {
	SetPtrParam("Buffer", NULL );
	SetParam("BufferSize", "0" );
	CALLBACK_PROTOCOL_HANDLER myCustomHandler = (CALLBACK_PROTOCOL_HANDLER)CustomHandler;
	myCustomHandler(this, NULL);
        }

    return handler_id;
}

int LoadSize=StrToInt( GetParam("LoadSize") );
LoadSize+=buf_size;
//printf("\n [%s] incoming .. %d\n",GetParam("LoadSize"),buf_size);
SetParam ("LoadSize", IntToStr( LoadSize ).c_str() );

//    FILE *f=fopen("http.log","ab+");
//    fwrite(buffer,buf_size,1,f);
//    fclose(f);

  filesize+=buf_size;
//  buffer[buf_size]='\0';

    char* char_ptr=buffer;
    int buffer_size = buf_size;

/////// HEADER /////////////////////////    
  if (header_on) {

    int cnt=0;

    while (cnt < buf_size){
	if ( ( buffer[cnt]== CR ) && ( buffer[cnt+1]== LF ) &&
	  ( buffer[cnt+2]== CR ) && ( buffer[cnt+3]== LF ) ) {

	    buffer[cnt]='\0';
	    printf("=============HTTP header===============\n");
	    printf("%s\n", char_ptr);
	    printf("=======================================\n");
	    // теперь парсим заголовок и заносим параметры к себе
	    Parse_HTTP_Header(buffer);

	    header_on=false;
	    char_ptr = char_ptr+cnt+4;
	    buffer_size = buffer_size - cnt - 4;
	    break;
	    }
        cnt++;
        };

    }

    if (CustomHandler) {
	SetPtrParam("Buffer", char_ptr);
	SetParam("BufferSize", IntToStr(buffer_size).c_str() );
	CALLBACK_PROTOCOL_HANDLER myCustomHandler = (CALLBACK_PROTOCOL_HANDLER)CustomHandler;
	myCustomHandler(this, NULL);
	// если после вызова пользовательской обработки указатель на обработчик исчез - значит надо закрывать соединение
	// и возвращать условный код 100 - признак того что соединение закрыто
	if (!CustomHandler) {NETTER->CloseListener(SupervisorId); return 100; }
        }

    return handler_id;

};

int HTTP_Client::Release(){

	printf("HTTP_Client::Release\n");
	if (CustomHandler) {

	CALLBACK_PROTOCOL_HANDLER myCustomHandler = (CALLBACK_PROTOCOL_HANDLER)CustomHandler;
	myCustomHandler(this, (void*)"release");

	}
	return 0;
};

bool HTTP_Client::Send(const char* Operation, const char* To, const char* Value){

    std::string send = Operation;
    send = send + " " + To + " " + "HTTP/1.0";
    send = send + "\r\n\r\n";
    
    printf("try send %s\n",send.c_str());
    NETTER->SendMsg(SupervisorId,(char *)send.c_str(),send.length());
    SetParam("LoadSize", "0" ); // все сбрасываем и обнуляем
    
};

// наследство от IBaseObject
void HTTP_Client::SetPtrParam(const char* param_name, void *param_value){
    if ( std::string(param_name)==std::string("Handler") ) CustomHandler=param_value;
    CBaseObject::SetPtrParam(param_name, param_value);
};
