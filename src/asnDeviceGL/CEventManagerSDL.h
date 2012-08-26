#ifndef _IEVENTMANAGERSDL_H
#define _IEVENTMANAGERSDL_H

#include "IEventManager.h"
#include "IEngine.h"
#include <list>

// мы работаем в паре с CDeviceGL, т.к.
// в SDL объединены окно отрисовки и события
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

    int GetInput(); //  Listener'ы перехватывают сообщения
                    //  и после обработки выдают сообщения

    IEventListener* AddEventListener( const char *ListenerType );
    bool AddCustomEventListener( IEventListener *listener );
    bool DelEventListener( UID listenID );

    int GetLastKey(){return LastKey;};
    UINT16 GetUnicodeKey(){return UnicodeKey;};
    unsigned short GetKeyModifier(){return KeyModifier;};

protected:

    int LastKey;
    UINT16 UnicodeKey;			/* translated character */
    unsigned short KeyModifier;         /* Alt, Ctrl, итп */

    int GetInputInternal(); // просто читаем события из очереди
    Listeners_map Listeners;
//    std::list<IEventListener*> Listeners;
//    IEngine *Engine;
    CDeviceGl *device;

};


#endif
