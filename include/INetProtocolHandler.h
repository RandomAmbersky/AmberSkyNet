#ifndef _INetProtocolHandler_H
#define _INetProtocolHandler_H

#include <string>
#include "IBaseObject.h"

class INetProtocolHandler: public virtual IBaseObject {
public:

//virtual int Release()=0;
//virtual ~INetProtocolHandler(){};
//virtual void Init(unsigned int supervisor_id,std::string Init_String)=0;
virtual void Init( const char* Init_String )=0;

//// в случае если buffer==NULL функция вызывана перед закрытием сокета
virtual unsigned int HandleMessage(unsigned int handler_id, char *buffer, long int buf_size)=0;

//virtual void SendMessage(std::string To, std::string Message)=0;
//virtual void SendMsg(const char *To, const char *Message)=0;

// true - не удалось отправить или команда не поддерживается
virtual bool Send(const char* To, const char* Operation, const char* Value)=0;

unsigned int SupervisorId;
//unsigned int ListenerID;

};

typedef void (*CALLBACK_PROTOCOL_HANDLER)( INetProtocolHandler* , void* );

#endif
