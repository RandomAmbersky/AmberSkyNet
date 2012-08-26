#ifndef _IEVENTMANAGERSDL_H
#define _IEVENTMANAGERSDL_H

#include "IEventManager.h"
#include "IEngine.h"
#include <list>

// �� �������� � ���� � CDeviceGL, �.�.
// � SDL ���������� ���� ��������� � �������
#include "CDeviceGl.h"
#include "CBaseObject.h"

typedef std::map<UID,IEventListener*> Listeners_map;

class CEventManagerSDL: public IEventManager, public CBaseObject {
public:

    CEventManagerSDL(IEngine *engine);
    virtual ~CEventManagerSDL();

    void InitManager();
    int Release();
    void Update(float tms);

    int GetInput(); //  Listener'� ������������� ���������
                    //  � ����� ��������� ������ ���������

    IEventListener* AddEventListener( const char *ListenerType );
    bool AddCustomEventListener( IEventListener *listener );
    bool DelEventListener( UID listenID );

    int GetLastKey(){return LastKey;};
    UINT16 GetUnicodeKey(){return UnicodeKey;};
    unsigned short GetKeyModifier(){return KeyModifier;};

protected:

    int LastKey;
    UINT16 UnicodeKey;			/* translated character */
    unsigned short KeyModifier;         /* Alt, Ctrl, ��� */

    int GetInputInternal(); // ������ ������ ������� �� �������
    Listeners_map Listeners;
//    std::list<IEventListener*> Listeners;
//    IEngine *Engine;
    CDeviceGl *device;

};


#endif
