#ifndef C_TEXTLIST_H
#define C_TEXTLIST_H

#include "IEngine.h"

#include "IResourceSystem.h"

#include <string>
#include <list>

typedef std::vector<std::string> asn_textlist;


class CTextlist{
public:
CTextlist(std::string File_Name="", int size=30);
~CTextlist();
virtual void Add(std::string Line);
virtual asn_textlist *GetList(int start=0, int num=10);
virtual const char* GetIncString();
virtual const char* GetDecString();
virtual bool IsFirst();
virtual bool IsEnd();
virtual void End();
virtual void Begin();
//virtual int GetSize();
private:
std::string FileName;
int Size;
int CurPos;
asn_textlist::iterator it;
asn_textlist TextList;
asn_textlist Cache;
};

class CTextlistMap:public ITextListMap {
public:
CTextlistMap();
~CTextlistMap();

virtual void GetList(const char* MapName, asn_ext_chars_list* list, int start=0, int num=30);
virtual void Add(const char* MapName, const char* Line);
virtual void Begin(const char* MapName);
virtual void End(const char* MapName);
virtual const char* GetIncString(const char* MapName);
virtual const char* GetDecString(const char* MapName);

virtual CTextlist *GetTextList(std::string MapName);
IEngine *Engine;
private:
std::map<std::string,CTextlist*> ListMap;
};

#endif
