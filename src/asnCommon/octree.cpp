#include "octree.h"

Octree::Octree(int max_deep)
{

    deep = max_deep;

    Init();

}

Octree::Octree(const CVector& _min, const CVector& _max, int max_deep){

    Init();
    point_min = _min;
    point_max = _max;
    
};


Octree::~Octree()
{
    Free();
};

void Octree::Init()
{

    point_min = CVector( 1000, 1000, 1000);
    point_max = CVector( -1000, -1000, -1000);

    for (int i=0;i<8;i++)
        p_OctreeChild[i] = NULL;

};


void Octree::Free()
{

    for (int i=0;i<8;i++)
    {
        if (p_OctreeChild[i])
        {
            delete p_OctreeChild[i];
            p_OctreeChild[i] = NULL;
        }
    };

};

// false - нельзя добавить
bool Octree::Add(const CVector& _min, const CVector& _max, void* _data ){

    if ( ( point_min > _min ) || ( point_max < _max ) ) return false;

    if (deep) {
    
        if ( p_OctreeChild[0]==NULL ) MakeChilds();

        if ( p_OctreeChild[0]->Add(_min,_max,data) ) return true;
        if ( p_OctreeChild[1]->Add(_min,_max,data) ) return true;
        if ( p_OctreeChild[2]->Add(_min,_max,data) ) return true;        
        if ( p_OctreeChild[3]->Add(_min,_max,data) ) return true;
        if ( p_OctreeChild[4]->Add(_min,_max,data) ) return true;
        if ( p_OctreeChild[5]->Add(_min,_max,data) ) return true;
        if ( p_OctreeChild[6]->Add(_min,_max,data) ) return true;
        if ( p_OctreeChild[7]->Add(_min,_max,data) ) return true;
        
    }
        
    data = _data;

};

void Octree::MakeChilds(){

CVector dividor=point_max-point_min;
dividor=dividor/2.0f;

CVector tmp_vec=CVector(point_min.v[0], point_min.v[1]+dividor.v[1],point_min.v[2]);

p_OctreeChild[0]=new Octree(tmp_vec,tmp_vec+dividor, deep-1);

tmp_vec=CVector(point_min.v[0]+dividor.v[0],point_min.v[1]+dividor.v[1],point_min.v[2]);

p_OctreeChild[1]=new Octree(tmp_vec,tmp_vec+dividor, deep-1);

p_OctreeChild[2]=new Octree(point_min,point_min+dividor, deep-1);

tmp_vec=CVector(point_min.v[0]+dividor.v[0],point_min.v[1],point_min.v[2]);

p_OctreeChild[3]=new Octree(tmp_vec,tmp_vec+dividor, deep-1);

tmp_vec=CVector(point_min.v[0],point_min.v[1]+dividor.v[1],point_min.v[2]+dividor.v[2]);

p_OctreeChild[4]=new Octree(tmp_vec,tmp_vec+dividor, deep-1);

tmp_vec=CVector(point_min.v[0]+dividor.v[0],point_min.v[1]+dividor.v[1],point_min.v[2]+dividor.v[2]);

p_OctreeChild[5]=new Octree(tmp_vec,tmp_vec+dividor, deep-1);

tmp_vec=CVector(point_min.v[0],point_min.v[1],point_min.v[2]+dividor.v[2]);

p_OctreeChild[6]=new Octree(tmp_vec,tmp_vec+dividor, deep-1);

tmp_vec=CVector(point_min.v[0]+dividor.v[0],point_min.v[1],point_min.v[2]+dividor.v[2]);

p_OctreeChild[7]=new Octree(tmp_vec,tmp_vec+dividor, deep-1);

};
