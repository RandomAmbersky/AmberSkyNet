#ifndef _CSCENE_OBJECT_H
#define _CSCENE_OBJECT_H

#include "IDrawObject.h"
#include "CMaterial.h"
#include <map>

#include <GL/gl.h>

//struct BuffInfo {
//ASN_DATA_Buffer Buffers_Type;
//void *Buffer;
//unsigned int BuffSize;
//};

//typedef BuffInfo BuffInfo_type;

class CDrawObject: public IDrawObject{
public:

CDrawObject();
~CDrawObject();

bool Release();

bool SetDrawType(ASN_DRAW_Types type, bool dynamic_);

//bool SetMaterial(IMaterial *Material_);

// буфер данных - в одном массиве храним вертексы, нормали, текстурные координаты
bool SetDataBuffer(void *buffer, unsigned int element_size, unsigned int buf_size);

// задаём сдвиг данных
bool SetDataOffset(ASN_DATA_Buffer buffer_type, unsigned long data_offset);

// буфер индекса
bool SetIndexBuffer(void *buffer, unsigned int element_size, unsigned int buf_size);

//ASN_DRAW_Types Draw_Type;

GLuint Draw_Type;

//CMaterial* Material;
void* DataBuffer;
unsigned int BufSize;
unsigned int BufElemSize;

unsigned int NumPointInPrim; // число точек в одном примитиве


void* IndexBuffer;
unsigned int IndexSize;
unsigned int IndexElemSize;

std::string Name;

bool dynamic;
//bool init_vbo;                              // заполнены ли буферы VBO

//uint *Vertex_buf;

unsigned int vertexId;						// id of vertex VBO
unsigned int indexId;						// id of index VBO

std::map<ASN_DATA_Buffer, unsigned long> DataOffset;
};

#endif

