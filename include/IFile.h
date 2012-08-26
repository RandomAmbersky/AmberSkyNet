#ifndef _I_FILE_
#define _I_FILE_

#include <string>
// состояния файла

enum asn_File_State{
ASN_FILE_NONE, //состояния нет
ASN_FILE_NOT_FOUND, //файл не найден
ASN_FILE_DISK_OPEN, //файл открыт на диске
ASN_FILE_DISK_IN_MEM, //файл загружен в память и закрыт на диске
ASN_FILE_WAIT, //файл ожидает загрузки
ASN_FILE_LOADING, //файл в процессе загрузки
ASN_FILE_LOADING_END, //файл загружен но не открыт
};

class IFile{
public:
virtual void Release()=0;
virtual const char* GetName(){return FileName.c_str();} //чтобы не возникло желания изменить имя файла
// открыть файл (источник известен, имя знаем ;)
virtual asn_File_State FileOpen()=0;
// закрыть файл (но не уничтожить его)
virtual void FileClose()=0;
// загрузить файл в память, на выходе - указатель на область
virtual void *LoadInMem()=0;
// освободить память
virtual void FreeMem()=0;
// передвинуть на позицию
virtual void Seek(unsigned int Position, int From)=0;
// прочитать часть файла в буфер
virtual void ReadBuf(void *buf, unsigned int size)=0;
// получить размер файла в байтах;
virtual unsigned int GetSizeInBytes(){ return SizeInBytes; }
//состояние файла
asn_File_State GetState(){ return state; };
protected:
asn_File_State state; //состояние файла
std::string FileName; //имя файла
unsigned int SizeInBytes; //указатель на размер файла
unsigned int CurPos; //указатель на текущую позицию файла
};

#endif
