//[497] Человек очень силен, когда довольствуется тем, что он есть, и очень слаб, когда хочет подняться выше человечества.
//Ж.-Ж. Руссо
#ifndef _IFILESOURCE_H
#define _IFILESOURCE_H

#include "IBaseObject.h"

class IFile;

class IFileSource: public virtual IBaseObject {
public:
virtual bool isRemote()=0; // remote network source - true
virtual int Release()=0;
virtual IFile *LoadFile(const std::string& FileName)=0;
virtual void AddPath(const std::string& PathName, bool Recursive)=0;
};
#endif
