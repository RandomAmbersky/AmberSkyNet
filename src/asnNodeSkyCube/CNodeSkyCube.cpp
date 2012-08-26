#include "IEngine.h"

#include "CNodeSkyCube.h"
//#include "GL/gl.h"
#include "ASN_utils.h"
#include <iostream>

#include "ILog.h"
//#include "IFile.h"
//#include "IFileSystem.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

using namespace std;

const float QuadBuffer[]=
{
	0, 0, 0,   0,0,  0,0,0,
 	1, 0, 0,   1,0,  0,0,0,
	1, 0, 1,   1,1,  0,0,0,
	0, 0, 1,   0,1,  0,0,0,

    0, 1, 0,   1,1,  0,0,0,
    1, 1, 0,   0,1,  0,0,0,
    0, 1, 1,   1,0,  0,0,0,
    1, 1, 1,   0,0,  0,0,0,
/*	1, 0, 0,   0,0,  0,0,1,
 	1, 0, 1,   1,0,  0,0,1,
	1, 1, 1,   1,1,  0,0,1,
	1, 1, 0,   0,1,  0,0,1,

    1, 1, 0,   0,0,  0,0,1,
 	1, 1, 1,   1,0,  0,0,1,
	0, 1, 1,   1,1,  0,0,1,
	0, 1, 0,   0,1,  0,0,1,

	0, 0, 1,   0,0,  0,0,1,
 	0, 1, 1,   1,0,  0,0,1,
	0, 1, 0,   1,1,  0,0,1,
	0, 0, 0,   0,1,  0,0,1,

	0, 0, 0,   0,0,  0,0,1,
 	1, 0, 0,   1,0,  0,0,1,
	1, 1, 0,   1,1,  0,0,1,
	0, 1, 0,   0,1,  0,0,1,

    1, 0, 1,   0,0,  0,0,1,
 	1, 1, 1,   1,0,  0,0,1,
	0, 1, 1,   1,1,  0,0,1,
	0, 0, 1,   0,1,  0,0,1,*/
};

unsigned int IndexBuffer[]=
{
  0, 1, 2, 3,
  4, 5, 7, 6,
  0, 1, 2, 3,
  4, 5, 7, 6,


  0, 4, 6, 3, // лицевая грань
  1, 5, 7, 2, // тыльная
  3, 6, 7, 2,
  0, 4, 5, 1,
};

CNodeSkyCube::CNodeSkyCube(IEngine *_Engine):CNode(_Engine)
{
    DrawSprite=NULL;
    DrawMaterial=NULL;
    dataBuffer=NULL;

    SetParam("BaseClass","Node");
    SetParam("Type","NodeSkyCube");
    SetParam("Name","-");

    SetPtrParam("Node", this);

    SetParam("Texture",ASN_NOT_FOUND_PARAM);
    current_frame=0;
    lifeTime=0;
    SetParam("Anim_Count_X","1");
    SetParam("Anim_Count_Y","1");
    SetParam("Start_Anim","0");
    SetParam("Count","1");
    SetParam("Repeat","0");
    SetParam("Billbord","0");
    SetParam("Speed","0.5");

//    printf("Create CNodeSprite\n");
};

CNodeSkyCube::~CNodeSkyCube()
{
    printf("Delete CNodeSprite\n");
    delete dataBuffer;
    if (DrawSprite) DrawSprite->Release();
};

