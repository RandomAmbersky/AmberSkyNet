#ifndef _C_SOUND_SDL_MIXER_
#define _C_SOUND_SDL_MIXER_

#include "IEngine.h"

#include "ISound.h"

#include "ASN_Types.h"

#include "ASN_Types_internal.h"

#include "SDL/SDL_mixer.h"
#include <map>

#include "CBaseObject.h"

class CSoundSDL_mixer: public ISound, public CBaseObject {
public:

    CSoundSDL_mixer(IEngine *_Engine);
    ~CSoundSDL_mixer();

    void InitManager();
    int Release();
    void Update(float tms);

    bool PlayFX(const char *FxName);

    bool PlayMusic(const char *FxName);
    bool StopMusic();

    bool SetBaseSound(const char *BaseName, const char *FileName);

protected:
    bool SoundEnabled;
    Mix_Music *CurrentMusic;
    IEngine *Engine;
    asn_List BaseSoundList;
};

#endif
