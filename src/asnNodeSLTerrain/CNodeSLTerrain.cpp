#include "IEngine.h"

#include "CNodeSLTerrain.h"
#include "GL/gl.h"
#include "ASN_utils.h"

#include "ILog.h"
#include "IFile.h"
#include "IFileSystem.h"

#define ENGINE Engine // we already have pointer to Engine

/*// tEmPoRaRy UGLY TRICK: (~~)
// this declaration MUST BE in include files! (~~)
extern void xyz_to_uv_sphere( float uv[2], const float vertex[3], const float normal[3] );*/

CNodeSLTerrain::CNodeSLTerrain(IEngine *_Engine):
 CNode(_Engine)
{
    drawMaterial = NULL;
    drawTask = new IDrawTask;
    drawTask->drawObject=DEVICER->AddDrawObject("-");
    waterTask = new IDrawTask;
    waterTask->drawObject=DEVICER->AddDrawObject("-");
    mesh = new ASN_SimpleIndexMesh;
    meshWater = new ASN_SimpleIndexMesh;

    SetParam("BaseClass","Node");
    SetParam("Type","NodeSLTerrain");
    SetParam("Name","-");
    needUpdAll();
    firstWaterDraw = true;

//printf("CNodeSLTerrain created\n");

};

CNodeSLTerrain::~CNodeSLTerrain(){
    delete drawTask;
    delete waterTask;
    delete mesh;
    delete meshWater;
};

bool CNodeSLTerrain::LoadResource(){
    printf("\nCNodeSLTerrain::LoadResource begin\n");
    char err[MAX_PATH+64];
    if (load_all) return false; // work done

    showGround = GetIntParam("ShowGround", 1);
    showWater  = GetIntParam("ShowWater",  1);
    NumPointReduction = GetIntParam("NumPointReduction",  1);
    if ((NumPointReduction<1)||(NumPointReduction>MAXTERRAIN/2)) {
        sprintf(err, "Error! Bad NumPointReduction value");
        LOGGER->ErrMsg(0, err);
        return true;
    };
    BYTE waterMarkOfSim = 0;
    if (showGround) {
        if (needUpdTerrain) {
            // Load terrain file
            std::string terrainFileName=std::string(GetParam("TerrainFileName"));
            if(terrainFileName==ASN_NOT_FOUND_PARAM) return true;

            IFile *terFile=FILER->GetFile(terrainFileName.c_str());
            if (!terFile) {
                sprintf(err, "Error! Missing file %s", terrainFileName.c_str());
                LOGGER->ErrMsg(0, err);
                return true;
            };
            terrain=(SLTERRAIN*)terFile->LoadInMem();
            if (terFile->GetState()!=ASN_FILE_DISK_IN_MEM) return true;
            if (!terrain) {
                sprintf(err, "Error! Error reading or empty file %s",
                    terrainFileName.c_str());
                LOGGER->ErrMsg(0, err);
                return true;
            };
            if(terFile->GetSizeInBytes()!=sizeof(SLTERRAIN)) {
                sprintf(err, "Error! Invalid format of file %s",
                    terrainFileName.c_str());
                LOGGER->ErrMsg(0, err);
                return true;
            };
printf("\nCNodeSLTerrain::LoadResource::NumPointReduction=%d\n", NumPointReduction);
            // Model Creating
            prepareGround(mesh);
printf("\nCNodeSLTerrain::prepareGround::NumPointReduction=%d\n", NumPointReduction);
            mesh->SetPointerIn(drawTask->drawObject);
            // Water level
            waterMarkOfSim = (*terrain)[0][0].waterHeightOfSim;

            needUpdTerrain = false;
        };
        if (needUpdTexture) {
            // Load texture
            useSingleColor = false;
            textureFileName=std::string(GetParam("TextureFileName"));
            if(textureFileName==ASN_NOT_FOUND_PARAM)
                textureFileName="grass.jpg";
            if(!FILER->GetFile(textureFileName.c_str()))
                useSingleColor = true;
            // Material for terrain
            drawMaterial=DEVICER->GetMaterial(GROUNDMATERIALNAME);
            // if material still not created - create it
            if (!drawMaterial) {
                drawMaterial=DEVICER->AddMaterial(GROUNDMATERIALNAME);
                if (!drawMaterial) {
                    sprintf(err, "Error! Can't create material %s",
                        GROUNDMATERIALNAME);
                    LOGGER->ErrMsg(2, err);
                    return true;
                }
                if (useSingleColor) { // texture not found...
                    drawMaterial->SetParam("Ambient",  "0 0.5 0");
                    drawMaterial->SetParam("Diffuse",  "0 0.75 0");
                    drawMaterial->SetParam("Specular",  "0 1 0");
                }else{
                    drawMaterial->SetParam("Texture", textureFileName.c_str());
                    drawMaterial->SetParam("TextureOnly", "1" ); // in future change
                    drawMaterial->SetParam("Texture_repeat", "1" );
                };
            };
            needUpdTexture = false;
        };
    };
    // Water Height
    if (showWater && needUpdWater) {
        int oldWaterMark = waterMark;
        printf("mark: %d",oldWaterMark);
        waterMark = GetIntParam("WaterHeight",
            waterMarkOfSim ? waterMarkOfSim : DEFAULTWATERMARK);
        if (firstWaterDraw) {
            // Model Creating
            prepareWater(meshWater);
            meshWater->SetPointerIn(waterTask->drawObject);
            firstWaterDraw = false;
        }
        else
            meshWater->Move(0, 0, waterMark - oldWaterMark);

        needUpdWater = false;
    };
    // Done
    load_all=true;
    return false;
};

