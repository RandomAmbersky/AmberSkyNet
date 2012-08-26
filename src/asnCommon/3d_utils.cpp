#include <math.h>
#include "ASN_Types.h"

//// определ€ем по координатам текстурные координаты - как бы нат€гиваем шарик с текстурой на объект :-)
void xyz_to_uv_sphere( float uv[2], const float vertex[3], const float normal[3] )
{
const float radius = (float) sqrt( vertex[0]*vertex[0] + vertex[1]*vertex[1] + vertex[2]*vertex[2] );
double longitude = atan2( -vertex[0], vertex[2] );
double latitude = atan( vertex[1] / sqrt( vertex[0] * vertex[0] + vertex[2] * vertex[2] ) );
longitude = 1.0 - longitude / ( 2 * PI );
latitude = latitude / PI + 0.5;

uv[0] = longitude - floor( longitude );
uv[1] = latitude;

if( vertex[0] == 0.0f && normal[0] < 0.0 && normal[2] > 0 ) uv[0] = 1.0f; // *** NEW - test normal ***
}
