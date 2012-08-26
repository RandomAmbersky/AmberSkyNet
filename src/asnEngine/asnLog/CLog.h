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
std::string LogName; //��� �������� ������������ ����  ������� YmdHMS
FILE *pLogFile; //��������� �� ������� ����������� ���
//FILE *pErrFile; //��������� �� ������� ��� ������
int ErrLevel; // ������� ������, ���� �������� �� ��������
bool ShowTime; //���������� �� ����� � ����
};

/// �������� ��� ����. ������� � ������ ������� ������ � CD-ROM, ��������...
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
