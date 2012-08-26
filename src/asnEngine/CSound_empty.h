#include "IEngine.h"
#include "ISound.h"

class CSoundSDL_empty: public ISound {
public:

    CSoundSDL_empty(IEngine *_Engine){};
    ~CSoundSDL_empty(){};

    bool PlayFX(const char *FxName){};

    bool PlayMusic(const char *FxName){};
    bool StopMusic(){};

    bool SetBaseSound(const char *BaseName, const char *FileName){};

};
