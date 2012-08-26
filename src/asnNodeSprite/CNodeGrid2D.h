#ifndef _CNodeGrid2D_H
#define _CNodeGrid2D_H

#include "CNode.h"
#include "IDevice.h"
#include "Interfaces/ASNIGrid2D.h"

class CNodeGrid2D: public CNode, public ASNIGrid2D {
public:
    CNodeGrid2D(IEngine* engine);
    ~CNodeGrid2D();

virtual bool LoadResource();
virtual bool Draw();

virtual void SetParam(const char* param_name,const char* param_value);

// interface
ASNInterface* GetInterface(const char* interface_name);

void SetCell(int cell_x, int cell_y, INode* node);
INode* GetCell(int cell_x, int cell_y);
bool MoveCell(int cell_xs, int cell_ys, int cell_xf, int cell_yf );

bool FindMouseCell(int scr_x, int scr_y, int* cell_x, int* cell_y);

protected:
void Calculate(); // функци€ пересчета базовых параметров (ширина и высота €чейки)

INode **myNodes; // массив нод

int NumX; // число €чеек по ’
int NumY; // число €чеек по Y

float w_x; // ширина 1 €чейки сетки
float w_y; // высота 1 €чейки сетки

IDrawObject* myGrid; // объект отрисовки
float *buff;         // буфер дл€ отрисовки линий сетки

bool show_grid;
bool rescale;
};

#endif
