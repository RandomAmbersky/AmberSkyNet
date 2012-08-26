#ifndef _I_LOG_
#define _I_LOG_

#include "IManager.h"
// интерфейс к классу лога
class ILog: public IManager {
public:
//ILog(){};
//virtual ~ILog(){};
// обычное сообщение кидаем в стандартный файл
virtual void LogMsg(const char* Msg)=0;

// спец. лог с указанием имени лог-файла
virtual void ExtLogMsg(const char* LogName, const char* Msg)=0;

// обычное сообщение об ошибке
virtual void ErrMsg(int level,const char* Msg)=0;

};

#define ErrMsgStr(X,Y) ErrMsg(X, std::string(Y).c_str() );
#define LogMsgStr(X) LogMsg( std::string(X).c_str() );

#define LOGGER ((ILog*)ENGINE->GetPtrParam("LogManager"))

#define VerbLogMsg(X) LogMsg(std::string(__FILE__)+" in line "+IntToStr(__LINE__)+ ": "+X);
#define VerbErrMsg(X,Y) ErrMsg(std::string(__FILE__)+" in line "+IntToStr(__LINE__)+ ": ",X,Y);

// если надо писать в лог-файле имя исходника и строчку,
// в которой вызывается сообщение - закомментируйте строчку ниже
#define SIMPLE_LOG 1

#ifdef SIMPLE_LOG
#define LOG(X) LogMsg(X)
#define ERR(X,Y) ErrMsg(X,Y)
#else
#define LOG(X) VerbLogMsg(X)
#define ERR(X,Y) VerbErrMsg(X,Y)
#endif

#define LogError(X,Y) LOGGER->ErrMsg(X,Y)

#endif
