
#include <stdlib.h>
//#include "lib/extgl.h"
//#include <GL/gl.h>

#include "CDrawObject.h"

CDrawObject::CDrawObject(){

//init_vbo=false;
vertexId=0;
IndexSize=0;
DataBuffer=NULL;
IndexBuffer=NULL;

 Draw_Type=GL_TRIANGLES;
 NumPointInPrim=3; // 3 точки в треугольнике - логично, да?

 dynamic=false;
//Material=NULL;
//Buffers_num=0;
//Buffers_Type=NULL;
//Buffers=NULL;
//	extgl_Initialize();
//		if (extgl_Extensions.ARB_vertex_buffer_object) printf("ARB_vertex_buffer OK");

}

CDrawObject::~CDrawObject(){

}

bool CDrawObject::Release(){

}

/*
bool CDrawObject::SetMaterial(IMaterial *Material_){
fprintf(stdout,"Set Material: %s\n",Material_->GetParam("Name").c_str());
Material=(CMaterial*)Material_;
//vertex=NULL;
};
*/

bool CDrawObject::SetDataBuffer(void *buffer, unsigned int element_size, unsigned int buf_size){

DataBuffer=buffer;
BufSize=buf_size;
BufElemSize=element_size;

vertexId=0;
IndexSize=0;

DataOffset[ASN_DATA_VERTEX_BUFF]=0;
DataOffset[ASN_DATA_NORMAL_BUFF]=0;
DataOffset[ASN_DATA_COLOR_BUFF]=0;
DataOffset[ASN_DATA_UV_BUFF]=0;

}

bool CDrawObject::SetIndexBuffer(void *buffer, unsigned int element_size, unsigned int buf_size){

IndexBuffer=buffer;
IndexSize=buf_size;
IndexElemSize=element_size;

}

bool CDrawObject::SetDataOffset(ASN_DATA_Buffer buffer_type, unsigned long data_offset){

DataOffset[buffer_type]=data_offset;
//printf("data offset %d\n",data_offset);
}

bool CDrawObject::SetDrawType(ASN_DRAW_Types type, bool dynamic_){

dynamic=dynamic_;

switch (type) {

case ASN_DRAW_POINTS: Draw_Type=GL_POINTS;NumPointInPrim=1;break;
case ASN_DRAW_LINE_STRIP: Draw_Type=GL_LINE_STRIP;NumPointInPrim=1;break;
case ASN_DRAW_LINE_LOOP: Draw_Type=GL_LINE_LOOP;NumPointInPrim=1;break;
case ASN_DRAW_LINES: Draw_Type=GL_LINES;NumPointInPrim=2;break;
case ASN_DRAW_TRIANGLE_STRIP: Draw_Type=GL_TRIANGLE_STRIP;NumPointInPrim=3;break;
case ASN_DRAW_TRIANGLE_FAN: Draw_Type=GL_TRIANGLE_FAN;NumPointInPrim=3;break;
case ASN_DRAW_TRIANGLES: Draw_Type=GL_TRIANGLES;NumPointInPrim=3;break;
case ASN_DRAW_QUAD_STRIP: Draw_Type=GL_QUAD_STRIP;NumPointInPrim=4;break;
case ASN_DRAW_QUADS: Draw_Type=GL_QUADS;NumPointInPrim=4;break;
case ASN_DRAW_POLYGON: Draw_Type=GL_POLYGON;NumPointInPrim=1;break;

//case ASN_DRAW_LINES: Draw_Type=GL_LINES;break;
//case ASN_DRAW_QUADS: Draw_Type=GL_QUADS;break;
default:
    Draw_Type=GL_TRIANGLES;
//case ASN_DRAW_TRIANGLES: Draw_Type=ASN_DRAW_TRIANGLES
    break;
};

//Draw_Type=type;
}
