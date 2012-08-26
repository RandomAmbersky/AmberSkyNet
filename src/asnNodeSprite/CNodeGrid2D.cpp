#include "IEngine.h"

#include "CNodeGrid2D.h"
#include "ASN_utils.h"

#include "ILog.h"
#include "IDevice.h"
#include "IFile.h"
#include "IFileSystem.h"

#define ENGINE Engine // we already have pointer to Engine
#include <iostream>

using namespace std;

const float QuadBuffer[]=
{
	0, 0, 0,  1, 0, 0,
	0, 0, 0,  0, 1, 0,

    1, 0, 0,  1, 1, 0,
    0, 1, 0,  1, 1, 0,

};

CNodeGrid2D::CNodeGrid2D(IEngine *_Engine):
  CNode(_Engine){

cout << "CNodeGrid2D::CNodeGrid2D1" << endl;

    SetParam("BaseClass","Node");
    SetParam("Type","NodeGrid2D");
//    SetParam("Name","-");

//cout << "CNodeGrid2D::CNodeGrid2D2" << endl;

    SetPtrParam("Node", this);

    myGrid = NULL;
    buff = NULL;
    myNodes = NULL;

//cout << "CNodeGrid2D::CNodeGrid2D3" << endl;
    NumX=10;
    NumY=10;

    SetParam("NumX", "10");

//cout << "CNodeGrid2D::CNodeGrid2D3.1" << endl;

    SetParam("NumY", "10");
    Calculate(); // for 10x10 default;

//cout << "CNodeGrid2D::CNodeGrid2D3.2" << endl;

    SetParam("ShowGrid", "0");
    SetParam("Rescale", "0");

//cout << "CNodeGrid2D::CNodeGrid2D4" << endl;
//cout << "CNodeGrid2D::CNodeGrid2D5" << endl;

};

CNodeGrid2D::~CNodeGrid2D(){
        if (buff) delete []buff;
        if (myNodes) delete []myNodes;
};

bool CNodeGrid2D::LoadResource(){

    if (load_all) return false;

//    Calculate();

    if (!myGrid) {

        myGrid=DEVICER->AddDrawObject( "-" );
        myGrid->SetDrawType(ASN_DRAW_LINES);
        myGrid->SetDataOffset(ASN_DATA_VERTEX_BUFF,0);

    };

// сколько добавлять линий
    int l_x = NumX-1;
    int l_y = NumY-1;

    if (buff) delete []buff;
    buff = new float[ (4+l_x+l_y)*6 ];
    myGrid->SetDataBuffer((void*)buff,sizeof(float)*3,8+l_x*2+l_y*2);

    memcpy(buff,QuadBuffer,4*6*sizeof(float));

// дальше чистый хардкод, чтобы выводилось без вызова Resize() да и неизвестно что в нодах будет;
buff[3] = Size.v[0];
buff[10] = Size.v[1];
buff[12] = Size.v[0];
buff[15] = Size.v[0];
buff[16] = Size.v[1];

buff[19] = Size.v[1];

buff[21] = Size.v[0];
buff[22] = Size.v[1];

int buff_p = 24; // указатель на следующую ячейку буфера

float cur_x = w_x; // текущий сдвиг от начала по X;

/// цикл по X
for (int i=0;i<l_x;i++){
// первые 3 координаты линии
buff[buff_p]=cur_x;
buff[buff_p+1]=0;
buff[buff_p+2]=0;

// вторые три координаты линии
buff[buff_p+3]=cur_x;
buff[buff_p+4]=Size.v[1];
buff[buff_p+5]=0;

cur_x+=w_x;
buff_p+=6;
};

float cur_y = w_y; // текущий сдвиг от начала по Y;

/// цикл по Y
for (int i=0;i<l_y;i++){
// первые 3 координаты линии
buff[buff_p]=0;
buff[buff_p+1]=cur_y;
buff[buff_p+2]=0;

// вторые три координаты линии
buff[buff_p+3]=Size.v[0];
buff[buff_p+4]=cur_y;
buff[buff_p+5]=0;

cur_y+=w_y;
buff_p+=6;
};

//    memset( myNodes, sizeof(INode*)*NumX*NumY, 0 );

    load_all=true;
    return false;

};

