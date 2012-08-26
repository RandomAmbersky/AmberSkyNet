#ifndef _NH_JABBER_H
#define _NH_JABBER_H

#include "INetProtocolHandler.h"
#include "CBaseObject.h"
#include "IEngine.h"


class NH_Jabber: public INetProtocolHandler, public CBaseObject{
public:
NH_Jabber(IEngine *_Engine);
~NH_Jabber();
void Init(const char* Init_String);
unsigned int HandleMessage(unsigned int handler_id, char *buffer, long int buf_size);

bool Send(const char* Operation, const char* To, const char* Value);

// наследство от IBaseObject
void SetPtrParam(const char* param_name, void *param_value);

private:

//IEngine *Engine;

std::string cut_buffer; //остатки от прежнего буфера
std::string ExtLogName; //имя лога сервера
bool connected;

//int TryConnect;
//void *CustomHandler;

};

#endif