bool CNodeSkyCube::LoadResource()
{
    if (load_all) return false; // если все загружено уже - возвращаемся сразу
    std::string TextureName;
    // не задано имя текстуры - возвращаем ошибку
    if ( (TextureName=std::string(GetParam("Texture")))==ASN_NOT_FOUND_PARAM) return true;
        // читаем объект отрисовки с именем текстуры
    // DrawSprite=DEVICER->GetDrawObject( TextureName );
    // если объект отрисовки не найден, создаём его
    if ( !DrawSprite )
    {
        printf("add new sprite...\n");
        DrawSprite=DEVICER->AddDrawObject( "-" ); //создаём динамический объект
        // если dataBuffer у нас еще не заполнили - создаём его
        if (!dataBuffer) dataBuffer=new float[8*8];
            memcpy(dataBuffer,QuadBuffer,64*sizeof(float));

        DrawSprite->SetDrawType(ASN_DRAW_QUADS,true);
        DrawSprite->SetDataBuffer((void*)dataBuffer,sizeof(float)*8,8);
        DrawSprite->SetDataOffset(ASN_DATA_VERTEX_BUFF,0);
        DrawSprite->SetDataOffset(ASN_DATA_UV_BUFF, 3*sizeof(float) );
        DrawSprite->SetDataOffset(ASN_DATA_NORMAL_BUFF, 5*sizeof(float) );
        DrawSprite->SetIndexBuffer(IndexBuffer,sizeof(ASN_SM_Quad),4);
    };

    // читаем материал с именем текстуры
    DrawMaterial=DEVICER->GetMaterial( TextureName.c_str() );

    // если такого материала нет - создаём его
    if (!DrawMaterial)
    {
        DrawMaterial=DEVICER->AddMaterial( TextureName.c_str() );
        if (!DrawMaterial)
        {
            LOGGER->ErrMsgStr(2, std::string("Error! Can't create material ")+TextureName );
            return true;
        }

        DrawMaterial->SetParam("Texture", TextureName.c_str() );
        DrawMaterial->SetParam("TextureOnly", "1" ); //мы используем только текстуру
        DrawMaterial->SetParam("Texture_repeat", "1" ); //повторяем текстуру


    };



    float old_life_time=lifeTime;
    load_all=true;
    lifeTime=1.0f;
    Update(Speed);
    lifeTime=old_life_time;

    return false;

};

bool CNodeSkyCube::Draw(){

//  return false;
 //printf("Draw\n");
//    CNode::Draw();

    if (load_all) {

    DEVICER->PushMatrix();
    DEVICER->MultModelMatrix(NodeMatrix.m_data);
/*
    if (Billbord) {
        // будем вычислять билборд

//           DEVICER->Move( out_Pos );
//           DEVICER->SetScale( Size );

           double Model_Matrix[16];
           double Project_Matrix[16];
           DEVICER->GetMatrixs(Model_Matrix, Project_Matrix);

           CVector right=CVector((float)Model_Matrix[0], (float)Model_Matrix[4], (float)Model_Matrix[8]);
           CVector up=CVector((float)Model_Matrix[1], (float)Model_Matrix[5], (float)Model_Matrix[9]);

//           right*=0.5;
//           up*=0.5;

//           printf("Right %f %f %f\n",right.v[0],right.v[1],right.v[2]);
//           printf("Up %f %f %f\n",up.v[0],up.v[1],up.v[2]);
        // Pos
        // NodeMatrix.m_data[12]
      	// NodeMatrix.m_data[13]
   		// NodeMatrix.m_data[14]

           dataBuffer[0]=(0-right.v[0]-up.v[0])*Size.v[0];
           dataBuffer[1]=(0-right.v[1]-up.v[1])*Size.v[1];
           dataBuffer[2]=(0-right.v[2]-up.v[2])*Size.v[2];

           dataBuffer[0+8]=(0-right.v[0]+up.v[0])*Size.v[0];
           dataBuffer[1+8]=(0-right.v[1]+up.v[1])*Size.v[1];
           dataBuffer[2+8]=(0-right.v[2]+up.v[2])*Size.v[2];

           dataBuffer[0+8+8]=(0+right.v[0]+up.v[0])*Size.v[0];
           dataBuffer[1+8+8]=(0+right.v[1]+up.v[1])*Size.v[1];
           dataBuffer[2+8+8]=(0+right.v[2]+up.v[2])*Size.v[2];

           dataBuffer[0+8+8+8]=(0+right.v[0]-up.v[0])*Size.v[0];
           dataBuffer[1+8+8+8]=(0+right.v[1]-up.v[1])*Size.v[1];
           dataBuffer[2+8+8+8]=(0+right.v[2]-up.v[2])*Size.v[2];

           dataBuffer[0+8+8+8+8]=(0+right.v[0]-up.v[0])*Size.v[0];
           dataBuffer[1+8+8+8+8]=(0+right.v[1]-up.v[1])*Size.v[1];


        }
        else {
     //           DEVICER->MultModelMatrix(NodeMatrix.m_data);
*/
                DEVICER->SetScale( Size );
//               };


    DEVICER->SetMaterial( DrawMaterial );
    if (NodeColor) DEVICER->SetColor(*NodeColor);
    DEVICER->Draw( DrawSprite );
    DEVICER->PopMatrix();

   }
    else return LoadResource();
    return false;
};

