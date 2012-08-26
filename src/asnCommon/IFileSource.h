//[497] ������� ����� �����, ����� �������������� ���, ��� �� ����, � ����� ����, ����� ����� ��������� ���� ������������.
//�.-�. �����
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
