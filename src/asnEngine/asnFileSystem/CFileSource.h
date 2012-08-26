#ifndef _CFILESOURCE_H
#define _CFILESOURCE_H

#include "IFileSource.h"
#include "CBaseObject.h"
#include "IEngine.h"

#include <string>
#include <vector>

class CFileSource: public IFileSource, public CBaseObject{
public:
CFileSource(IEngine *_Engine);
~CFileSource();
int Release();
bool isRemote(){ return false; }
IFile *LoadFile(const std::string& FileName);
void AddPath(const std::string& PathName, bool Recursive);
protected:
//IEngine *Engine;
std::vector<std::string> Paths;
};
#endif
