#ifndef _CNetSupervisor_H
#define _CNetSupervisor_H
#include "SDL/SDL_net.h"

#include "IEngine.h"

#include "INetSupervisor.h"
#include "CBaseObject.h"

#include <string>
#include <map>

struct COpenSocket{
bool ServerSocket;
TCPsocket socket;
INetProtocolHandler *handler;
unsigned int handler_id;
//unsigned int handler_count;
};

class CNetSupervisor: public INetSupervisor, public CBaseObject {
public:
CNetSupervisor(IEngine *_Engine);
~CNetSupervisor();

void InitManager();
int Release();
void Update(float tms);

INetProtocolHandler *OpenServerListener(const char* _HandlerName, const char* _Protocol, int port, const char* Init_string);
INetProtocolHandler *OpenClientListener(const char* _HandlerName, const char* _Protocol, const char* Server_Name, int port, const char* Init_string);
INetProtocolHandler *FindHandler(const char* _HandlerName);
int SendMsg(const char* _HandlerName, const char* To, const char* Message);
void CloseListener(const char* _HandlerName);
void Check();

bool AddCustomListener(INetProtocolHandler* handler, const char* Server_Name, int port);

// интерфейс для Handler'ов
bool AcceptSocket(unsigned int Open_Socket_Id,unsigned int handler_id);
int SendMsg(unsigned int Open_Socket_Id, char *buffer, int buf_size);
void CloseListener(int Open_Socket_Id);

void SetParam(const char* param_name, const char* param_value);

private:

void AddConnection(TCPsocket in_socket,INetProtocolHandler *handler, bool Server);
void ReCreateSockSet();

int GetMsg(TCPsocket socket, char *buffer, int buf_size);
int PutMsg(TCPsocket socket, char *buffer, int buf_size);

std::map<std::string,INetProtocolHandler *> HandlerPack;

//IEngine *Engine;

COpenSocket *OpenSocketPack;

TCPsocket current_socket;

int num_open_socket;
int BufferSize;
SDLNet_SocketSet set;

char* message;
};

#endif
