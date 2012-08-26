//
// Simple 3d mesh class
//

#ifndef	__ASN_SIMPLEMODEL__
#define	__ASN_SIMPLEMODEL__

#include "ASN_Types.h"

#include "IDrawObject.h"

#include <stdlib.h>
#include <vector>

struct ASN_SM_Vector3D{
 float v[3];
};

struct ASN_SM_Vector2D{
 float v[2];
};

struct	ASN_SM_Vertex
{
	ASN_SM_Vector3D	pos;						// position of vertex
	ASN_SM_Vector2D	tex;						// texture coordinates
	ASN_SM_Vector3D	n;							// unit normal
};

struct	ASN_SM_Triangle						// triangular face
{
	int	index [3];							// indices to Vertex array
};

struct	ASN_SM_Quad						// Quad face ---added by Meridian
{
	int	index [4];							// indices to Vertex array
};

class	ASN_SimpleMesh
{
public:
	int		 numVertices;
	ASN_SM_Vertex* vertices;

	ASN_SimpleMesh(){
	vertices=NULL;
	numVertices=0;
	};

	~ASN_SimpleMesh(){ Free(); }

    void SetVertexNum(UINT vert_num){ numVertices=vert_num; vertices = new ASN_SM_Vertex[vert_num]; };

    void SetPointerIn( IDrawObject *myObj){

   myObj->SetDataBuffer(vertices,sizeof(ASN_SM_Vertex), numVertices);
   myObj->SetDataOffset(ASN_DATA_NORMAL_BUFF, ((long)&vertices[0].n) - ((long)&vertices [0]) );
   myObj->SetDataOffset(ASN_DATA_UV_BUFF, ((long)&vertices[0].tex) - ((long)&vertices [0]));

    };

    void Scale(float Modulorus_x, float Modulorus_y, float Modulorus_z){
        for (UINT i=0;i<numVertices;i++){
                vertices[i].pos.v[0]*=Modulorus_x;
                vertices[i].pos.v[1]*=Modulorus_y;
                vertices[i].pos.v[2]*=Modulorus_z;
            };

    };

    void Move(float Modulorus_x, float Modulorus_y, float Modulorus_z){
        for (UINT i=0;i<numVertices;i++){
                vertices[i].pos.v[0]+=Modulorus_x;
                vertices[i].pos.v[1]+=Modulorus_y;
                vertices[i].pos.v[2]+=Modulorus_z;
            };

    };

   void Free(){
      if (numVertices!=0) free(vertices);
      numVertices=0;
   };

};

class ASN_SimpleIndexMesh: public ASN_SimpleMesh
{
public:
	int		 numFaces;
	ASN_SM_Triangle* faces;

	ASN_SimpleIndexMesh(){
	faces=NULL;
	numFaces=0;
	};

    void SetIndexNum(UINT index_num){
        if (faces) Free();
        numFaces=index_num;
        faces= new ASN_SM_Triangle[index_num];
     };

    void SetPointerIn( IDrawObject *myObj){

   myObj->SetDataBuffer(vertices,sizeof(ASN_SM_Vertex), numVertices);
   myObj->SetDataOffset(ASN_DATA_NORMAL_BUFF, ((long)&vertices[0].n) - ((long)&vertices [0]));
   myObj->SetDataOffset(ASN_DATA_UV_BUFF, ((long)&vertices[0].tex) - ((long)&vertices [0]));
   myObj->SetIndexBuffer(faces,sizeof(ASN_SM_Triangle),numFaces);

    };

     void Free(){
//      ASN_SimpleMesh::Free();
      if (numFaces!=0) free(faces);
      numFaces=0;
      printf("free buffer...\n");
   };

   ~ASN_SimpleIndexMesh(){ Free(); }

};

/*
class ASN_SimpleIndexMeshPack: public IasnObject{
public:
std::vector<ASN_SimpleIndexMesh*> meshPack;
std::string GetBaseClass(){ return "IndexMeshPack"; }
std::string GetType(){ return "SimpleIndexMeshPack"; }
// имя нам тут не надо по идее
std::string GetName(){ return "SimpleIndexMeshPack_1";  }

~ASN_SimpleIndexMeshPack(){
// удаляем все меши отседова
std::vector<ASN_SimpleIndexMesh*>::iterator it=meshPack.begin();
for (UINT i=meshPack.size(); i>0; i--){
delete (*it);
it++;
};

};

};
*/

typedef ASN_SimpleMesh* ASN_SimpleMesh_ptr;
typedef ASN_SimpleIndexMesh* ASN_SimpleIndexMesh_ptr;

#endif
