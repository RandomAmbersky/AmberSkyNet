#include "CSoundSDL_mixer.h"
#include "SDL/SDL.h"

#define ENGINE Engine //у нас уже есть указатель на Engine
#include "ILog.h"
#include "IFileSystem.h"
//#include "IFile.h"

//extern DECLSPEC Mix_Music * SDLCALL Mix_LoadMUS_RW(SDL_RWops *rw);


CSoundSDL_mixer::CSoundSDL_mixer(IEngine *_Engine):CBaseObject(_Engine){

    SetParam("BaseClass","Sound");
    SetParam("Type","SoundSDL_mixer");

    SoundEnabled = true;
    Engine=_Engine;
    CurrentMusic=NULL;

LOGGER->LogMsg("+CSoundSystem"); //менеджер файловой системы создан

  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
  int audio_channels = 2;
  int audio_buffers = 4096;

  if ( SDL_InitSubSystem(SDL_INIT_AUDIO) < 0 ) {
        LOGGER->ErrMsgStr(2, std::string("Couldn't initialize SDL: %s\n")+ std::string(SDL_GetError()) );
        SoundEnabled = false;
        }
  else {
        if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
              LOGGER->ErrMsg(2,"Unable to open audio!\n");
              SoundEnabled = false;
//              exit(1);
              } else {
              Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
              };
       };

};

CSoundSDL_mixer::~CSoundSDL_mixer(){
    StopMusic();
    LOGGER->LogMsg("-CSoundSystem"); //менеджер файловой системы создан
};

void CSoundSDL_mixer::InitManager(){

if ( strcmp(ENGINE->GetEnvParam("[SoundManager]SoundEnabled"),"0")==0) SoundEnabled=false;

};

int CSoundSDL_mixer::Release(){
    printf("CSoundSystem released...\n");
    delete this;
    return 0;
};

void CSoundSDL_mixer::Update(float tms){

};

bool CSoundSDL_mixer::PlayFX(const char *FxName){
if (!SoundEnabled) return true;

    asn_List::iterator _it;
    std::string FileName;
    // если в списке базовых звуков такой есть - берём его вместо имени
    _it=BaseSoundList.find(FxName);
    if ( _it!=BaseSoundList.end() ) FileName=(*_it).second;
    // если нет - то считаем что это нам задали имя файл
    else FileName=FxName;

//    FileName=std::string( ENGINE->GetEnvParam("[Paths]DataPath") )+FileName;

    IFile *file = FILER->GetFile( FileName.c_str() );
    if (!file) return true;
    file->FileOpen();
    const char *data=(const char *)file->LoadInMem();
    if (file->GetState()!=ASN_FILE_DISK_IN_MEM) return true;

//    printf("Try play %s\n",FileName.c_str());

	Mix_Chunk *tmp = Mix_LoadWAV_RW(SDL_RWFromMem( (void*) data,file->GetSizeInBytes()),1);
//	Mix_Chunk *tmp = Mix_LoadWAV(FileName.c_str());
//	Mix_Chunk *tmp = Mix_QuickLoad_WAV( (Uint8*) data /*, file->GetSizeInBytes()/8*/ );
	if (tmp) {
		Mix_PlayChannel(-1, tmp, 0);
		return false;
		  };
//  myFile->FileClose();
  return true;
};

bool CSoundSDL_mixer::PlayMusic(const char *FxName){
if (!SoundEnabled) return true;

    asn_List::iterator _it;
    std::string FileName;
    // если в списке базовых звуков такой есть - берём его вместо имени
    _it=BaseSoundList.find(FxName);
    if ( _it!=BaseSoundList.end() ) FileName=(*_it).second;
    // если нет - то считаем что это нам задали имя файл
    else FileName=FxName;

    // если музыка уже играет - останавливаем её
    if(CurrentMusic) StopMusic();

    FileName= std::string( "./" ) + std::string( ENGINE->GetEnvParam("[Paths]DataPath") )+FileName;
		LOGGER->LogMsg( ("Try play "+FileName).c_str() );
//    IFile *file = FILER->GetFile( FileName.c_str() );
//    if (!file) return true;
//    file->FileOpen();
//    const char *data=(const char *)file->LoadInMem();
//    if (file->GetState()!=ASN_FILE_DISK_IN_MEM) return true;

// пробуем загрузить файл с музыкой

// почему-то закомментарен этот кусочек в SDL_mixer, пришлось раскомментарить :D
//	CurrentMusic = Mix_LoadMUS_RW ( SDL_RWFromMem( (void*) data,file->GetSizeInBytes()) );

	CurrentMusic = Mix_LoadMUS(FileName.c_str());
	if (CurrentMusic) {
		Mix_PlayMusic(CurrentMusic, -1);
		LOGGER->LogMsg( ("Try play "+FileName).c_str() );
//		printf("Try play %s\n",FileName.c_str());
		return false;
			  }
    else {
        LOGGER->ErrMsgStr(2, std::string("Couldn't play ")+ FileName + " : "+std::string(SDL_GetError()) );
	return true; //че-то не сработало :(
    }
};

bool CSoundSDL_mixer::StopMusic(){
if (!SoundEnabled) return true;
    	Mix_HaltMusic();
    	Mix_FreeMusic(CurrentMusic);
    	CurrentMusic=NULL;
};

bool CSoundSDL_mixer::SetBaseSound(const char *BaseName, const char *FileName){
    BaseSoundList[BaseName]=FileName;
    return false;
};

