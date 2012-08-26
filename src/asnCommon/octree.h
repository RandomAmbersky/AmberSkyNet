#ifndef OCTREE_H
#define OCTREE_H

#include "CVector.h"

/*
struct COctNode{
COctNode* parent; // родитель
COctNode *child[8]; // дочерние ноды
CVector min;// стартовая точка
CVector max;// конечная точка
int node_level;// уровень ноды
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

    CVector point_min;// стартовая точка
    CVector point_max;// конечная точка

    int deep;
    
    void* data;

protected:

    void MakeChilds();

    Octree *p_OctreeChild[8];

};

#endif