void CNodeGrid2D::Calculate(){

w_x = Size.v[0] / NumX; // ширина 1 ячейки таблицы
w_y = Size.v[1] / NumY; // высота 1 ячейки таблицы

if (myNodes) delete []myNodes;

myNodes = new INode*[NumX*NumY];
for (int i=0;i<NumY;i++)
 for (int j=0;j<NumX;j++)
   myNodes[i*NumX+j]= NULL;

};

bool CNodeGrid2D::Draw(){
    if (load_all) {
        DEVICER->PushMatrix();
        DEVICER->MultModelMatrix(NodeMatrix.m_data);

//printf( "%f %f\n",1.0f/w_x, 1.0f/w_y );

         if (show_grid) {
            DEVICER->SetMaterial(NULL);
            if (NodeColor) DEVICER->SetColor(*NodeColor);
            else DEVICER->SetColor(CVector4(1, 1, 1, 1));
            DEVICER->Draw( myGrid );
            }

            DEVICER->PopMatrix();
            CNode::Draw();

    }
    else return LoadResource();

    return false;
};

void CNodeGrid2D::SetParam(const char* param_name,const char* param_value){
    std::string ParamName = std::string(param_name);
    std::string ParamValue = std::string(param_value);

 if (ParamName=="NumX") {
        NumX = StrToInt( ParamValue );
        load_all=false;
        Calculate();
    }

 else if (ParamName=="NumY") {
        NumY = StrToInt( ParamValue );
        load_all=false;
        Calculate();
    }

 else if (ParamName=="ShowGrid") {
        show_grid = StrToInt( ParamValue );
    }

 else if ( strcmp(param_name, "Size") == 0 ) {
        load_all=false;
        Calculate();
    }

 else if ( strcmp(param_name, "Rescale") == 0 ) {
        rescale=StrToInt( ParamValue );
    };


    CNode::SetParam(ParamName.c_str(),ParamValue.c_str());

};

void CNodeGrid2D::SetCell(int cell_x, int cell_y, INode* node){

    if (cell_x<0) return;
    if (cell_y<0) return;

    if (cell_x>=NumX) return;
    if (cell_y>=NumY) return;

printf("try set cell %d %d myNodes[%d]\n", cell_x, cell_y, cell_y*NumX + cell_x );


  if ( node ) {  // если нам не прислали NULL

//    addNode( node );
    node->SetPos( CVector( w_x*cell_x, w_y*cell_y, 0 ) );

    if ( rescale ) {

        CVector node_size = node->GetSize();
        node->SetSize( CVector( Size.v[0]/NumX, Size.v[1]/NumX, node_size .v[2])  );

    };

    }

    INode* nod_in_cell = myNodes[ cell_y*NumX + cell_x ];

    if (nod_in_cell)
    {
        nod_in_cell->Release();
        printf("release..\n");
        }

    myNodes[ cell_y*NumX + cell_x ] = node;
    CNode::AddChild(node);
};

INode* CNodeGrid2D::GetCell(int cell_x, int cell_y){

    if (cell_x<0) return NULL;
    if (cell_y<0) return NULL;

    if (cell_x>=NumX) return NULL;
    if (cell_y>=NumY) return NULL;

    printf("try get %d %d\n", cell_x, cell_y);

    INode* nod_in_cell = myNodes[ cell_y*NumX + cell_x ];
    return nod_in_cell;

};

