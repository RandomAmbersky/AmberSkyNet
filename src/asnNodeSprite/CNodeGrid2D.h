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
void Calculate(); // ������� ��������� ������� ���������� (������ � ������ ������)

INode **myNodes; // ������ ���

int NumX; // ����� ����� �� �
int NumY; // ����� ����� �� Y

float w_x; // ������ 1 ������ �����
float w_y; // ������ 1 ������ �����

IDrawObject* myGrid; // ������ ���������
float *buff;         // ����� ��� ��������� ����� �����

bool show_grid;
bool rescale;
};

#endif
