#ifndef _CFILESOURCEHTTP_H
#define _CFILESOURCEHTTP_H

#include "IFileSource.h"
#include "IEngine.h"

#include "CBaseObject.h"

#include <string>
#include <vector>
#include <map>

class CFileSourceHTTP: public IFileSource, public CBaseObject{
public:
CFileSourceHTTP(IEngine *_Engine);
~CFileSourceHTTP();
int Release();
bool isRemote(){ return true; }
IFile *LoadFile( const std::string& FileName);
void AddPath( const std::string& PathName, bool Recursive);
protected:
//IEngine *Engine;
std::vector<std::string> Paths; // заданные в AddPath пути
};
#endif
