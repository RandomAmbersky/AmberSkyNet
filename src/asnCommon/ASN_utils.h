// ������� ��� ������ � ������
#ifndef _ASN_UTILS_H
#define _ASN_UTILS_H

#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "CVector.h"
#include "ASN_Types.h"
#include "ASN_Types_internal.h"

#include "ASN_Params.h"

/// �������� ���������� �����
extern std::string GetFileExt(const std::string& FilePathNameExt);
/// �������� ��� �����, addExt - � �����������
extern std::string GetFileName(const std::string& FilePathNameExt, bool addExt=false);
/// ������� ������� � ������ ������ � � �����
extern std::string Trim(std::string& s,const std::string& drop = " ");
/// �������� ��������
extern std::string GetProtocolStr(const std::string& FilePathNameExt);

// ��������� ������ �������, bool - ������������ ������
extern bool ParseNetAdressStr(const std::string& FilePathNameExt,
    std::string& Protocol,
    std::string& Username,
    std::string& Password,
    std::string& Server,
    std::string& Port,
    std::string& Path);

extern std::string FloatToStr(const float Param);

extern std::string StrLow(const std::string& Param);

inline int StrToInt(const std::string& Param){
    int a;
    if (sscanf(Param.c_str(),"%d",&a)==1)
    return a;
    else return 0;
    //return atoi(Param.c_str());
}

inline float StrToFloat(const std::string& Param){
    float a;
    if (sscanf(Param.c_str(),"%f",&a)==1)
    return a;
    else return 0.0f;
//return (float)atof(Param.c_str());
}

inline float GetMin(float a, float b){
if (a>b) return b;
return a;
}

inline float GetMax(float a, float b){
if (a>b) return a;
return b;
}

extern std::string IntToIP(unsigned int intIP);

// false - �� ������� ������������� � ��������:��������, true - ����������
//extern void CharToParamValue(const char* buffer, const char *delimeter, char* Param, char* Value);
extern bool StrToParamValue(const std::string& InputString, const char Delimeter, std::string& Param, std::string& Value);

extern CVector StrToVector(const std::string& Param);
extern CVector4 StrToVector4(const std::string& Param);
extern std::string VectorToStr(const CVector& vector);

extern std::string IntToStr(int Param);

extern float GetAngles(float X, float Y);

extern std::string ListToStr(asn_List& list);
extern void StrToList(const std::string& Str, asn_List& list);
extern void StrPtrToList(const asn_ext_param_list* ParamList, asn_List& list);

extern bool CheckStrToVector(const std::string& Param);
extern bool CheckStrToInt(const std::string& Param);
extern bool CheckStrToFloat(const std::string& Param);

#endif
