#ifndef _ISCENE_OBJECT_H
#define _ISCENE_OBJECT_H

#include "IMaterial.h"

//ASN_DRAW_LINES=0,
//ASN_DRAW_QUADS,
//ASN_DRAW_TRIANGLES,

enum ASN_DRAW_Types{
ASN_DRAW_POINTS=0,
ASN_DRAW_LINE_STRIP,
ASN_DRAW_LINE_LOOP,
ASN_DRAW_LINES,
ASN_DRAW_TRIANGLE_STRIP,
ASN_DRAW_TRIANGLE_FAN,
ASN_DRAW_TRIANGLES,
ASN_DRAW_QUAD_STRIP,
ASN_DRAW_QUADS,
ASN_DRAW_POLYGON
};

enum ASN_DATA_Buffer{
ASN_DATA_VERTEX_BUFF=0,
ASN_DATA_NORMAL_BUFF,
ASN_DATA_COLOR_BUFF,
ASN_DATA_UV_BUFF,
};

class IDrawObject{
public:

virtual bool Release()=0; // удалить.. в том случае, если это не уникальный объект - удалить нельзя
// материал
// virtual bool SetMaterial(IMaterial *Material)=0;

virtual bool SetDrawType(ASN_DRAW_Types type, bool dynamic_=false)=0;

// буфер данных - в одном массиве храним вертексы, нормали, текстурные координаты
// на вход подаём указатель на буфер, размер одного элемента, кол-во элементов в буфере
virtual bool SetDataBuffer(void *buffer, unsigned int element_size, unsigned int buf_size)=0;

// задаём сдвиг данных
virtual bool SetDataOffset(ASN_DATA_Buffer buffer_type, unsigned long data_offset)=0;

// буфер индекса
virtual bool SetIndexBuffer(void *buffer, unsigned int element_size, unsigned int buf_size)=0;

};

struct IDrawTask {
IDrawObject *drawObject;
IMaterial *material;
};

#endif
