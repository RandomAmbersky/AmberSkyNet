#ifndef _CFILESOURCEZIP_H
#define _CFILESOURCEZIP_H

#include "IFileSource.h"
#include "IEngine.h"
#include "CBaseObject.h"

#include <string>
#include <vector>
#include <map>

//struct ZipFileRecord{
//std::string ZipFilePackName,
//std::string ZipFileName,
//};

class CFileSourceZIP: public IFileSource, public CBaseObject{
public:
CFileSourceZIP(IEngine *_Engine);
~CFileSourceZIP();
int Release();
bool isRemote(){ return false; }
IFile *LoadFile(const std::string& FileName);
void AddPath( const std::string& PathName, bool Recursive);
protected:
//IEngine *Engine;
std::vector<std::string> Paths; // заданные в AddPath пути
//std::vector<std::string> ZipFilePaths;
std::map<std::string,std::string> ZipFileSystem; //имена файлов считанные из архивов, имя архива
};
#endif
