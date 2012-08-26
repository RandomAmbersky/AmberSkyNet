/// ����������� ����� �� XML-������

#include "EngineStarter.h"

#define RANDOM(x) (rand() % (x))

IWorld *myWorld; // ��������� �� ���
ICamera *myCam; // ��������� �� ������

bool look_in_mouse;
bool k_MouseClick;

std::string MapName;

/*
// ����� ���� ��������� ������ ����� � ���������
// ��� ������ ��� ���������
void FindMousePointer(UINT x,UINT y)
{

    CVector Start=CVector();
    CVector End=CVector();

    // ��������� ���������� ������� ����� � ������� ����
    DEVICER->MouseToWorld(x,y,Start,End);

    asn_ext_ptr_list* myList = new asn_ext_ptr_list();
    // �������� ������ ���, ������� ��� ������� "��������"
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

// Draw-�������
bool DrawCallback(void* Params)
{

    //������ ��� ���
    //myWorld->Draw();

    //myCam2->setPos(myCam->getPos()-CVector(1,1,1));

    //"�������" � ���� ������
    //myCam2->View();
    //������ ��� ���

    // "�������" � ���� ������. ��� ���� ��� ����������� �����������
    // ������� "������������" ��� � FindMousePointer

    if ( k_MouseClick /*& !look_in_mouse*/ ) {
        k_MouseClick=false;
        myCam->View();
        //FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
    }

    myWorld->Draw();

    CVector4 color = CVector4( 0, 1, 1, 1 );
    CVector4 color2 = CVector4( 1, 0, 1, 1 );

    char buf[1024];  // ����� ������ ������� :)
    sprintf(buf,"FPS: %d ",DEVICER->FPS);
    DEVICER->SetColor(color2);
    DEVICER->PrintAt(0,0,buf);

    DEVICER->SetColor(color);
    DEVICER->PrintAt(1,1,buf);


    return true;
};

// Update-�������, ���� ��������� true- ���������� ����� �� ����� Engine
bool UpdateCallback(void *Params)
{

    //printf("update\n");

    // ������ ������ �������
    int evt=EVENTER->GetInput();

    // ���� - ���� �� ��������� ��� �������
    while (evt!=ASN_EVNT_EMPTY)
    {

        if (evt==ASN_EVNT_KEYPRESSED)
        { //���� ������� - ������� �������, ��...

            // ���� �� ������ Esc - ������������� ������� ������ � true
            if (EVENTER->getLastKey()==27)
            {
                return true;
            }
/*
            if (EVENTER->getLastKey()==ASNK_F5) {
                printf("SaveWorld\n");
                std::string tmpPath=std::string(ENGINE->GetEnvParam("[Paths]DataPath"))+"World.xml";
                myWorld->SaveWorld(tmpPath.c_str());
                }

            if (EVENTER->getLastKey()==ASNK_F6) {
                printf("LoadWorld\n");
                myWorld->LoadWorld("World.xml");
                }
*/

            // ���� �� ������ PrtSrc - ������ �������� � ������ screenshotXXX
            if (EVENTER->getLastKey()==316)
            {
                DEVICER->MakeScreenshot("screenshot");
                printf("Shoot!\n");
            }

            if (EVENTER->getLastKey()==9)
            {
                look_in_mouse=!look_in_mouse;
                DEVICER->CenterMouse(!look_in_mouse);
            }


        }

        else if (evt==ASN_EVNT_MOUSEDOWN)
        {
            k_MouseClick=true;
            //FindMousePointer(DEVICER->mouse_x,DEVICER->mouse_y);
        }
        evt=EVENTER->GetInput();
    }

    return false;

};

