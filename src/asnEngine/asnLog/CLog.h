#ifndef _C_LOG_
#define _C_LOG_

#include "IEngine.h"
#include "ILog.h"
#include "CBaseObject.h"
#include <stdio.h>

class CLog: public ILog, public CBaseObject {
public:
CLog(IEngine *_Engine);
~CLog();

void InitManager();
int Release();
void Update(float tms);

void LogMsg(const char* Msg);

void ExtLogMsg(const char* LogName, const char* Msg);

//void VerbLogMsg(const char *SourceName, const int Line,const std::string& Msg);

void ErrMsg(int level,const char* Msg);

//void VerbErrMsg(const char *SourceName, const int Line,const int level,const std::string& Msg);

private:
//IEngine *Engine;
std::string LogName; //имя текущего стандартного лога  формата YmdHMS
FILE *pLogFile; //указатель на текущий стандартный лог
//FILE *pErrFile; //указатель на текущий лог ошибок
int ErrLevel; // уровень ошибок, выше которого не выводить
bool ShowTime; //показывать ли время в логе
};

/// заглушка для лога. Полезна в случае запуска движка с CD-ROM, например...
class CLogNo: public ILog, public CBaseObject {
public:
CLogNo(IEngine *_Engine):CBaseObject(_Engine){};
~CLogNo(){};

void InitManager(){};
int Release(){ delete this; return 0;}
void Update(float tms){};

void LogMsg(const char* Msg){};

void ExtLogMsg(const char* LogName, const char* Msg){};

//void VerbLogMsg(const char *SourceName, const int Line,const std::string& Msg){};

void ErrMsg(int level,const char* Msg){};

//void VerbErrMsg(const char *SourceName, const int Line,const int level,const std::string& Msg){};

};

#endif
