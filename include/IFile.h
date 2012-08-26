#ifndef _I_FILE_
#define _I_FILE_

#include <string>
// ��������� �����

enum asn_File_State{
ASN_FILE_NONE, //��������� ���
ASN_FILE_NOT_FOUND, //���� �� ������
ASN_FILE_DISK_OPEN, //���� ������ �� �����
ASN_FILE_DISK_IN_MEM, //���� �������� � ������ � ������ �� �����
ASN_FILE_WAIT, //���� ������� ��������
ASN_FILE_LOADING, //���� � �������� ��������
ASN_FILE_LOADING_END, //���� �������� �� �� ������
};

class IFile{
public:
virtual void Release()=0;
virtual const char* GetName(){return FileName.c_str();} //����� �� �������� ������� �������� ��� �����
// ������� ���� (�������� ��������, ��� ����� ;)
virtual asn_File_State FileOpen()=0;
// ������� ���� (�� �� ���������� ���)
virtual void FileClose()=0;
// ��������� ���� � ������, �� ������ - ��������� �� �������
virtual void *LoadInMem()=0;
// ���������� ������
virtual void FreeMem()=0;
// ����������� �� �������
virtual void Seek(unsigned int Position, int From)=0;
// ��������� ����� ����� � �����
virtual void ReadBuf(void *buf, unsigned int size)=0;
// �������� ������ ����� � ������;
virtual unsigned int GetSizeInBytes(){ return SizeInBytes; }
//��������� �����
asn_File_State GetState(){ return state; };
protected:
asn_File_State state; //��������� �����
std::string FileName; //��� �����
unsigned int SizeInBytes; //��������� �� ������ �����
unsigned int CurPos; //��������� �� ������� ������� �����
};

#endif
