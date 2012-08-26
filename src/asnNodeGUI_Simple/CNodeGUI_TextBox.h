#ifndef _CNODEGUI_WINDOW_H
#define _CNODEGUI_WINDOW_H

#include "CNode.h"
#include "IMaterial.h"
#include "IDrawObject.h"
#include "IFile.h"

#include <list>

class CNodeGUI_TextBox: public CNode {
public:
    CNodeGUI_TextBox(IEngine* engine);
    ~CNodeGUI_TextBox();

virtual bool LoadResource();
virtual bool Draw();
virtual char Update(float tms);

virtual void SetParam(const char* param_name,const char* param_value);

protected:

//    std::list<std::string> TextLines; // что содержится в окне
    IMaterial* DrawMaterial;
    IDrawObject *DrawMesh;
//    IFile *TextFile;
};

#endif
