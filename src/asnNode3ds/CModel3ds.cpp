#include "CModel3ds.h"

#include "lib/l3ds.h"
#include "IDevice.h"

#include "ASN_utils.h"

#include <iostream>

#define ENGINE Engine //у нас уже есть указатель на Engine

extern void xyz_to_uv_sphere( float uv[2], const float vertex[3], const float normal[3] );

CModel3ds::CModel3ds(IEngine* engine):CBaseObject(engine){

};

CModel3ds::~CModel3ds(){

// удаляем индекс-меши
std::vector<ASN_SimpleIndexMesh*>::iterator it=meshPack.begin();
for (UINT i=meshPack.size(); i>0; i--){
delete [](*it)->faces;
delete [](*it)->vertices;
delete (*it);
it++;
};

// удаляем таски
std::vector<IDrawTask *>::iterator _it=drawPack.begin();
for (UINT i=drawPack.size(); i>0; i--){
delete [](*_it);
_it++;
};

};

bool CModel3ds::Load3ds(IEngine *Engine, void *data, UINT size){

L3DS scene;

scene.LoadMem(data,size);


// Читаем материалы сцены

    for (uint i= 0; i<scene.GetMaterialCount(); i++)
    {

//    char UpPer[] = " HELLO, My Name Is John! ";

//    locale locl("German_Germany");
//    use_facet< ctype<char> > (locl).tolower( &UpPer[0], &UpPer[strlen(&UpPer[0])-1 );

    LMaterial& material = scene.GetMaterial(i);
    LMap& map=material.GetTextureMap1();
//    strlwr(map.mapName);

    IMaterial *myMaterial=DEVICER->AddMaterial( ASN_Str( Name+material.GetName()).c_str() );
    std::string MyName=StrLow(map.mapName);
    fprintf(stdout,"----Material name: %s----\n",material.GetName().c_str());
    fprintf(stdout,"MapName: %s\n", MyName.c_str());

    if (MyName!="") myMaterial->SetParam("Texture", MyName.c_str() );

        LColor3 lcolor3=material.GetAmbientColor();
     fprintf(stdout,"AmbientColor: %f %f %f\n",lcolor3.r,lcolor3.g,lcolor3.b);
     myMaterial->SetParam("Ambient",  VectorToStr( CVector(lcolor3.r,lcolor3.g,lcolor3.b) ).c_str() );
     lcolor3=material.GetDiffuseColor();
     fprintf(stdout,"DiffuseColor: %f %f %f\n",lcolor3.r,lcolor3.g,lcolor3.b);
     myMaterial->SetParam("Diffuse",  VectorToStr( CVector(lcolor3.r,lcolor3.g,lcolor3.b) ).c_str() );
     lcolor3=material.GetSpecularColor();
     fprintf(stdout,"SpecularColor: %f %f %f\n",lcolor3.r,lcolor3.g,lcolor3.b);
     myMaterial->SetParam("Specular",  VectorToStr( CVector(lcolor3.r,lcolor3.g,lcolor3.b) ).c_str() );

     float shin=material.GetShininess();
     fprintf(stdout,"GetShininess: %f\n",shin);
     myMaterial->SetParam("Shininess", FloatToStr(shin).c_str() );

    fprintf(stdout,"texture: %s\n",map.mapName);
    fprintf(stdout,"alpha: %f\n",map.strength);


    };

 printf("Materials OK\n");

// Прочитали материалы сцены

// инициализируем минимальную и максимальную координаты
float min_x=100000.0f;
float min_y=100000.0f;
float min_z=100000.0f;

float max_x=-1000.0f;
float max_y=-1000.0f;
float max_z=-1000.0f;

 char buf[80];

 // цикл по всем мешам сцены
    for (uint i= 0; i<scene.GetMeshCount(); i++) {

    sprintf(buf,"%d",i);

    LMesh &mesh = scene.GetMesh(i);

    IDrawTask *myDrawTask = new IDrawTask;
    myDrawTask->drawObject=DEVICER->AddDrawObject( ASN_Str(Name+std::string(buf)).c_str() );
    myDrawTask->material = NULL;

    ASN_SimpleIndexMesh *myIndexMesh = new ASN_SimpleIndexMesh;

    printf("material count: %d\n",mesh.GetMaterialCount() );

//инициализируем место под вертексы
    myIndexMesh->SetVertexNum( mesh.GetVertexCount() );


/// заполняем вертексы...
    for(uint k= 0; k<mesh.GetVertexCount(); k++) {

    /// заполняем координаты...
    LVector4 vec=mesh.GetVertex(k);

    myIndexMesh->vertices[k].pos.v[0]=vec.x;
    myIndexMesh->vertices[k].pos.v[1]=vec.y;
    myIndexMesh->vertices[k].pos.v[2]=vec.z;

  min_x=GetMin(vec.x,min_x);
  min_y=GetMin(vec.y,min_y);
  min_z=GetMin(vec.z,min_z);

  max_x=GetMax(vec.x,max_x);
  max_y=GetMax(vec.y,max_y);
  max_z=GetMax(vec.z,max_z);

   /// заполняем нормали...
   LVector3 vec_3=mesh.GetNormal(k);

    myIndexMesh->vertices[k].n.v[0]=vec_3.x;
    myIndexMesh->vertices[k].n.v[1]=vec_3.y;
    myIndexMesh->vertices[k].n.v[2]=vec_3.z;

  /// заполняем текстурные координаты...
  // если включено автокартографирование - вычисляем вертексы
    if (AutoMap){
   xyz_to_uv_sphere( myIndexMesh->vertices[k].tex.v , myIndexMesh->vertices[k].pos.v, myIndexMesh->vertices[k].n.v );
//   printf("automap...OK\n");
    }

    else {
 // иначе читаем их из модели
    LVector2 vec_2=mesh.GetUV(k);
    myIndexMesh->vertices[k].tex.v[0]=vec_2.x;
    myIndexMesh->vertices[k].tex.v[1]=vec_2.y;
    }

   };  /// закончили заполнять вертексы...

   //инициализируем место для индексов

   myIndexMesh->SetIndexNum( mesh.GetTriangleCount() );

    // заполняем индексы
    for(uint k= 0; k<mesh.GetTriangleCount(); k++) {

    LTriangle tri=mesh.GetTriangle(k);

    myIndexMesh->faces[k].index[0]=tri.a;
    myIndexMesh->faces[k].index[1]=tri.b;
    myIndexMesh->faces[k].index[2]=tri.c;

    };

    // устанавливаем в таске смещения на меш
    myIndexMesh->SetPointerIn(myDrawTask->drawObject);

    // если включено автокартографирование, то надо бы указать ссылку на текстурные координаты
    if (AutoMap)
    myDrawTask->drawObject->SetDataOffset(ASN_DATA_UV_BUFF, ((long)&myIndexMesh->vertices[0].tex) - ((long)&myIndexMesh->vertices [0]));

   if (Color)
    myDrawTask->drawObject->SetDataOffset(ASN_DATA_COLOR_BUFF, ((long)&myIndexMesh->vertices[0].n) - ((long)&myIndexMesh->vertices [0]));

    // если на меш установлено материалов больше чем 1
    if (mesh.GetMaterialCount()>0) {

    // читаем ID первого материала меша
    uint MaterialID=mesh.GetMaterial(0);
    // получаем материал по ID
    LMaterial material = scene.GetMaterial(MaterialID);
    // берём имя материала
    std::string MaterialName=Name+material.GetName();
    // устанавливаем материал в таск
    myDrawTask->material= DEVICER->GetMaterial ( MaterialName.c_str() ) ;
    myDrawTask->drawObject->SetDataOffset(ASN_DATA_UV_BUFF, ((long)&myIndexMesh->vertices[0].tex) - ((long)&myIndexMesh->vertices [0]));

    };

    drawPack.push_back(myDrawTask);
    meshPack.push_back(myIndexMesh);

}; /// закончился цикл по всем мешам сцены


// сдвигаем

    float length_x=fabs(max_x-min_x);
    float length_y=fabs(max_y-min_y);
    float length_z=fabs(max_z-min_z);

    Real_Size = CVector(length_x, length_y, length_z);

    std::vector<ASN_SimpleIndexMesh*>::iterator it=meshPack.begin();
    for (UINT i=meshPack.size(); i>0; i--){
    (*it)->Move(-min_x,-min_y,-min_z); //сдвигаем модельку на начало координаты
    it++;
    };

// масштабируем в 1

//    Norm_Size = Real_Size;

MaxSize = 1.0f;

//if (MaxSize!=0) {

    float length=GetMax(Real_Size.v[0],Real_Size.v[1]);
    length=GetMax(length,Real_Size.v[2]);

//    float Modulor = 1.0f/length;

    float Modulor_x=MaxSize/length;
    float Modulor_y=MaxSize/length;
    float Modulor_z=MaxSize/length;

//    float Modulor_x=Real_Size.v[0];
//    float Modulor_y=Real_Size.v[1];
//    float Modulor_z=Real_Size.v[2];

printf("%f %f %f\n",Modulor_x,Modulor_y,Modulor_z);

    it=meshPack.begin();
    for (UINT i=meshPack.size(); i>0; i--){
    (*it)->Scale(Modulor_x,Modulor_y,Modulor_z);
    it++;
    };

    Norm_Size = Real_Size * Modulor_x;
    printf("Resized: %f %f %f\n", Norm_Size.v[0], Norm_Size.v[1], Norm_Size.v[2] );
//}


return false;

};

bool CModel3ds::Draw3ds(IDevice* device, bool add_material, CVector4* node_color){

printf("draw model 3ds %s %d\n",Name.c_str(),drawPack.size());

    std::vector<IDrawTask*>::iterator _it=drawPack.begin();

if (!add_material) {

    for (UINT i=drawPack.size(); i>0; i--){
	printf("def model %d\n",i);
        device->Draw( (*_it)->drawObject );
        _it++;
        };

} else {

    for (UINT i=drawPack.size(); i>0; i--){
	printf("model %d\n",i);
        device->SetMaterial( (*_it)->material );
        if (node_color) DEVICER->SetColor(*node_color);
        device->Draw( (*_it)->drawObject );
        _it++;
        };
}

};

