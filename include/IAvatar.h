#ifndef _IAVATAR_H
#define _IAVATAR_H

#include "INode.h"

class IAvatar: public virtual INode {

public:

// One step in direction, defined in INode::setRot(const CVector& Rot)
virtual bool step();
// Speak string to avatar receiver, or in common chat, if receiver is NULL
virtual bool speakText(std::string text, IAvatar* receiver = 0);
// Speak by voice to avatar receiver, or in common chat, if receiver is NULL
//virtual bool speakVoice(??? sound, IAvatar* receiver = 0, bool backgroundMode = true);
// setParam must receive bool parameter "FlyMode"
};


#endif
