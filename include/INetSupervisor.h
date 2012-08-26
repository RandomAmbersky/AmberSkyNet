#ifndef _INetSupervisor_H
#define _INetSupervisor_H

#include "INetProtocolHandler.h"

#include "IManager.h"

#include <string>

class INetSupervisor: public IManager {
public:

virtual ~INetSupervisor(){}

virtual INetProtocolHandler *OpenServerListener(const char* _HandlerName, const char* _Protocol, int port, const char* Init_string)=0;
virtual INetProtocolHandler *OpenClientListener(const char* _HandlerName, const char* _Protocol, const char* Server_Name, int port, const char* Init_string)=0;
virtual INetProtocolHandler *FindHandler(const char* _HandlerName)=0;

virtual void CloseListener(const char* _HandlerName)=0;

virtual bool AddCustomListener(INetProtocolHandler* handler, const char* Server_Name, int port)=0;

virtual void Check()=0;

// интерфейс для Handler'ов

// для серверных сокетов
// true - не удалось принять
virtual bool AcceptSocket(unsigned int Open_Socket_Id,unsigned int handler_id)=0;
virtual int SendMsg(unsigned int Open_Socket_Id, char *buffer, int buf_size)=0;
virtual void CloseListener(int num)=0;

};

#define NETTER ((INetSupervisor*)ENGINE->GetPtrParam("NetManager"))

#endif
