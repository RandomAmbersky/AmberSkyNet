#include "ASN_utils.h"

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ASN_Types.h"

std::string GetFileExt(const std::string& FilePathNameExt)
{
    std::string FileExt="";
    size_t i=FilePathNameExt.rfind('.');
    if (i!=-1)
        FileExt=FilePathNameExt.substr(i+1,FilePathNameExt.length()-i);
    return FileExt;
}

/// получить протокол
std::string GetProtocolStr(const std::string& FilePathNameExt){
    std::string Protocol="";
    size_t i=FilePathNameExt.find("://");
    if (i!=-1)
        Protocol=FilePathNameExt.substr(0,i);
    return Protocol;
}

extern bool ParseNetAdressStr(const std::string& FilePathNameExt,
    std::string& Protocol,
    std::string& Username,
    std::string& Password,
    std::string& Server,
    std::string& Port,
    std::string& Path){

// [protocol]://{[username]:[password]}@[server]:[port]/[path]

    std::string Current = FilePathNameExt;
    Protocol = "";
    Username = "";
    Password = "";
    Server = "";
    Port = "";
    Path = "";

//printf("try parse protocol...\n");

//  сначала - протокол
    size_t i=Current.find("://");
    if (i!=-1) {
        Protocol=Current.substr(0,i);
//	printf("protocol: %s\n", Protocol.c_str() );
	Current=Current.substr(i+3, Current.length()-i);
        }

//printf("try parse username...\n");

// username:password@
    i=Current.find('@');
    if (i!=-1) {
        Username=Current.substr(0,i);
	Current=Current.substr(i+1, Current.length()-i);
	i=Username.find(':');
	if (i!=-1) { Password=Username.substr(i+1, Username.length()-i); Username=Username.substr(0,i); }
	}

//printf("try parse server...\n");

// server:port
    i=Current.find('/');
    if (i!=-1) {
        Server=Current.substr(0,i);
	Current=Current.substr(i+1, Current.length()-i);
	i=Server.find(':');
	if (i!=-1) { Password=Username.substr(i+1, Username.length()-i); Username=Username.substr(0,i); }
	}

    Path = Current;
//    printf("ok..\n");

    return true;

}

std::string GetFileName(const std::string& FilePathNameExt, bool addExt)
{
    std::string FileName="";
    size_t i=FilePathNameExt.rfind('/');
    if (i!=-1)
        FileName=FilePathNameExt.substr(i+1,FilePathNameExt.length()-i);
    i=FilePathNameExt.rfind('\\');
    if (i!=-1)
        FileName=FilePathNameExt.substr(i+1,FilePathNameExt.length()-i);

    if ( !addExt )
    { //расширение тоже удаляем
        size_t i=FileName.rfind('.');
        if (i!=-1)
            FileName=FileName.substr(0,i);
    };

    return FileName;
}

std::string Trim(std::string& s,const std::string& drop)
{
    //printf("TRY TRIM {%s}\n",s.c_str());
    std::string t=s;
    size_t i=t.find(0xd);
    if (i>0)
        t=t.substr(0,i);
    std::string::size_type posLast=t.find_last_not_of(drop)+1;
    std::string::size_type posFirst=t.find_first_not_of(drop);
    return std::string(t, posFirst, posLast-posFirst);
}

std::string IntToStr(int Param)
{
    char buf[64];
    sprintf(buf,"%d",Param);
    return std::string (buf);
}

std::string FloatToStr(const float Param)
{
    char buf[80];
    sprintf(buf,"%f",Param);
    //gcvt(Param,5,buf);
    return std::string (buf);
}

CVector StrToVector(const std::string& Param)
{
    float a, b,c;

//    try
//{
    if (sscanf(Param.c_str(),"%f %f %f",&a,&b,&c)==3)
//            throw "StrToVector error!";
        return CVector(a,b,c);
//}
//    catch( const char *msg ) {
//        printf("THROW!: %s\n");
//        throw;
        return CVector(0,0,0);
//    }

}

CVector4 StrToVector4(const std::string& Param)
{
    float a, b,c, d;

//    try
//{
    if (sscanf(Param.c_str(),"%f %f %f %f",&a,&b,&c, &d)==4)
//            throw "StrToVector error!";
        return CVector4(a,b,c,d);
//}
//    catch( const char *msg ) {
//        printf("THROW!: %s\n");
//        throw;
        return CVector4(0,0,0,0);
//    }

}

std::string VectorToStr(const CVector& vector)
{
    std::string ret=FloatToStr( vector.v[0] )+" "+FloatToStr( vector.v[1] )+" "+FloatToStr( vector.v[2] );
    return ret;
}