int main(int argc, char *argv[])
{

    MapName="map.xml";

    look_in_mouse=false;
    k_MouseClick=false;

    if (!ENGINE)
    {  // ENGINE ��������� � NULL - ��������� �� ���������� :(
        fprintf(stderr,"Can't load main Engine module! Exiting...\n");
        exit (1);
    };

    printf("Welcome to AmberSkyNet ( %s %d )\n",__FILE__,__LINE__);

    //////////////// TEST //////////////////
    ENGINE->SetEnvParam("TestParam1","OK");
    printf("Get TestParam1: %s\n", ENGINE->GetEnvParam("TestParam1") );
    //////////////// TEST //////////////////

    int width = 640;
    int height = 480;

    sscanf(ENGINE->GetEnvParam("[DeviceManager]Width") ,"%d",&width);
    sscanf(ENGINE->GetEnvParam("[DeviceManager]Height") ,"%d",&height);

    int start_param=ASN_DEV_CANRESIZE;
    if ( ASN_Str( ENGINE->GetEnvParam("[DeviceManager]FullScreen")) =="1")
        start_param=start_param||ASN_DEV_FULLSCREEN;

    ///// ������ ���������� ������� ������

	for(int i = 1; i < argc; i++) {

/// �������� ������������� �����
		if(strcmp(argv[i], "-fullscreen") == 0) {
			start_param = start_param || ASN_DEV_FULLSCREEN;
			continue;
		}

/// ����� ��� �����
		if(strcmp(argv[i], "-map") == 0) {
			MapName = argv[i+1];
			continue;
		}

///  ���������� �������� �� ��, ��� �������� � ������, �������� -res 1024 768
		if(argc >= i + 2 && strcmp(argv[i], "-res") == 0) {
			width = atoi(argv[i+1]);
			height = atoi(argv[i+2]);
			continue;
		}

// �� ���� �� ����������� ������ ���...
		if(argc >= i + 2 && strcmp(argv[i], "-set") == 0) {
		    ENGINE->SetEnvParam( argv[i+1] , argv[i+2] );
			continue;
		}

		if( argv[i][0]=='-'){
		    printf("Hmm.. i don't know %s\n",argv[i]);
		    continue;
		};
	}

    // ������ �������� ����
    myWorld = (IWorld *)ENGINE->CreateObject("WorldSimple3D");
    if (!myWorld)
    {
        printf("Sorry, WorldSimple3D-plugin not found :(\n");
        return 1;
    }

    printf("World created.\n");

    /*
    myCam2 = (ICamera *)ENGINE->CreateObject("CameraSimple");
    myCam2->setPos( CVector(-93,48,126) );
    myCam2->setRotY( -32 );

    myCam = (ICamera *)ENGINE->CreateObject("CameraSimple");
    if (!myCam)
    {
        printf("Class CameraSimple can't found :(\n");
        return 1;
    }

    printf("Camera created.\n");
    */

    UINT cam_id = myWorld->addCamera("CameraSimple");

    myCam = myWorld->getCamera(cam_id);
    myCam->setScreen(0,0,1,1/*0.5*/);

    // ��������� "Camera Walker" listener
    IEventListener *myList = EVENTER->AddEventListener( "EvListCamWalker" );
    if (!myList)
    {
        printf("Class EvListCamWalker can't found :(\n");
        return 1;
    }
    myList->SetPtrParam("Camera",myCam);
    printf("Camera walker created.\n");

    // ��������� "Camera Looker" listener
    //  IEventListener*
    myList = EVENTER->AddEventListener( "EvListCamLooker" );
    if (!myList)
    {
        printf("Class EvListCamWalker can't found :(\n");
        return 1;
    }
    myList->SetPtrParam("Camera",myCam);
    printf("Camera looker created.\n");


    // ������������� ������
    myCam->setPos( CVector(-93,48,126) );
    myCam->setRotY( -32 );

    // ��������� � ��� ��� �����
    myWorld->LoadWorld( MapName.c_str() );
    printf("Map loaded.\n");

    if ( DEVICER->Init(width,height,0,start_param) )
    {
        printf("Ooops... video error!\n");
        return 0;
    }

    // ������������� 3d-����� �������
    DEVICER->SetMode( ASN_GRAPH_3D );
    DEVICER->CenterMouse(false);

    printf("Devicer [OK]\n");

    int evt;
    bool non_exit=true;

    int t=ENGINE->AddWorld(myWorld);
    printf("world num: %d\n",t);

    ENGINE->SetPtrParam("UpdateCallback",(void*)UpdateCallback);
    ENGINE->SetPtrParam("DrawCallback",(void*)DrawCallback);

    ENGINE->EngineStart();

    printf("----End----\n");

    return 0;
};
