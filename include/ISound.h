#ifndef _I_SOUND_
#define _I_SOUND_

class ISound: public IManager {
public:
    virtual ~ISound(){}

    // играем однократный эффект
    virtual bool PlayFX(const char *FxName)=0;

    // играем музыку
    virtual bool PlayMusic(const char *FxName)=0;
    virtual bool StopMusic()=0;

    // устанавливаем соответствие звука и звукового файла
    // мы можем например, сделать SetBaseSound("OgreDie","sound/death.wav")
    // а потом в программе вызывать PlayFX("OgreDie")
    virtual bool SetBaseSound(const char *BaseName, const char *FileName)=0;

};

#define SOUND ((ISound*)ENGINE->GetPtrParam("SoundManager"))

#endif
