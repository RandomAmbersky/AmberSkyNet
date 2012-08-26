#ifndef _C_PLUGINFACTORY_H
#define _C_PLUGINFACTORY_H

#include "IAsnGenerator.h"
//#include "IPluginFactory.h"
#include "IEngine.h"

#include <map>

class CPluginFactory {
public:
CPluginFactory(IEngine *Engine);
~CPluginFactory();

// ��������� ���������� � ��������� �������-����������� � ��������� ��
void ScanDirectory(std::string DirName, bool Recursive);

// ��������� ������ ������-���������� IAsnGenerator, true - ������ ��� ��������
bool LoadASNModule(const std::string& ModuleName);

// ��������� ������ �������
//bool UnloadModule(const std::string& ModuleName);
bool LoadGenerator(IAsnGenerator*  myGenerator);

// ������� ����� � ����� Type �� ������ ���� ��������� �������-�����������
//void *CreateType(const std::string& Type);

IBaseObject* CreateObject(const char* Type);

private:

UID next_uid;

IEngine *Engine;
std::map<std::string,void*> LibraryList;
std::map<std::string,IAsnGenerator*> GeneratorList;

};

#endif
