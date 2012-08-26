#ifndef _NH_HTTP_CL_H
#define _NH_HTTP_CL_H

#include "INetProtocolHandler.h"
#include "CBaseObject.h"
#include "IEngine.h"

class HTTP_Client: public INetProtocolHandler, public CBaseObject {
public:
HTTP_Client(IEngine *_Engine);
~HTTP_Client();

int Release();

void Init(const char* Init_String);

unsigned int HandleMessage(unsigned int handler_id, char *buffer, long int buf_size);

bool Send(const char* Operation, const char* To, const char* Value);

// наследство от IBaseObject
void SetPtrParam(const char* param_name, void *param_value);

protected:

    void Parse_HTTP_Header(char* ptr);
    void ParseParams(const std::string& InpString);

    void *CustomHandler;
    int filesize;    
    bool header_on;

};

#endif

