#ifndef _I_ASN_GRID2D_H
#define _I_ASN_GRID2D_H

// ��������� � ��������
class ASNIGrid2D: public ASNInterface {
    public:
    const char* GetType(){ return "ASNIGrid2D"; }

//    virtual void getNodesByPos(const CVector& Start, const CVector& End, asn_ext_ptr_list* list)=0;// �������� ������ ���,

    virtual void SetCell(int cell_x, int cell_y, INode* node)=0;
    virtual INode* GetCell(int cell_x, int cell_y)=0;

    virtual bool MoveCell(int cell_xs, int cell_ys, int cell_xf, int cell_yf )=0;

// ������� ������� �� ����� ���� ��������� �����
// scr_x, scr_y - ������� ������� �� ������
    virtual bool FindMouseCell(int scr_x, int scr_y, int* cell_x, int* cell_y)=0;

    };

#endif
