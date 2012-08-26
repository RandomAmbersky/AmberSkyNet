#ifdef _WIN32
#include <windows.h>
#endif

#include "IAsnGenerator.h"
#include "CEngine.h"
#include "CNode3ds.h"
#include "CLog.h"
#include "CDeviceGl.h"
#include "CEventManagerSDL.h"
#include "CEvListCamWalker.h"
#include "CEvListCamLooker.h"
#include "CFileSystem.h"
#include "CFileSystemASN.h"
#include "CFileSource.h"
#include "CFileSourceZIP.h"
#include "CWorld.h"
#include "CWorldSimple3D.h"
#include "CModifierPack.h"
#include "CModifiers.h"
#include "CCamera.h"
#include "CDataParserXML.h"
#include "CNodeSprite.h"
#include "CNodeGUI_TextBox.h"
#include "CSoundSDL_mixer.h"
#include "CNodeSLTerrain.h"
#include "CNodeRuler.h"
#include "CScriptManagerLua.h"
//#include "CPhysicManager.h"


class CLoader: public IAsnGenerator {
public:
    CLoader() {

    GeneratorInfo.name="AmberSkyNet Engine";
    GeneratorInfo.type="Engine,Node3ds,NodeBase,LogTxt,LogNo,DeviceGl,EventManagerSDL,EvListCamWalker,EvListCamLooker,CameraSimple,FileSystem,FileSystemASN,FSDisk,FSZIP,WorldSimple3D,World,ModifierPack,NodeModifierRotate,DataParserXML,NodeSprite,NodeGUI_TextBox,SoundSDL_mixer,NodeRuler,NodeSLTerrain,ScriptManagerLua";
//    GeneratorInfo.type=GeneratorInfo.type+",PhysicManager";
    GeneratorInfo.author="R.Ambersky";
    GeneratorInfo.description="Main Engine Module and all plugins in one pack. Enjoy :)";
    GeneratorInfo.plugin_version="0.8";
    GeneratorInfo.required_version="0.8";
      };

    ~CLoader() { };
    void *GetClass(const std::string& Type){

    if (Type=="NodeBase") return new CNode(Engine);
    if (Type=="Node3ds") return new CNode3ds(Engine);
    if (Type=="LogTxt") return new CLog(Engine);
    if (Type=="LogNo") return new CLogNo(Engine);

    if (Type=="DeviceGl") return new CDeviceGl(Engine);
    if (Type=="CameraSimple") return new CCamera(Engine);
    if (Type=="EventManagerSDL") return new CEventManagerSDL(Engine);
    if (Type=="EvListCamWalker") return new CEvListCamWalker(Engine);
    if (Type=="EvListCamLooker") return new CEvListCamLooker(Engine);

    if (Type=="FileSystem") return new CFileSystem(Engine);
    if (Type=="FileSystemASN") return new CFileSystemASN(Engine);
    if (Type=="FSDisk") return new CFileSource(Engine);
    if (Type=="FSZIP") return new CFileSourceZIP(Engine);

    if (Type=="World") return new CWorld(Engine);
    if (Type=="WorldSimple3D") return new CWorldSimple3D(Engine);

    if (Type=="ModifierPack") return new CModifierPack(Engine);
    if (Type=="NodeModifierRotate") return new CModifRotate(Engine);

    if (Type=="DataParserXML") return new CDataParserXML(Engine);

    if (Type=="NodeSprite") return new CNodeSprite(Engine);
    if (Type=="SoundSDL_mixer") return new CSoundSDL_mixer(Engine);
    if (Type=="NodeGUI_TextBox") return new CNodeGUI_TextBox(Engine);
    if (Type=="NodeRuler") return new CNodeRuler(Engine);
    if (Type=="NodeSLTerrain") return new CNodeSLTerrain(Engine);

    if (Type=="ScriptManagerLua") return new CScriptManagerLua(Engine);

//    if (Type=="PhysicManager") return new CPhysicManager(Engine);
//    if (Type=="Engine") return new CEngine();
//    else

    return NULL;

    };

};

/*
extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void GetPlugin( IAsnGenerator** ppOutput )
{
    *ppOutput = new CLoader();
};

*/

// экспортим функцию создания класса Engine
extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void GetEngine( IEngine** ppOutput )
{
    *ppOutput = new CEngine();
};

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void GetPlugin( IAsnGenerator** ppOutput )
{
     *ppOutput = new CLoader();
};

