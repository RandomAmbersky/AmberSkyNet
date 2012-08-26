#include "CLog.h"
#include "ASN_utils.h"

#ifdef _WIN32
#include "windows.h"
#include "time.h"
#endif

static char buffer[ 80 ];

const char *get_time(){
//char *buf_ptr=buffer;
time_t time_of_day=time(NULL);
strftime( buffer, 80, "%H:%M:%S", localtime( &time_of_day ) );
return buffer;
};

CLog::CLog(IEngine *_Engine):CBaseObject(_Engine){

Engine=_Engine;

/// запоминаем уровень показа ошибок
ErrLevel=StrToInt(ASN_Str(Engine->GetEnvParam("[LogManager]ErrorLevel")));
/// запоминаем надо ли выводить время в лог
ShowTime=(ASN_Str(Engine->GetEnvParam("[LogManager]ShowTime"))=="1");

char *buf_ptr=buffer;

LogName=Engine->GetEnvParam("[LogManager]LogName");

if (LogName==ASN_NOT_FOUND_PARAM) {
    LogName="log.txt";
    Engine->SetEnvParam("[LogManager]LogName",LogName.c_str());
}

time_t time_of_day=time(NULL);
strftime( buffer, 80, LogName.c_str(), localtime( &time_of_day ) );

LogName=std::string(buf_ptr);

// устанавливаем имя лога
//Engine->SetEnvParam("[LogManager]LogName",LogName.c_str());

// пытаемся открыть файл с таким именем
pLogFile = fopen((ASN_Str(Engine->GetEnvParam("[Paths]LogPath"))+LogName).c_str(),"w+b");

if (!pLogFile) {printf("Can't init LogFile!\n");}
//else fclose(pLogFile);

//pErrFile = fopen((Engine->GetEnvParam("[Paths]LogPath")+"error.log").c_str(),"a+b");
//if (!pErrFile) {printf("Can't init ErrFile!\n");}

    SetParam("BaseClass","Log");
    SetParam("Type","LogTxt");

LogMsg("Ok, i'm starting...");

};

CLog::~CLog(){
 LogMsg("Ok, i'm ending...");
 fclose(pLogFile);
};

void CLog::InitManager(){
 printf("CLog init...\n");
}

int CLog::Release(){
 printf("CLog released...\n");
 delete this;
 return 0;
}

void CLog::Update(float tms){

};

void CLog::LogMsg(const char* Msg){

if (ShowTime) {
fputs(get_time(),pLogFile);
fputs(":",pLogFile); }

fputs(Msg,pLogFile);
fputs(ASN_END_OF_LINE,pLogFile);

fflush(pLogFile);
printf("CLog::LogMsg << %s\n", Msg );
};

void CLog::ExtLogMsg(const char* LogName, const char* Msg){

FILE *tmp=fopen((ASN_Str(Engine->GetEnvParam("[Paths]LogPath"))+LogName).c_str(),"a+b");

if (ShowTime) {
fputs(get_time(),tmp);
fputs(":",tmp); }

fputs(Msg,tmp);
fputs(ASN_END_OF_LINE,tmp);
fclose(tmp);

};

/*
void CLog::VerbLogMsg(const char *SourceName, const int Line,const std::string& Msg){
LogMsg(std::string(SourceName)+" in line "+IntToStr(Line)+ ": "+Msg);
};
*/

void CLog::ErrMsg(int level,const char* Msg){

if (level>ErrLevel) return; // ошибка незначительна, ничего выводить не надо
ExtLogMsg("error.log",ASN_Str(IntToStr(level)+":"+ASN_Str(Msg)).c_str());

#ifdef _WIN32
if (ASN_Str(Engine->GetEnvParam("[LogManager]ShowErrorWindow"))=="1")
MessageBox(0, Msg, "Fatal error:", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif

};

//void CLog::VerbErrMsg(const char *SourceName, const int Line,const int level,const std::string& Msg){

//if (level<ErrLevel) return; // ошибка незначительна, ничего выводить не надо

//ExtLogMsg("error.log",
//std::string(SourceName)+" in line "+IntToStr(Line)+":"+IntToStr(level)+":"+Msg);

//};

