#include "CNode3ds.h"

#include "IBaseObject.h"

#include "IEngine.h"
#include "IDevice.h"
#include "IFileSystem.h"
#include "IResourceSystem.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

//#include "GL/gl.h"

#include "ASN_utils.h"

//extern void xyz_to_uv_sphere( float uv[2], const float vertex[3], const float normal[3] );

CNode3ds::CNode3ds(IEngine *_Engine):CNode(_Engine) {

//CenterPoint=CVector( 0,0,0 );
//Size=CVector( 1.0f,1.0f,1.0f );
//Rot=CVector( 0,0,0 );
//Pos=CVector( 0,0,0 );

//SetParam("MeshName","empty");
//SetParam("MaxSize",ASN_NOT_FOUND_PARAM);
SetParam("ModelScale","1.0");
SetParam("MeshFileName",ASN_NOT_FOUND_PARAM);

SetParam("BaseClass","Node");
SetParam("Type","Node3ds");
SetParam("Name","-");

SetParam("ModelSize","1.0");

//ObjList=NULL;

myModel=NULL;
defaultMaterial=NULL;
//Parent=NULL;
//num_objects=0;
};

CNode3ds::~CNode3ds(){
    printf("-CNode3ds\n");

//for (int i=0;i<num_objects;i++){
//delete ObjList[i];
//delete myModel[i];
//};

//delete []ObjList;
//delete myModel;
};


bool CNode3ds::Draw(){


//CVector MoveVec=CVector(0.001f,0.001f,0.001f);

//double Model_Matrix[16];
//double Project_Matrix[16];

CNode::Draw();

DEVICER->PushMatrix();
//CVector unSise = CVector ( 1.0f/Size.v[0], 1.0f/Size.v[1], 1.0f/Size.v[2] );

//printf("draw node 3ds %s\n", GetParam("Name") );

float scale=GetFloatParam("ModelScale"); //StrToFloat( GetParam("ModelScale") );

DEVICER->MultModelMatrix(NodeMatrix.m_data);

if ( std::string(GetParam("ShowSize"))=="1") {
    DEVICER->PushMatrix();
    DEVICER->SetScale( Size );
    IDrawObject *myObj = DEVICER->GetDrawObject ("_Cube");
    DEVICER->SetMaterial( NULL );
    if (NodeColor) {
    CVector4 myCol = *NodeColor;
    DEVICER->SetColor(myCol);
//    printf("color: %f %f %f %f\n",NodeColor->v[0],
//                                 NodeColor->v[1],
//                                  //NodeColor->v[2],
//                                  NodeColor->v[3]);
        //,myCol.v[1],myCol.v[2],myCol.v[3]);
    //DEVICER->SetColor( CVector4(1,0,0,1) );
    }
    DEVICER->Draw( myObj );
    DEVICER->PopMatrix();
    }

DEVICER->SetScale( CVector(scale,scale,scale) );

//printf("prepare draw 3ds...\n");
if (load_all){
//DEVICER->SetScale( myModel->Norm_Size*2 );
//myModel->drawPack.begin();

//    printf("draw 3ds...\n");

    // если задан один материал на все меши:
    if (defaultMaterial) {
    // устанавливаем его
    DEVICER->SetMaterial( defaultMaterial );
    std::vector<IDrawTask*>::iterator _it=myModel->drawPack.begin();
    if (NodeColor) DEVICER->SetColor(*NodeColor);

//	myModel->Draw3ds( DEVICER, defaultMaterial, NodeColor);
    for (UINT i=myModel->drawPack.size(); i>0; i--){
//	printf("def model %d\n",i);
        DEVICER->Draw( (*_it)->drawObject );
        _it++;
        };

    }
    else {

//	myModel->Draw3ds( DEVICER, defaultMaterial, NodeColor);

// устанавливаем материал индивидуально для каждого меша
    std::vector<IDrawTask*>::iterator _it=myModel->drawPack.begin();

    for (UINT i=myModel->drawPack.size(); i>0; i--){
//	printf("model %d\n",i);
        DEVICER->SetMaterial( (*_it)->material );
        if (NodeColor) DEVICER->SetColor(*NodeColor);
        DEVICER->Draw( (*_it)->drawObject );
        _it++;
        };

    };

DEVICER->PopMatrix();
return false;
}
else {
DEVICER->PopMatrix();
LoadResource();
return true;
};

};


