// Программа демонстрации работы с рельефом
//

#include "EngineStarter.h"
#include "IWorld.h"
//#include "IWorld3D.h"
#include "INode.h"
//#include "ASN_utils.h"
#include "Interfaces/ASNITerrain.h"

#define RANDOM(x) (rand() % (x))

// Вынести в DEF_H
#ifndef DEF_H
#define DEF_H
const float camHeight = 1.7f; // рост носителя камеры, единицы мира
const char* camStep = "10.0f"; // шаг движения камеры, единицы мира
/* char потому, что в движке нет SetFloatParam :((( */
const float initFOV = 27.0f; // начальный угол зрения, градусы
const float FOVStep = 1.0f; // шаг изменения зума, градусы
const float waterStep = 1.0f; // шаг уровня воды при приливах и отливах
#endif
//===>

IWorld *myWorld; // указатель на мир
ICamera *myCam; // указатель на камеру

bool look_in_mouse;
bool k_MouseClick;
bool gravity_on=true;
float FOV;

std::string MapName;

// Узлы мира, являющиеся ландшафтами
struct TerrainList: public asn_ext_ptr_list {
    INode* getItem(UINT index)
    {
        return (INode*)(ptr_list[index].pointer);
    };
};

TerrainList Terrains;
std::vector<ASNITerrain*> TerrainInterfaces;

/*
// найти куда указывает курсор мышки и проверить
// что именно там находится
void FindMousePointer(UINT x,UINT y)
{

    CVector Start=CVector();
    CVector End=CVector();

    // переводим координаты курсора мышки в отрезок мира
    DEVICER->MouseToWorld(x,y,Start,End);

    asn_ext_ptr_list* myList = new asn_ext_ptr_list();
    // получаем список нод, которые наш отрезок "проткнул"
    myWorld->getNodesByPos(Start,End,myList);

    if ( myList->nums>0)
    {

        for ( UINT i=0; i<myList->nums; i++ )
        {

            if ( std::string( ((INode*)myList->ptr_list[i].pointer)->GetParam("CanDel") ) == "Yes" )
            {
            DestroyFlyingObject( (INode*)myList->ptr_list[i].pointer );
            deletePtrList(myList);
            return;
            }

            //it++;
        };
    };

    deletePtrList(myList);

};
*/

void ShowCamInfo(const CVector4& color){
    int width, height;
    float text_width, text_height, RotX, RotY;
    // Retrieve position / rotation
    CVector pos=myCam->GetPos();
    myCam->GetRot(RotX, RotY);
    char buf[1024];
    sprintf(buf,
            "FPS: %d    X:%5.1f   Y:%5.1f   Z:%5.1f   rX:%5.1f   rY:%5.1f",
            DEVICER->FPS,
            pos.v[0], pos.v[1], pos.v[2],
            RotX, RotY
    );
    if(gravity_on)
        strcat(buf, "   Gravity: ON");
    else
        strcat(buf, "   Gravity: OFF");
    // Draw on screen (not in world)
    DEVICER->StartMode2D(false);
    // Calculate position of info text
    sscanf(ENGINE->GetEnvParam("[DeviceManager]width"),"%d",&width);
    sscanf(ENGINE->GetEnvParam("[DeviceManager]height"),"%d",&height);
    DEVICER->GetTextWH(buf,text_width,text_height);
    float pos_x=(float)width-text_width-10;
    float pos_y=(float)height-text_height;
    // Draw :)
    DEVICER->SetColor(color);
    DEVICER->PrintAt(pos_x,pos_y,buf);

    DEVICER->EndMode2D();
};

// Ограничение перемещений поверхностью земл и гравитацией
void groundMoveLock()
{
    if (TerrainInterfaces.size()>0) {
        CVector pos = myCam->GetPos();
        const float groundHeight = TerrainInterfaces[0]->GetHeight(pos.v[0],pos.v[1]);
        // Высота почвы определена?
        if (!isnan(groundHeight)) {
             if ( (gravity_on) || ( pos.v[2] < (groundHeight+camHeight)) )
                 pos.v[2]=groundHeight+camHeight;
             myCam->SetPos(pos);
        }
    }
}

// Коррекция перспективы при установке/изменении FOV
void UpdatePerspective(){
    myCam->SetPerspective(FOV, (float)4.0f/3.0f, 0.1f, 1e5f);
};

// Draw-функция
int DrawCallback(void* Params)
{
    if ( k_MouseClick /*& !look_in_mouse*/ ) {
        k_MouseClick=false;
        myCam->View();
        //FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
    }
myCam->View();
   myWorld->Draw();
    ShowCamInfo(CVector4(0,0,0,1));
    return true;
};

