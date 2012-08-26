#ifndef TERRAIN_LOD_H
#define TERRAIN_LOD_H

#include "ASN_SimpleModel.h"
#include <vector>

typedef struct {
    ASN_SM_Triangle* faces;
    int numFaces;
} sm_lod;

class ASN_SimpleTerrainLOD {
    public:

    std::vector<sm_lod> faces_lod;

    ASN_SimpleTerrainLOD();
    ~ASN_SimpleTerrainLOD();
    void Free();
    bool MakeLOD(int width, int height, float* height_map);

};
#endif
