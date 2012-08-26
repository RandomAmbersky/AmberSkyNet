#ifndef _CNODESLTERRAIN_H
#define _CNODESLTERRAIN_H

#include "CNode.h"
#include "IDevice.h"
#include "Interfaces/ASNITerrain.h"
#include "ASN_SimpleModel.h"
#include "SLTerrainDef.h"

class CNodeSLTerrain: public CNode, public ASNITerrain {
public:
    CNodeSLTerrain(IEngine* engine);
    ~CNodeSLTerrain();

virtual bool LoadResource();
virtual bool Draw();

virtual void SetParam(const char* param_name,const char* param_value);
ASNInterface* GetInterface(const char* interface_name);

/// === ASNITerrain interface start ====================================
    virtual const float* GetHeightMap(int& width, int& height);
    virtual bool UploadHeightMap(int h_width, int h_height, float* map);
    virtual const float GetHeight(float pos_x, float pos_y);
    virtual bool SetHeight(float pos_x, float pos_y, float height);
/// ==== ASNITerrain interface end =====================================

protected:
    SLTERRAIN   *terrain;
    IMaterial *drawMaterial;
    IDrawTask   *drawTask;
    IDrawTask   *waterTask;
    ASN_SimpleIndexMesh *mesh;
    ASN_SimpleIndexMesh *meshWater;
    // Parameters
    int waterMark;
    bool showGround;
    bool showWater;
    bool useSingleColor;
    std::string textureFileName;
    unsigned int NumPointReduction;
    // Flags for redraw
    bool needUpdTerrain;
    bool needUpdTexture;
    bool needUpdWater;
    bool firstWaterDraw;
    // Calculating mesh for ground drawing
    void prepareGround(ASN_SimpleIndexMesh *mesh);
    // Calculating mesh for water drawing
    void prepareWater(ASN_SimpleIndexMesh *mesh);
    // Clear update flags
    void needUpdAll();
    // Return real height of (x, y) point
    inline float calcHeight(int rel_x, int rel_y);
    // А енту хрень крайне желательно сделать стандартной...
    // и сделали-таки :)
//    int getIntParam(const char* param_name, int default_value);
};

#endif