char CNode3ds::Update(float tms){

CNode::Update(tms);

/*
 std::map<std::string,INode*>::iterator it=NodeNameList.begin();
for (UINT i=NodeNameList.size(); i>0; i--){
(*it).second->Update(tms);
it++;
};
*/

return false;
};

bool CNode3ds::LoadResource(){

printf("CNode3ds: try load resource \n");
// если задан материал для всех мешей - устанавливаем его
if ( std::string(GetParam("MeshMaterial"))!=ASN_NOT_FOUND_PARAM ) {
defaultMaterial = DEVICER->GetMaterial ( GetParam("MeshMaterial") ) ;
}
else defaultMaterial=NULL;

// меш не установлен - не знаю что грузить
if ( std::string(GetParam("MeshFileName"))==ASN_NOT_FOUND_PARAM) return true;

//void* tmp_data=ENGINE->GetPtrParam( GetParam("MeshFileName") );

void* tmp_data=RESOURSER->GetResource("Model3ds",GetParam("MeshFileName"));

myModel=(CModel3ds*)tmp_data;

if (myModel){
// если уже есть такое в кеше
//printf("Load from cache %s...\n",myModel->Name.c_str()/*, GetParam("Name")*/ );

//myModel->Draw3ds(NULL);

printf("set def mat\n");

//Size=
SetSize( myModel->Norm_Size*StrToFloat(GetParam("ModelScale")) ); //ставим размеры модели
load_all=true;
return false;
};

printf("loading file %s...",GetParam("MeshFileName") );
IFile *myFile=FILER->GetFile( GetParam("MeshFileName") );
if (!myFile) return true; //файла нет такого
myFile->FileOpen();
void *data=myFile->LoadInMem();
if (myFile->GetState()!=ASN_FILE_DISK_IN_MEM) return true;
printf("File size: %d",myFile->GetSizeInBytes());

myModel=new CModel3ds(Engine);
myModel->Name = GetParam("MeshFileName");
if ( std::string(GetParam("AutoMap"))=="1" ) myModel->AutoMap=true;
else myModel->AutoMap=false;

// если установлен параметр масштабирования
//if ( GetParam("MaxSize")!=ASN_NOT_FOUND_PARAM)
//myModel->MaxSize=StrToInt( GetParam("MaxSize") );
// если нет, то ставим 0
//else myModel->MaxSize=0;
//myModel->MaxSize=1.0f;

// если установлен массив цветов
if (std::string(GetParam("ColorM"))=="1") myModel->Color=true;
else myModel->Color=false;

if ( myModel->Load3ds(Engine, data,myFile->GetSizeInBytes()) ) {
// загрузка окончилась неудачей
myModel->Release();
myModel=NULL;
return true;
};

// загрузили нормально - загружаем в ресурс эту модель, вдруг понадобится еще кому
RESOURSER->SetResource("Model3ds", GetParam("MeshFileName"), (BYTE*)myModel , sizeof (myModel) );
//ENGINE->SetPtrParam( GetParam("MeshFileName"), myModel );
SetSize( myModel->Norm_Size*StrToFloat( GetParam("ModelScale") ) ); //ставим размеры модели
load_all=true;
return false;

/*
myModel = new ASN_SimpleIndexMeshPack();

//return true;


myFile->FileClose();
printf("[OK]\n");

// Читаем материалы сцены

    for (uint i= 0; i<scene.GetMaterialCount(); i++)
    {

    LMaterial& material = scene.GetMaterial(i);
    LMap& map=material.GetTextureMap1();
    _strlwr(map.mapName);

    IMaterial *myMaterial=DEVICER->AddMaterial( GetParam("MeshFileName")+material.GetName() );
    std::string MyName=_strlwr(map.mapName);
    fprintf(stdout,"----Material name: %s----\n",material.GetName().c_str());
    fprintf(stdout,"MapName: %s\n",map.mapName);

    if (MyName!="") myMaterial->SetParam("Texture", MyName );

        LColor3 lcolor3=material.GetAmbientColor();
     fprintf(stdout,"AmbientColor: %f %f %f\n",lcolor3.r,lcolor3.g,lcolor3.b);
     myMaterial->SetParam("Ambient",  VectorToStr( CVector(lcolor3.r,lcolor3.g,lcolor3.b) ) );
     lcolor3=material.GetDiffuseColor();
     fprintf(stdout,"DiffuseColor: %f %f %f\n",lcolor3.r,lcolor3.g,lcolor3.b);
     myMaterial->SetParam("Diffuse",  VectorToStr( CVector(lcolor3.r,lcolor3.g,lcolor3.b) ) );
     lcolor3=material.GetSpecularColor();
     fprintf(stdout,"SpecularColor: %f %f %f\n",lcolor3.r,lcolor3.g,lcolor3.b);
     myMaterial->SetParam("Specular",  VectorToStr( CVector(lcolor3.r,lcolor3.g,lcolor3.b) ) );


     float shin=material.GetShininess();
     fprintf(stdout,"GetShininess: %f\n",shin);
     myMaterial->SetParam("Shininess", FloatToStr(shin) );

    fprintf(stdout,"texture: %s\n",map.mapName);
    fprintf(stdout,"alpha: %f\n",map.strength);


    };

 printf("Materials OK\n");

 char buf[80];

if (ObjList) {
for (int i=0;i<num_objects;i++) if (ObjList[i]) delete ObjList[i];
delete []ObjList;
ObjList=NULL;
};

if (myModel) {
//for (int i=0;i<num_objects;i++) if (myModel[i]) delete myModel[i];
//delete myModel;
myModel=NULL;
}

  num_objects = scene.GetMeshCount();

  ObjList = new DrawObj_ptr[num_objects];
//  myModel = new ASN_SimpleIndexMesh_ptr[num_objects];

for (int i=0;i<num_objects;i++){
ObjList[i]=NULL;
//myModel[i]=NULL;
};

//return false;

float min_x=100000.0f;
float min_y=100000.0f;
float min_z=100000.0f;

float max_x=-1000.0f;
float max_y=-1000.0f;
float max_z=-1000.0f;

    for (uint i= 0; i<scene.GetMeshCount(); i++) {

    sprintf(buf,"%d",i);

    LMesh &mesh = scene.GetMesh(i);

    ObjList[i] = new IDrawTask;
    ObjList[i]->drawObject=DEVICER->AddDrawObject(GetParam("MeshFileName")+std::string(buf) );
    ObjList[i]->material = NULL;

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
    LVector2 vec_2=mesh.GetUV(k);

  // если включено автокартографирование - вычисляем вертексы
    if (GetParam("AutoMap")=="1"){

    xyz_to_uv_sphere( myIndexMesh->vertices[k].tex.v , myIndexMesh->vertices[k].pos.v, myIndexMesh->vertices[k].n.v );

    }

    else {
 // иначе читаем их из модели
    myIndexMesh->vertices[k].tex.v[0]=vec_2.x;
    myIndexMesh->vertices[k].tex.v[1]=vec_2.y;
    }

   }; /// закончили заполнять вертексы...

   myIndexMesh->SetIndexNum( mesh.GetTriangleCount() ); //инициализируем место для индексов

    // заполняем индексы
    for(uint k= 0; k<mesh.GetTriangleCount(); k++) {

    LTriangle tri=mesh.GetTriangle(k);

    myIndexMesh->faces[k].index[0]=tri.a;
    myIndexMesh->faces[k].index[1]=tri.b;
    myIndexMesh->faces[k].index[2]=tri.c;

    };

    // запоминаем меш
    myModel->meshPack.push_back(myIndexMesh);

    // устанавливаем смещения
    myIndexMesh->SetPointerIn(ObjList[i]->drawObject);

    // если включен цветовой массив - устанавливаем смещение на него
   if (GetParam("Color")=="1")
         ObjList[i]->drawObject->SetDataOffset(ASN_DATA_COLOR_BUFF, ((int)&myIndexMesh->vertices[0].n) - ((int)&myIndexMesh->vertices [0]));

defaultMaterial
    // если задан материал для всех мешей - устанавливаем его
    if ( GetParam("MeshMaterial")!=ASN_NOT_FOUND_PARAM ) {
    ObjList[i]->material=DEVICER->GetMaterial ( GetParam("MeshMaterial") ) ;
    ObjList[i]->drawObject->SetDataOffset(ASN_DATA_UV_BUFF, ((int)&myIndexMesh->vertices[0].tex) - ((int)&myIndexMesh->vertices [0]));
    }
    // иначе, если число материалов в меше больше 0
    else if (mesh.GetMaterialCount()>0) {

    uint MaterialID=mesh.GetMaterial(0); // читаем ID первого материала меша

    // берём материал
    LMaterial& material = scene.GetMaterial(MaterialID);
//    LMap& map=material.GetTextureMap1();

//    printf("texture: %s\n",map.mapName);

    // берём его имя
    std::string MaterialName=GetParam("MeshFileName")+material.GetName();

//    fprintf(stdout,"{%s} VS {%s}\n",MaterialName.c_str(), GetParam("MeshName").c_str() );
//    fprintf(stdout,"{%s}{%s}\n",material.GetName().c_str(),
//    DEVICER->GetMaterial ( MaterialName )->GetParam("Texture").c_str()
//          );

       // и устанавливаем его!
      ObjList[i]->material= DEVICER->GetMaterial ( MaterialName ) ;
//      if (DEVICER->GetMaterial ( MaterialName )->GetParam("Texture")!=ASN_NOT_FOUND_PARAM )
      ObjList[i]->drawObject->SetDataOffset(ASN_DATA_UV_BUFF, ((int)&myIndexMesh->vertices[0].tex) - ((int)&myIndexMesh->vertices [0]));

  };
};

//printf("Load ok Resizing\n");

    float length_x=fabs(max_x-min_x);
    float length_y=fabs(max_y-min_y);
    float length_z=fabs(max_z-min_z);

    Size = CVector(length_x, length_y, length_z);
    for (int i=0;i<num_objects;i++){
//    myModel[i]->Move(-min_x,-min_y,-min_z); //сдвигаем модельку на начало координаты
    };

    CenterPoint = Size /2 ;
//Resize();


if ( GetParam("MaxSize")!=ASN_NOT_FOUND_PARAM) Resize();

printf("Load ok!\n");
//MyMesh3d->SetMaterial( DEVICER->GetMaterial( GetParam("MeshMaterial").c_str()) );
//Tmesh *mTMesh;
//Load3ds(mTMesh,(char*)GetParam("MeshFileName").c_str());
//printf("mesh loaded %d\n",num_vertex);

load_all=true;
return false;
};

bool CNode3ds::Resize() {

/*
    float length=GetMax(Size.v[0],Size.v[1]);
    length=GetMax(length,Size.v[2]);

    float MaxSize=StrToFloat( GetParam("MaxSize") );//10.0f;
    float Modulor_x=MaxSize/length;
    float Modulor_y=MaxSize/length;
    float Modulor_z=MaxSize/length;

    for (int i=0;i<num_objects;i++){
    myModel[i]->Scale(Modulor_x,Modulor_y,Modulor_z);
    };

    Size = Size * Modulor_x;
    printf("Resized: %f %f %f", Size.v[0], Size.v[1], Size.v[2] );
*/

//return true;
};

//void CNode3ds::setSize(const CVector& _Size){
//Size=_Size;
//};

void CNode3ds::SetParam(const char* param_name,const char* param_value){

if (std::string(param_name)=="MeshFileName") {myModel=NULL; load_all=false;} //поменялся меш - надо загрузить
if (std::string(param_name)=="MeshMaterial") {defaultMaterial=NULL; load_all=false;} //поменялся материал - надо б загрузить однако
if (std::string(param_name)=="ModelScale") {
    // если модель уже есть - меняем размеры соответственно
    if (load_all) {
//         CVector size=myModel->Norm_Size;
         SetSize( myModel->Norm_Size*StrToFloat(param_value) );
     };
//    if (myModel) setSize( myModel->Norm_Size*StrToFloat(param_value) );
    } //поменялся масштаб модели - надо загрузить

CNode::SetParam(param_name, param_value);

};