bool CNodeSLTerrain::Draw()
{
//    printf("\nCNodeSLTerrain::Draw begin\n");
/*    if (!load_all) {
        LoadResource();
        if (!load_all) return true;
    };*/
    if (load_all) {
        DEVICER->PushMatrix();
        DEVICER->MultModelMatrix(NodeMatrix.m_data);
        if (showGround) {
            DEVICER->SetMaterial( drawMaterial );
            DEVICER->Draw( drawTask->drawObject );
        };
        if (showWater) {
            DEVICER->SetMaterial(NULL);
            DEVICER->SetColor(CVector4(0, 0, 0.4, 0.75));
            DEVICER->Draw( waterTask->drawObject );
        };
        DEVICER->PopMatrix();
    }
    else return LoadResource();
    //printf("\nCNodeSLTerrain::Draw end\n");
    return false;
};

void CNodeSLTerrain::SetParam(const char* param_name,const char* param_value)
{
    std::string ParamName = std::string(param_name);
    std::string ParamValue = std::string(param_value);
    if (ParamName=="WaterHeight")
        needUpdWater = true;
    else
    if ((ParamName=="TerrainFileName")
    ||(ParamName=="NumPointReduction")
    ||(ParamName=="NumPointReduction"))
        needUpdTerrain = true;
    else
    if (ParamName=="TextureFileName")
        needUpdTexture = true;
    else
        needUpdAll();

    load_all = load_all && !needUpdTerrain && !needUpdTexture && !needUpdWater;
    CNode::SetParam(ParamName.c_str(),ParamValue.c_str());
};

// ASNInterface support members
ASNInterface* CNodeSLTerrain::GetInterface(const char* interface_name)
{
    if (strcmp(interface_name,"ASNITerrain")==0)
        return (ASNITerrain*)this;
    else
    {
        LOGGER->ErrMsgStr(2,std::string("CNodeSLTerrain don't support ")+
            interface_name);
        return NULL;
    }
};

const float* CNodeSLTerrain::GetHeightMap(int& width, int& height)
{
    if (!terrain) {
        width=height=0;
        return NULL;
    }
    else
    {



        return NULL; // Сделать по-нормальному!!!


    };
};

bool CNodeSLTerrain::UploadHeightMap(int h_width, int h_height, float* map)
{
    // Сделать по-нормальному!!!
}

const float CNodeSLTerrain::GetHeight(float pos_x, float pos_y)
{
    uint rel_x = (uint)pos_x;
    uint rel_y = (uint)pos_y;
    if ((terrain)&&(rel_x<MAXTERRAIN)&&(rel_y<MAXTERRAIN))
        return calcHeight(rel_x, rel_y);
    else
        return NAN;
}

bool CNodeSLTerrain::SetHeight(float pos_x, float pos_y, float height)
{
    // Сделать по-нормальному!!!
}


