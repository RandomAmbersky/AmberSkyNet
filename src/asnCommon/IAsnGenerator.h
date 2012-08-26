// [1258] � ������� � ������� ������� ��������� ����� ������ ��� ����,
// ����� ������� ��������� ����������.
//  �. ������

#ifndef _I_ASN_GENERATOR_PLUGIN_H
#define _I_ASN_GENERATOR_PLUGIN_H

#include "IEngine.h"

struct IAsnGeneratorInfo
{
	const char* name;         //��� ������������ ������
        const char* type;             //�������������� ���� ��������� ����������� �������
        const char* author;           //����� �������
        const char* description;      //�������� �������
        const char* plugin_version;   //������ �������
        const char* required_version; //��������� ������ ������, �� ����   //TODO

};

struct  IAsnGenerator
{
public:
//    virtual IAsnGenetaror() {Engine=NULL;};
//    virtual ~IAsnGenerator() { };

// ���������� ��������� ������ �� ���� � ������ Type
// virtual void *GetClass(const char* Type)=0;

    virtual IBaseObject* GetObject(const char* Type)=0;

// ������� ��������� �� Engine
    virtual void Initialize(IEngine *vEngine) {Engine=vEngine;}
// �������� ���������� � �������
    virtual IAsnGeneratorInfo GetInfo() {return GeneratorInfo;}

protected:
// ���������� � �������. ����������� � ������������ ������
IAsnGeneratorInfo GeneratorInfo;
// ��������� �� ������� �����, ���������� � �������. ����� ��� ��������� �����������
//�������, ������� ��� �������� ��������� ��������� �� Engine.
IEngine *Engine;
};

#endif
