#ifndef _NH_IRC_H
#define _NH_IRC_H

#include "INetProtocolHandler.h"
#include "CBaseObject.h"
#include "IEngine.h"

class NH_IRC: public INetProtocolHandler, public CBaseObject{
public:
NH_IRC(IEngine *_Engine);
~NH_IRC();
void Init(const char* Init_String);
unsigned int HandleMessage(unsigned int handler_id, char *buffer, long int buf_size);

bool Send(const char* Operation, const char* To, const char* Value);

//void SendMsg(const char *To, const char *Message);
//void SendMessage(std::string To, std::string Message);

// наследство от IBaseObject
void SetPtrParam(const char* param_name, void *param_value);

private:

IEngine *Engine;

std::string cut_buffer; //остатки от прежнего буфера
std::string ExtLogName; //имя лога сервера
//std::string UpdateScript;
int NumTryConnect;

void *CustomHandler;

//unsigned int SupervisorId;
};

class NH_IRC_DCC_CHAT: public INetProtocolHandler, public CBaseObject{
public:
NH_IRC_DCC_CHAT(IEngine *_Engine);
~NH_IRC_DCC_CHAT();
void Init(const char* Init_String);
unsigned int HandleMessage(unsigned int handler_id, char *buffer, long int buf_size);

bool Send(const char* Operation, const char* To, const char* Value);

// наследство от IBaseObject
void SetPtrParam(const char* param_name, void *param_value);

private:
IEngine *Engine;
//unsigned int SupervisorId;
void *CustomHandler;
};

class NH_IRC_DCC_GETFILE: public INetProtocolHandler, public CBaseObject{
public:
 NH_IRC_DCC_GETFILE(IEngine *_Engine);
~ NH_IRC_DCC_GETFILE();
void Init(const char* Init_String);
unsigned int HandleMessage(unsigned int handler_id, char *buffer, long int buf_size);

bool Send(const char* Operation, const char* To, const char* Value);

void SetPtrParam(const char* param_name, void *param_value);

private:
IEngine *Engine;
//unsigned int SupervisorId;
void *CustomHandler;
};

class NH_IRC_DCC_SENDFILE: public INetProtocolHandler, public CBaseObject{
public:
NH_IRC_DCC_SENDFILE(IEngine *_Engine);
~NH_IRC_DCC_SENDFILE();
void Init(const char* Init_String);
unsigned int HandleMessage(unsigned int handler_id, char *buffer, long int buf_size);

bool Send(const char* Operation, const char* To, const char* Value);

void SetPtrParam(const char* param_name, void *param_value);

private:

char spec[4]; // число принятых пакетов в ненормальном формате :)

IEngine *Engine;
//unsigned int SupervisorId;
void *CustomHandler;
};


#endif