float GetAngles(float X, float Y)
{

    float len = sqrt( X*X + Y*Y );
    X*=len;
    Y*=len;

    if (Y == 0.0)
        return X > 0.0 ? 90.0 : 180.0;

    float tmp = Y / sqrt(X*X + Y*Y);
    tmp = atan(sqrt(1 - tmp*tmp) / tmp) * GRAD_PI;

    if (X>0.0 && Y>0.0)
        return tmp + 270;
    else
        if (X>0.0 && Y<0.0)
            return tmp + 90;
        else
            if (X<0.0 && Y<0.0)
                return 90 - tmp;
            else
                if (X<0.0 && Y>0.0)
                    return 270 - tmp;

    return tmp;
}

std::string ListToStr(asn_List& list)
{
    std::string Ret="";

    asn_List::iterator it=list.begin();
    for( UINT i=0; i<list.size(); ++i)
    {
        Ret=Ret+(*it).first+"="+(*it).second+";";
        it++;
    };
    return Ret;
}

void StrToList(const std::string& Str, asn_List& list)
{

    //list.clear();

    std::string Param;
    std::string Value;

    std::string Parse_String=Str;
    std::string Parse_String_Part;
    int pos=0;
    int pos2=0;

    while(pos!=-1)
    {
        //      printf("Parse string: %s\n",Parse_String.c_str());
        pos=Parse_String.find_first_of(';',0);
        Parse_String_Part=Parse_String.substr(0,pos);
        //      printf("Parse string part: %s\n",Parse_String_Part.c_str());
        pos2=Parse_String_Part.find_first_of('=',0);
        if (pos2!=0)
        {
            Param=Parse_String_Part.substr(0,pos2);
            Value=Parse_String_Part.substr(pos2+1,Parse_String_Part.size());
            //          printf("Initialise: >%s< = %s\n",Param.c_str(), Vale.c_str());
            if (Param!="")
            {
                list[Param]=Value;
                //            this->SetParam(Param, Value);
            };
        };
        //         printf("pos %d pos2 %d\n",pos,pos2);
        Parse_String=Parse_String.substr(pos+1);
    };
    //printf("ggod bye!\n");
}

std::string StrLow(const std::string& Param)
{
    std::string temp=Param;

    return temp;
}

std::string IntToIP(unsigned int intIP)
{

    //        printf("Start IP: %d\n",intIP);
    unsigned int tmp=intIP/256/256/256;
    std::string inetIP=IntToStr( tmp );
    //        printf("%d Inet IP: %s\n",tmp,inetIP.c_str());
    intIP-=tmp*256*256*256;

    tmp=intIP/256/256;
    inetIP=inetIP+"."+IntToStr( tmp );
    intIP-=tmp*256*256;

    tmp=intIP/256;
    inetIP=inetIP+"."+IntToStr( tmp );
    intIP-=tmp*256;

    tmp=intIP;
    inetIP=inetIP+"."+IntToStr( tmp );

    return inetIP;
}

void StrPtrToList(const asn_ext_param_list* ParamList, asn_List& list)
{

    if (!ParamList) return;
    list.clear();

    for (UINT i=0;i<ParamList->nums;i++){
        list[ParamList->param_list[i].name]=ParamList->param_list[i].value;
    };

}

bool CheckStrToVector(const std::string& Param){
    float a, b,c;
    if (sscanf(Param.c_str(),"%f %f %f",&a,&b,&c)==3)
    return true;
    else return false;
}

bool CheckStrToInt(const std::string& Param){
    int a;
    if (sscanf(Param.c_str(),"%d",&a)==1)
    return true;
    else return false;
}

bool CheckStrToFloat(const std::string& Param){
    float a;
    if (sscanf(Param.c_str(),"%f",&a)==1)
    return true;
    else return false;
}

/*
bool CharToParamValue(char* buffer, const char* delimeter, char* Param, char* Value){

	Value = NULL;

        Param = strtok( buffer, delimeter); // берём первый токен до знака

        if(Param == NULL) return false;

        Value = strtok(NULL, delimeter); // читаем что там дальше, надеюсь знака больше не будет

	if (Value == NULL) return false;

	return true;

};
*/

bool StrToParamValue(const std::string& InputString, const char Delimeter, std::string& Param, std::string& Value){

    int pos=InputString.find_first_of(Delimeter,0);

    if (pos==-1) return false;

    Param=InputString.substr(0,pos);
    Value=InputString.substr(pos+1,InputString.size());

    return true;

}