// Calculating mesh for ground drawing
void CNodeSLTerrain::prepareGround(ASN_SimpleIndexMesh *mesh)
{
    // Mesh filling
    int actualNumPointX = MAXTERRAIN/NumPointReduction;
    int actualNumPointY = MAXTERRAIN/NumPointReduction;

    mesh->SetVertexNum(actualNumPointX*actualNumPointY);
    // Vertices
    uint i, j, index, distancex, distancey;
    CVector normal();
#ifdef LOGCOORDINATES
    FILE* clog=fopen("coordinates.log", "w");
    if (clog) fprintf(clog, "x\ty\tz\theight\n");
#endif
    for(i=0, index=0, distancex=0; i<actualNumPointX; i++)
    {
        for(j=0, distancey=0; j<actualNumPointY; j++, index++) {
            ASN_SM_Vertex* vertice = &(mesh->vertices[index]);
            // Coordinates
            vertice->pos.v[0] = distancex;
            vertice->pos.v[1] = distancey;
            vertice->pos.v[2] =
             calcHeight(i*NumPointReduction, j*NumPointReduction);
#ifdef LOGCOORDINATES
            if (clog)
                fprintf(clog, "%3d\t%3d\t%3d\t%f\n", distancex, distancey,
                (*terrain)[i][j].heightField, vertice->pos.v[2]);
#endif
            distancey += METERSPERPIXEL*NumPointReduction;
        };
        distancex += METERSPERPIXEL*NumPointReduction;
    };
#ifdef LOGCOORDINATES
    if (clog) fclose(clog);
#endif
    // There is 2 cycles instead 1 because pos for all points
    // must be completed before normales calculation
    for(i=0, index=0; i<actualNumPointX; i++)
    {
        for(j=0; j<actualNumPointY; j++, index++) {
            ASN_SM_Vertex* vertice = &(mesh->vertices[index]);
            ASN_SM_Vertex* vertice2;
            ASN_SM_Vertex* vertice3;
            // Normales
            // use two nearby points
            if (i+1<actualNumPointX)
                vertice2 = &(mesh->vertices[index+actualNumPointX]);
            else
                vertice2 = &(mesh->vertices[index-actualNumPointX]);
            if (j+1<actualNumPointY)
                vertice3 = &(mesh->vertices[index+1]);
            else
                vertice3 = &(mesh->vertices[index-1]);
            CVector norm;
            CVector v1 = CVector(vertice->pos.v[0], vertice->pos.v[1], vertice->pos.v[2]);
            CVector v2 = CVector(vertice2->pos.v[0], vertice2->pos.v[1], vertice2->pos.v[2]);
            CVector v3 = CVector(vertice3->pos.v[0], vertice3->pos.v[1], vertice3->pos.v[2]);
            norm.GetNormal(v1, v2, v3);
            vertice->n.v[0]=norm.v[0];
            vertice->n.v[1]=norm.v[1];
            vertice->n.v[2]=-norm.v[2];
            // Texture coordinates
            /*xyz_to_uv_sphere(vertice->tex.v, vertice->pos.v, vertice->n.v );*/
            vertice->tex.v[0]=double(i)/actualNumPointX;
            vertice->tex.v[1]=double(j)/actualNumPointY;
        };
    };
    // Indexes
    long triangleCount = 2*(actualNumPointX-1)*(actualNumPointY-1);
    mesh->SetIndexNum(triangleCount);
    uint baseIndex=0;
    for(i=0, index=0; i<actualNumPointX-1; i++)
    {
        for(j=0; j<actualNumPointY-1; j++, baseIndex++) {
            // 0, 2...
            int* indexes = mesh->faces[index++].index;
            indexes[0]=baseIndex;
            indexes[1]=baseIndex+1;
            indexes[2]=baseIndex+actualNumPointX;
            // 1, 3...
            indexes = mesh->faces[index++].index;
            indexes[0]=baseIndex+1;
            indexes[1]=baseIndex+actualNumPointX;
            indexes[2]=baseIndex+actualNumPointX+1;
        };
        baseIndex++;
    };
};

// Calculating mesh for water drawing
void CNodeSLTerrain::prepareWater(ASN_SimpleIndexMesh *mesh)
{
    mesh->SetVertexNum(4);
    mesh->vertices[0].pos.v[0]=0;
    mesh->vertices[0].pos.v[1]=0;
    mesh->vertices[0].pos.v[2]=waterMark;
    mesh->vertices[1].pos.v[0]=0;
    mesh->vertices[1].pos.v[1]=MAXTERRAINPOINT;
    mesh->vertices[1].pos.v[2]=waterMark;
    mesh->vertices[2].pos.v[0]=MAXTERRAINPOINT;
    mesh->vertices[2].pos.v[1]=MAXTERRAINPOINT;
    mesh->vertices[2].pos.v[2]=waterMark;
    mesh->vertices[3].pos.v[0]=MAXTERRAINPOINT;
    mesh->vertices[3].pos.v[1]=0;
    mesh->vertices[3].pos.v[2]=waterMark;
    mesh->SetIndexNum(2);
    mesh->faces[0].index[0]=0;
    mesh->faces[0].index[1]=1;
    mesh->faces[0].index[2]=2;
    mesh->faces[1].index[0]=0;
    mesh->faces[1].index[1]=3;
    mesh->faces[1].index[2]=2;
};

void CNodeSLTerrain::needUpdAll()
{
    needUpdTerrain = true;
    needUpdTexture = true;
    needUpdWater = true;
}
;

inline float CNodeSLTerrain::calcHeight(int rel_x, int rel_y){
    return METERSPERPIXEL*(*terrain)[rel_x][rel_y].heightField*
          ((float)((*terrain)[rel_x][rel_y].heightMultiplyFactor)/127);
}

/* реализовано в BaseObject
int CNodeSLTerrain::getIntParam(const char* param_name, int default_value)
{
    std::string buf(GetParam(param_name));
    if (buf==ASN_NOT_FOUND_PARAM)
        return default_value;
    else
        return StrToInt(buf);
};
*/
