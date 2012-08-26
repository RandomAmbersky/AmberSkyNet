#ifndef _CNODEHEIGHTMAP_H
#define _CNODEHEIGHTMAP_H

#include "CNode.h"
#include "IDevice.h"
#include "ASN_SimpleModel.h"
#include "Interfaces/ASNITerrain.h"

#include "lod_terrain.h"

class CNodeHeightMap: public CNode, public ASNITerrain {
public:
    CNodeHeightMap(IEngine* engine);
    ~CNodeHeightMap();

bool LoadResource();
bool Draw();

ASNInterface* GetInterface(const char* interface_name);
void SetParam(const char* param_name,const char* param_value);

/// интерфейс Terran'а
    virtual const float* GetHeightMap(int& width, int& height);
    virtual bool UploadHeightMap(int h_width, int h_height, float* map);
    virtual const float GetHeight(float pos_x, float pos_y);
    virtual bool SetHeight(float pos_x, float pos_y, float height);

protected:

    void CalculateLOD();

    IDrawObject *DrawTerrain;
    IMaterial *DrawMaterial;
    ASN_SimpleIndexMesh terramesh;

    bool update_texture;
    bool update_mesh;

    float detail;

    int width;
    int height;
    float* HeightMap;

    //Octree* map_octree;
    ASN_SimpleTerrainLOD* lod_terrain;

};

#endif