/*char CNodeSkyCube::Update(float tms){

    if (tms<0) return true; //бред какой-то!

//    SetParam("CurrentFrame", IntToStr(current_frame).c_str() );
    SetParam("lifeTime", FloatToStr(lifeTime).c_str() );

    lifeTime+=tms;

//   if (lifeTime<0) { printf("%f update time: %f\n",lifeTime,tms);   system("PAUSE"); }

    if (lifeTime>=Speed) lifeTime-=Speed;
    else return false;

//  return false;
    if (load_all) {

//    if (Repeat!=0) {

//    printf("update %d %d...\n",current_frame,Count);
    current_frame++;
//    SetParam("CurrentFrame", IntToStr(current_frame).c_str() );
    if ( current_frame>Count ) { current_frame=0;
//            printf("REPEAT: %d\n",Repeat);
            Repeat--; }
    if (Repeat==1) { /*delete this;*///Release(); return 2;}

//        }

//    if ( Count==1) {

//    float dx= Size.v[1] / (float) StrToInt( DrawMaterial->GetParam("Tex_W") ) ;//*310.0f;
//    float dy= Size.v[0] / (float) StrToInt( DrawMaterial->GetParam("Tex_H") ) ;//*30.0f;

    //printf("%f %f\n",dx,dy);

//    dataBuffer[3]=0;       dataBuffer[4]=dy;
//    dataBuffer[3+8]=0;             dataBuffer[4+8]=0;
//    dataBuffer[3+8+8]=dx;        dataBuffer[4+8+8]=0;
//    dataBuffer[3+8+8+8]=dx;      dataBuffer[4+8+8+8]=dy;


//    } else {
/*	float ex = (float)((Start_Anim+current_frame)%Anim_Count_X)/(float)Anim_Count_X;
	float ey = (float)((Start_Anim+current_frame)/Anim_Count_Y)/(float)Anim_Count_Y;

	float dx =  1.0f/(float)Anim_Count_X;
	float dy =  1.0f/(float)Anim_Count_Y;

//	printf("%f %f %f %f\n",ex,ey,dx,dy);

    dataBuffer[3]=ex;       dataBuffer[4]=1-(ey+dy);
    dataBuffer[3+8]=ex;             dataBuffer[4+8]=1-ey;
    dataBuffer[3+8+8]=ex+dx;        dataBuffer[4+8+8]=1-ey;
    dataBuffer[3+8+8+8]=ex+dx;      dataBuffer[4+8+8+8]=1-(ey+dy);

//    }

    return false;
    } //   if (load_all)

    return true;

};*/

void CNodeSkyCube::SetParam(const char* param_name,const char* param_value){
std::string ParamName = std::string(param_name);
std::string ParamValue = std::string(param_value);

//printf("%s %s\n",param_name,param_value);
if (ParamName=="Texture") load_all=false;

    if (ParamName=="Anim_Count_X") Anim_Count_X=StrToInt( ParamValue );
    if (ParamName=="Anim_Count_Y") Anim_Count_Y=StrToInt( ParamValue );
    if (ParamName=="Start_Anim")   {Start_Anim=StrToInt( ParamValue );current_frame=Start_Anim;}
    if (ParamName=="Speed") Speed=StrToFloat( ParamValue );
    if (ParamName=="Count")        {
                if (ParamValue=="all") { Count=Anim_Count_X*Anim_Count_Y-1; ParamValue=IntToStr(Count); }
                else Count=StrToInt( ParamValue );
                };

    if (ParamName=="Repeat") {
                Repeat=StrToInt( ParamValue );
                if (Repeat==1) Repeat=2;  // т.к. если Repeat==1 мы его удаляем при вызове Update()
                };

    if (ParamName=="Billbord") {
                if (ParamValue=="1") Billbord=true;
                else Billbord=false;
                };


    CNode::SetParam(ParamName.c_str(),ParamValue.c_str());

};
