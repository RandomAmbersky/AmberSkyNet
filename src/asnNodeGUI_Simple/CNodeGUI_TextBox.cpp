#include <stdlib.h>

#include "CNodeGUI_TextBox.h"

#include "IEngine.h"
#include "IDevice.h"
#include "ILog.h"
#include "IFileSystem.h"

#include "IResourceSystem.h"
#include "CTextlist.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

const float CubeBuffer[]=
{
    /// X, Y, Z      UV        NORMALS
	0, 0, 0,      0,0,       0,0,0,
 	1, 0, 0,      0,1,       1,0,0,
	1, 1, 0,      1,1,       1,1,0,
	0, 1, 0,      1,0,       0,1,0,
	0, 0, 0,      0,0,       0,0,1,
	1, 0, 0,      1,0,       1,0,1,
	1, 1, 0,      1,1,       1,1,1,
	0, 1, 0,      0,0,       0,1,1,

};


const int CubeIndex[] =
{
	0, 1, 2, 3,
	4, 5, 6, 7,
	1, 2, 6, 5,
	0, 4, 7, 3,
	1, 5, 4, 0,
	3, 7, 6, 2,
};

float line_width;  // ширина одной линии
int line_height;   // сколько строчек уместится в объекте
IDevice* myDevice; // ссылка на Device

std::string CheckLineWidth(std::string InputString){

std::string OutString="";

float str_width=0.0;

int j=0;
float x,y;

while ( j<InputString.size()){
myDevice->GetGlyphSize(InputString.at(j), x, y);
if ( (str_width+fabs(x))>line_width ) { OutString+='\n'; str_width=0; line_height--; }
OutString+=InputString.at(j);
str_width+=fabs(x);
//printf("%c %f\n",InputString.at(j),x);
j++;
};

return OutString;
//printf("----\n");
};


CNodeGUI_TextBox::CNodeGUI_TextBox(IEngine* engine):CNode(engine){

    SetParam("BaseClass","Node");
    SetParam("Type","NodeGUI_TextBox");
    SetParam("Name","-");

    DrawMaterial=NULL;
    DrawMesh=NULL;

    SetParam("Texture",ASN_NOT_FOUND_PARAM);
    };

CNodeGUI_TextBox::~CNodeGUI_TextBox(){
    if (DrawMesh) DrawMesh->Release();
};

bool CNodeGUI_TextBox::LoadResource(){

    if (load_all) return false;

    std::string TextureName;
    std::string TextFileName;

    if (!DrawMesh) {
        DrawMesh = DEVICER->AddDrawObject("-");

        DrawMesh->SetDrawType(ASN_DRAW_QUADS);
        DrawMesh->SetDataBuffer( (void *) CubeBuffer , sizeof (float)*8, 8);
        DrawMesh->SetDataOffset(ASN_DATA_VERTEX_BUFF,0);
        DrawMesh->SetDataOffset(ASN_DATA_UV_BUFF, 3*sizeof (float) );
        DrawMesh->SetDataOffset(ASN_DATA_NORMAL_BUFF, 5*sizeof (float) );
//        DrawMesh->SetDataOffset(ASN_DATA_COLOR_BUFF, 5*sizeof (float) );
        DrawMesh->SetIndexBuffer( (void *) CubeIndex , sizeof (int), 1 );
        };


        // если не задано имя файла - возврат
//   if ( (TextFileName=std::string(GetParam("FileName")))==ASN_NOT_FOUND_PARAM) return true;

//        TextFile = FILER->GetFile( TextFileName );
//        if (!TextFile) {
//                printf("%s Файла нет!\n",TextFileName.c_str());
//                return true; //файла нет такого
//                };

//        TextFile->FileOpen();
//        char *txt=(char*)TextFile->LoadInMem();

//        if (TextFile->GetState()!=ASN_FILE_DISK_IN_MEM) {
//                printf("%s файл нельзя загрузить нет!\n",TextFileName.c_str());
//                return true;
//                };

//        txt[TextFile->GetSizeInBytes()]='\0';
//        printf("File size: %d",myFile->GetSizeInBytes());


    // не задано имя текстуры - возвращаем ошибку
    if ( (TextureName=std::string(GetParam("Texture")))!=ASN_NOT_FOUND_PARAM) {

    // читаем материал с именем текстуры
    DrawMaterial=DEVICER->GetMaterial( TextureName.c_str() );
    // если такого материала нет - создаём его
    if (!DrawMaterial) {
        DrawMaterial=DEVICER->AddMaterial( TextureName.c_str() );
        DrawMaterial->SetParam("Texture", TextureName.c_str() );
        DrawMaterial->SetParam("TextureOnly", "1" ); //мы используем только текстуру
        DrawMaterial->SetParam("Texture_repeat", "1" ); //повторяем текстуру
        };

     };

    load_all=true;
    return false;
};

bool CNodeGUI_TextBox::Draw(){

CNode::Draw();

//printf("Try Draw Window...\n");

if (load_all){

    DEVICER->PushMatrix();
    DEVICER->MultModelMatrix(NodeMatrix.m_data);
    DEVICER->SetScale( Size );

//    IDrawObject *myObj = DEVICER->GetDrawObject ("_Cube");

    DEVICER->SetMaterial( DrawMaterial );
    DEVICER->Draw( DrawMesh );

    CVector TextScale=CVector(0.3,0.3,0.3);

    myDevice=DEVICER;
    line_width=Size.v[0]/TextScale.v[0];

    DEVICER->SetScale( TextScale/Size );

///// НАЧИНАЕМ ГРУБЫЙ ХАК =)   TODO

//    ITextListMap* TextlistMap=(ITextListMap *)LISTMAPPER;
    CTextlist *TextList=((CTextlistMap*)LISTMAPPER)->GetTextList( GetParam("ListName") );

//    _textlist* my_list =  LISTMAPPER->GetList( GetParam("ListName") );

    if (TextList!=NULL) {

//      LISTMAPPER->End( GetParam("ListName") );
    TextList->End();

    float x,y;
    DEVICER->GetGlyphSize('1', x, y);
    float start_point_y=Size.v[1]/TextScale[1]-y;

    line_height= abs ( (int)(Size.v[1]/TextScale[1]/y) )-1;

    std::string PrintText;

    if ( TextList->IsFirst() ) { // если указатель у нас в конце (т.е. в списке одна запись)
    PrintText= CheckLineWidth ( TextList->GetDecString() );
    } else {

    PrintText=CheckLineWidth ( TextList->GetDecString() );
    std::string TempText=""; //"временный" текст

        while ( ( !TextList->IsFirst() ) & ( line_height>0 ) )  {
        TempText=CheckLineWidth ( TextList->GetDecString() );
        PrintText=TempText+"\n"+PrintText;
        line_height--;
        };
        if (line_height>0) PrintText=CheckLineWidth ( TextList->GetDecString() )+"\n"+PrintText;

    };


    DEVICER->PrintAt(0,start_point_y,(char *) PrintText.c_str() );
    };

    DEVICER->PopMatrix();

}
else {
    LoadResource();
    return true;
    };


};

char CNodeGUI_TextBox::Update(float tms){

};

void CNodeGUI_TextBox::SetParam(const char* param_name,const char* param_value){

//    if ( strcmp(param_name, "ADDTEXT") == 0) {
//    TextLines.push( param_value );

//    }
//    if ( strcmp(param_name, "CLEARTEXT") == 0) {

//    };
//    }
//    else
CNode::SetParam(param_name, param_value);

};