bool CNodeGrid2D::MoveCell(int cell_xs, int cell_ys, int cell_xf, int cell_yf ){

    if (cell_xf<0) return false;
    if (cell_yf<0) return false;

    if (cell_xf>=NumX) return false;
    if (cell_yf>=NumY) return false;

    INode* mCell = GetCell(cell_xs, cell_ys);

    if (!mCell) return false;

    INode *fCell = myNodes[ cell_yf*NumX + cell_xf ];
    if (fCell) fCell->Release();

    SetCell(cell_xf, cell_yf, mCell );
    myNodes[ cell_ys*NumX + cell_xs ] = NULL; // не вызываем setCell(x,y,NULL), чтобы не удалить перенесенную ноду
                                            // а просто обнуляем указатель в старом поле

};

bool CNodeGrid2D::FindMouseCell(int scr_x, int scr_y, int* cell_x, int* cell_y){

CVector p1, p2;

DEVICER->MouseToWorld(scr_x, scr_y, p1, p2);
CVector Rot = GetRot();
CVector Pos = GetPos();

// переносим две точки на наше начало
p1 = p1 - Pos;
p2 = p2 - Pos;

p1.RotateX( -Rot.v[0] );
p1.RotateY( Rot.v[1] );
p1.RotateZ( -Rot.v[2] );

p2.RotateX( -Rot.v[0] );
p2.RotateY( Rot.v[1] );
p2.RotateZ( -Rot.v[2] );

p1 = p1 + Pos;
p2 = p2 + Pos;

CVector V1, V2, V3; // 3 точки на плоскости
CVector Normal; // нормаль к плоскости

CVector Mouse_Pos; // точка пересечения прямой и плоскости

V1=Pos;
V2=Pos;
V3=Pos;

V2.v[0]+=Size.v[0];
V3.v[1]+=Size.v[1];

Normal.GetNormal(V1,V2,V3);

//p2.RotateX( -Rot.v[0] );
//p2.RotateY( Rot.v[1] );
//p2.RotateZ( -Rot.v[2] );

//p1.RotateX( -Rot.v[0] );
//p1.RotateY( Rot.v[1] );
//p1.RotateZ( -Rot.v[2] );

//Normal.RotateX( Rot.v[0] );
//Normal.RotateY( Rot.v[1] );
//Normal.RotateZ( Rot.v[2] );

//V2.RotateX( -Rot.v[0] );
//V2.RotateY( Rot.v[1] );
//V2.RotateZ( -Rot.v[2] );

//V3.RotateX( -Rot.v[0] );
//V3.RotateY( Rot.v[1] );
//V3.RotateZ( -Rot.v[2] );
CVector zero = CVector ( 0, 0, 0 );

Mouse_Pos.GetPlaneIntersect(p1,p2,V1,Normal);

// printf("%f %f %f - %f %f %f\n", p1.v[0],p1.v[1],p1.v[2],p2.v[0],p2.v[1],p2.v[2]);
printf("%f %f %f \n", Mouse_Pos.v[0], Mouse_Pos.v[1], Mouse_Pos.v[2] );

// нашли точку пересечения, теперь вычитаем из нее начало нашей сетки
Mouse_Pos=Mouse_Pos-Pos;

if ( Mouse_Pos.v[0]<0 ) return false; // точка не в гриде
if ( Mouse_Pos.v[1]<0 ) return false; // точка не в гриде
if ( Mouse_Pos.v[0]>Size.v[0] ) return false; // точка не в гриде
if ( Mouse_Pos.v[1]>Size.v[0] ) return false; // точка не в гриде

printf("%f %f %f \n", Mouse_Pos.v[0], Mouse_Pos.v[1], Mouse_Pos.v[2] );

    *cell_x = (int)( Mouse_Pos.v[0]/w_x );
    *cell_y = (int)( Mouse_Pos.v[1]/w_y );

    return true;
};

ASNInterface* CNodeGrid2D::GetInterface(const char* interface_name){

if ( strcmp(interface_name,"ASNIGrid2D") == 0 ) {
// printf("get interface %s...\n",interface_name);
 return dynamic_cast<ASNIGrid2D*>(this);//new CWorld3D();
    }
    return NULL;
};
