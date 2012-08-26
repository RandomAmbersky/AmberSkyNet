// типы и константы, определяемые в AmberSkyNet

#ifndef _ASN_TYPES_H
#define _ASN_TYPES_H

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef UINT16
typedef unsigned int UINT16;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

// universal id
typedef unsigned int UID;

#include <string>
#include <map>

#include <math.h>

#define ASN_NOT_FOUND_PARAM "N/A" // Строка, возвращаемая в случае если параметр не найден

#ifndef _WIN32
#define ASN_MODULE_EXTENSION_NAME "so" // расширения модуля загрузки для Win и Lin
#define ASN_END_OF_LINE "\n"           // конец строки
#else
#define ASN_MODULE_EXTENSION_NAME "dll"
#define ASN_END_OF_LINE "\r\n"
#endif

#define PI						3.14159265358979323846f
#define M_E	                    2.7182818284590452354
#define M_LOG2E	            	1.4426950408889634074
#define M_LOG10E            	0.43429448190325182765
#define M_LN2	              	0.69314718055994530942
#define M_LN10	             	2.30258509299404568402
#define M_PI            		3.14159265358979323846
#define M_PI_2	             	1.57079632679489661923
#define M_PI_4	             	0.78539816339744830962
#define M_1_PI	             	0.31830988618379067154
#define M_2_PI          		0.63661977236758134308
#define M_2_SQRTPI	            1.12837916709551257390
#define M_SQRT2	            	1.41421356237309504880
#define M_SQRT1_2           	0.70710678118654752440
#define GRAD_PI		            180.0 / 3.1415926535897932384626433832795
#define GRAD_PI2		        3.1415926535897932384626433832795 / 180.0

//const std::string ASN_NOT_FOUND_PARAM_STR=std::string(ASN_NOT_FOUND_PARAM);

#ifndef NAN
#define NAN 0.0f/0.0f
#endif

#define ASN_Str(X) std::string(X)

typedef int (*CALLBACK_ASN_FUNCTION)(void*);

#endif
