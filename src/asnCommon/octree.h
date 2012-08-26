#ifndef OCTREE_H
#define OCTREE_H

#include "CVector.h"

/*
struct COctNode{
COctNode* parent; // ��������
COctNode *child[8]; // �������� ����
CVector min;// ��������� �����
CVector max;// �������� �����
int node_level;// ������� ����
};
*/

#define MAX_POINT_VALUE 1000

class Octree {
public:
    Octree(int max_deep);
    Octree(const CVector& _min, const CVector& _max, int max_deep);
    ~Octree();

    void Init();
    void Free();
    bool Add(const CVector& _min, const CVector& _max, void* _data );

    CVector point_min;// ��������� �����
    CVector point_max;// �������� �����

    int deep;
    
    void* data;

protected:

    void MakeChilds();

    Octree *p_OctreeChild[8];

};

#endif
