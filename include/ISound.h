#ifndef _I_SOUND_
#define _I_SOUND_

class ISound: public IManager {
public:
    virtual ~ISound(){}

    // ������ ����������� ������
    virtual bool PlayFX(const char *FxName)=0;

    // ������ ������
    virtual bool PlayMusic(const char *FxName)=0;
    virtual bool StopMusic()=0;

    // ������������� ������������ ����� � ��������� �����
    // �� ����� ��������, ������� SetBaseSound("OgreDie","sound/death.wav")
    // � ����� � ��������� �������� PlayFX("OgreDie")
    virtual bool SetBaseSound(const char *BaseName, const char *FileName)=0;

};

#define SOUND ((ISound*)ENGINE->GetPtrParam("SoundManager"))

#endif