// Update-функция, если возвратит true- произойдет выход из цикла Engine
int UpdateCallback(void *Params)
{
    groundMoveLock();
    // Read first event
    int evt=EVENTER->GetInput();

    // цикл - пока не прочитали все события
/*LOGGER->LogMsg("UpdateCallback evt cycle begin");*/
    while (evt!=ASN_EVNT_EMPTY)
    {
        if (evt==ASN_MOUSE_WHEEL_UP) {
            FOV+=FOVStep;
            UpdatePerspective();
        }
        else if (evt==ASN_MOUSE_WHEEL_DOWN) {
            FOV-=FOVStep;
            UpdatePerspective();
        }

        if (evt==ASN_EVNT_KEYPRESSED)
        { //если событие - нажатие клавиши, то...
            // если мы нажали Esc - устанавливаем признак выхода в true
            if (EVENTER->GetLastKey()==27)
            {
                return true;
            }
            else if (EVENTER->GetLastKey()==ASNK_SPACE) {  gravity_on=!(gravity_on);/*LoadMap(); ProceedCustomMap(); UploadCustomMap();*/ }

            if (EVENTER->GetLastKey()==ASNK_F5) {
                printf("SaveWorld\n");
                std::string worldFilePath=std::string(ENGINE->GetEnvParam("[Paths]DataPath"))+"World.xml";
                ENGINE->SaveDataFile(worldFilePath.c_str());
                }

            if (EVENTER->GetLastKey()==ASNK_F6) {
                printf("LoadWorld\n");
                ENGINE->LoadDataFile("World.xml");
                myWorld = ENGINE->GetWorld();
                }


            // если мы нажали PrtSrc - делаем скриншот с именем screenshotXXX
            else
            if (EVENTER->GetLastKey()==316)
            {
                DEVICER->MakeScreenshot("screenshot");
                printf("Shoot!\n");
            }
            // Нажатие табуляции - захват/освобождение мыши
            else
            if (EVENTER->GetLastKey()==9)
            {
                look_in_mouse=!look_in_mouse;
                DEVICER->CenterMouse(!look_in_mouse);
            }
            // Ирригация и мелиорация
            else
            if ((EVENTER->GetLastKey()==ASNK_KP_PLUS)||
            (EVENTER->GetLastKey()==ASNK_KP_MINUS))
            {
                //beep();
                for (UINT i=0; i<Terrains.nums; i++){
                    INode *n = Terrains.getItem(i);
                    double currentHeight = n->GetDoubleParam("WaterHeight");
                    if (EVENTER->GetLastKey()==ASNK_KP_PLUS)
                        currentHeight+=waterStep;
                    else
                        currentHeight-=waterStep;
                    n->SetParam("WaterHeight", currentHeight);
                };
            }
            // Гравитация
            else
            if (EVENTER->GetLastKey()=='g')
            {
                gravity_on = !gravity_on;
            }
        }

        else if (evt==ASN_EVNT_MOUSEDOWN)
        {
            k_MouseClick=true;
            //FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
        }
        evt=EVENTER->GetInput();
    }

    LOGGER->LogMsg("UpdateCallback evt cycle end");

    return false;

};

int main(int argc, char *argv[])
{
    // Engine init
    ENGINE_INIT(argc, argv);
    MapName="terrain_map.xml";
    look_in_mouse=false;
    k_MouseClick=false;
    if (!ENGINE)
    {
        fprintf(stderr,"Can't load main Engine module! Exiting...\n");
        return 1;
    };
    printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);
    // Load world
    FILER->AddPath("samples/TerrainTest/", false);
    ENGINE->LoadDataFile(MapName.c_str());
    myWorld = ENGINE->GetWorld();
    if (!myWorld) {
        printf("Can't get world! Exiting... :(\n");
        return 1;
    };
    printf("World loaded.\n");
    // Camera
    myCam = myWorld->GetCamera( myWorld->AddCamera("CameraSimple") );
    myCam->SetScreen(0,0,1,1/*0.5*/);
    // ?
    FOV=initFOV;
    UpdatePerspective();
    // Camera Walker listener
    IEventListener *myList = EVENTER->AddEventListener( "EvListCamWalker" );
    if (!myList)
    {
        printf("Class EvListCamWalker can't found :(\n");
        return 1;
    }
    myList->SetPtrParam("Camera",myCam);
    myList->SetParam("step", camStep);
    printf("Camera walker created.\n");
    // Camera Looker listener
    myList = EVENTER->AddEventListener( "EvListCamLooker" );
    if (!myList)
    {
        printf("Class EvListCamWalker can't found :(\n");
        return 1;
    }
    myList->SetPtrParam("Camera",myCam);
    printf("Camera looker created.\n");
    // Camera position
    myCam->SetPos( CVector(5,128,0) );
    myCam->SetRotY( 0 );
    // Search terrains
    myWorld->FindChildrenByInterface("ASNITerrain", &Terrains);
    for (int i=0; i<Terrains.nums; i++)
        TerrainInterfaces.push_back((ASNITerrain*)(Terrains.getItem(i)->GetInterface("ASNITerrain")));
    // ?
    int evt;
    bool non_exit=true;

    ENGINE->SetPtrParam("UpdateCallback",(void*)UpdateCallback);
    ENGINE->SetPtrParam("DrawCallback",(void*)DrawCallback);
    // Main loop
    ENGINE->EngineStart();
    // End of work
    clearPtrList(&Terrains);
    printf("----End----\n");

    return 0;
};
