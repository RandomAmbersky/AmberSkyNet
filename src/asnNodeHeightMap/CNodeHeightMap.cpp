#include "IEngine.h"

#include "CNodeHeightMap.h"
//#include "GL/gl.h"
#include "ASN_utils.h"

#include "ILog.h"
#include "IFile.h"
#include "IFileSystem.h"

#include <math.h>

#define ENGINE Engine // we already have pointer to Engine

using namespace std;

CNodeHeightMap::CNodeHeightMap(IEngine *_Engine):
 CNode(_Engine)
{

    DrawTerrain = NULL;
    DrawMaterial = NULL;

    HeightMap = NULL;
    width=0;
    height=0;

    update_texture = true;
    update_mesh = true;

    SetParam("BaseClass","Node");
    SetParam("Type","NodeHeightMap");
    SetParam("Name","-");

    SetParam("SizeFromHeightMap","0");
    SetParam("Detail","1");

    lod_terrain = new ASN_SimpleTerrainLOD();

    //map_octree = new Octree(10);

};

CNodeHeightMap::~CNodeHeightMap(){
    printf("-CNodeHeightMap");
    if (HeightMap) free(HeightMap);
    terramesh.Free();
//    lod_terrain->Free();
    delete lod_terrain;
};

#undef	CLAMP
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

float clamp( float x, float low, float high)
{
    if (x > high)
        return high;

    if (x < low)
        return low;

    return x;

}

bool CNodeHeightMap::LoadResource(){

    if (load_all) return false;

        if (update_mesh) {
        std::string terrainFileName=std::string(GetParam("HeightMap"));

        if(terrainFileName==ASN_NOT_FOUND_PARAM) return true;

        printf("try get bitmap");
        ImageBitmap* myBitmap = DEVICER->GetBitmapFromImage(terrainFileName.c_str(),1.0);
        printf(" .... ok\n");

        if (!myBitmap) return true;

        printf("terrain %d %d %d\n",
        myBitmap->GetWidth(),
        myBitmap->GetHeight(),
        myBitmap->GetBpp()
        );

        std::string prop_Size=GetParam("SizeFromHeightMap");

        if (prop_Size=="1") {

                SetSize( CVector(myBitmap->GetWidth(),myBitmap->GetHeight(),Size.v[2]));
//                setCenterPoint( CVector(myBitmap->width,myBitmap->height,255.0f
        }

        width = (int)(floor((float)myBitmap->GetWidth()*detail));
        height = (int)(floor((float)myBitmap->GetHeight()*detail));

        width=2;
        height=2;

        printf("detail: %d %d %f\n", width, height, detail);

        if (HeightMap) free(HeightMap);
        terramesh.Free();
        HeightMap = new float[width*height];

        unsigned char* img_data = myBitmap->GetData();

//        int pos = 0;
//        double pos_x = 0;
//        double pos_y = 0;


        for (int j=0;j<height;j++) {

            for (int i=0;i<width;i++)
            {
//               int pos = ( (int)((float)j/detail)*width+(int)((float)i/detail))*myBitmap->bpp;//(int)(floor((float)j/detail*width+(float)i/detail))*myBitmap->bpp;
//               int pos = ( (int)((float)j/detail)*width+ (int)( (float)i/detail) )*myBitmap->bpp;//(int)(floor((float)j/detail*width+(float)i/detail))*myBitmap->bpp;
//                int pos = ((int)pos_y*width + (int)pos_x)*myBitmap->bpp;

            int pos_y = (int)((float)j/detail);
            int pos_x = (int)((float)i/detail);
            int pos = (pos_y * myBitmap->GetWidth() + pos_x)*myBitmap->GetBpp() ;

               HeightMap[j*width+i]=(
                              (float)((img_data[pos])/255.0f)
//                              +(float)((img_data[pos+1])/255.0f)
//                              +(float)((img_data[pos+2])/255.0f)
//                              )/3.0;
                                    );
//               printf("%d %d %d %f\n",j,i,pos,HeightMap[j*width+i]);

//            pos_x+=1.0/detail;

            };
//            pos_y+=1.0/detail;
//            pos_x=0;
       };

        if (!DrawTerrain) DrawTerrain = DEVICER->AddDrawObject( "-" ); //создаём динамический объект
        DrawTerrain->SetDrawType(ASN_DRAW_TRIANGLES);

        terramesh.SetVertexNum( (width-0) * (height-0) * 12 );

        int pbuf=0;

        for (int j=0;j<height;j++)
        {

            float x1 = clamp(((float)j-0.5)/height, 0, 1);
            float x2 = clamp(((float)j+0.0)/height, 0, 1);
            float x3 = clamp(((float)j+0.5)/height, 0, 1);
            float x4 = clamp(((float)j+1.0)/height, 0, 1);

            printf("%f %f %f %f\n", x1, x2, x3, x4);

            float hw = 0;//float(width)/2;
            float hh = 0;//float(height)/2;

            for (int i=0;i<width;i++)
            {

                float t = ((float)i+0)/width;
                float m = ((float)i+0.5)/width;
                float b = ((float)i+1.0)/width;

            printf("%f %f %f \n", t, m, b);

//////////////// 1 ///////////////////

//                        x1*width-hw, t*height-hh,
                terramesh.vertices[pbuf].pos.v[0]=(x1*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(t*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x3*width-hw, t*height-hh,

                terramesh.vertices[pbuf].pos.v[0]=(x3*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(t*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x2*width-hw, m*height-hh,

                terramesh.vertices[pbuf].pos.v[0]=(x2*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(m*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x2*width-hw, m*height-hh,

//////////////// 2 ///////////////////

                terramesh.vertices[pbuf].pos.v[0]=(x2*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(m*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x3*width-hw, t*height-hh,

                terramesh.vertices[pbuf].pos.v[0]=(x3*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(t*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x4*width-hw, m*height-hh,

                terramesh.vertices[pbuf].pos.v[0]=(x4*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(m*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x1*width-hw, b*height-hh,

//////////////// 3 ///////////////////

                terramesh.vertices[pbuf].pos.v[0]=(x1*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(b*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x2*width-hw, m*height-hh,

                terramesh.vertices[pbuf].pos.v[0]=(x2*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(m*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x3*width-hw, b*height-hh,

                terramesh.vertices[pbuf].pos.v[0]=(x3*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(b*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x2*width-hw, m*height-hh,

//////////////// 4 ///////////////////

                terramesh.vertices[pbuf].pos.v[0]=(x2*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(m*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x4*width-hw, m*height-hh,

                terramesh.vertices[pbuf].pos.v[0]=(x4*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(m*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];
            pbuf++;
//                        x3*width-hw, b*height-hh

                terramesh.vertices[pbuf].pos.v[0]=(x3*width-hw)*Size.v[0]/width;
                terramesh.vertices[pbuf].pos.v[1]=(b*height-hh)*Size.v[1]/height;
                terramesh.vertices[pbuf].pos.v[2]=10;//HeightMap[j*width+i]*Size.v[2];

            pbuf++;



            //terramesh.vertices[j*width+i].pos.v[0]=i*Size.v[0]/width;
            //terramesh.vertices[j*width+i].pos.v[1]=j*Size.v[1]/height;
            //terramesh.vertices[j*width+i].pos.v[2]=HeightMap[j*width+i]*Size.v[2];

            //terramesh.vertices[j*width+i].tex.v[0]=(double)i/width;
            //terramesh.vertices[j*width+i].tex.v[1]=(double)j/height;

            };

        };

   terramesh.SetIndexNum(width*height);

    uint baseIndex=0;
    for(int i=0, index=0; i<height-1; i++)
    {
        for(int j=0; j<width-1; j++, baseIndex++) {
            // 0, 2...
            int* indexes = terramesh.faces[index++].index;

            indexes[0]=baseIndex;
            indexes[1]=baseIndex+1;
            indexes[2]=baseIndex+2;

            //indexes[0]=baseIndex;
            //indexes[1]=baseIndex+1;
            //indexes[2]=baseIndex+width;
            // 1, 3...
            //indexes = terramesh.faces[index++].index;
            //indexes[0]=baseIndex+1;
            //indexes[1]=baseIndex+width;
            //indexes[2]=baseIndex+width+1;
        };
        baseIndex++;
    };

        DrawTerrain->SetDataBuffer(terramesh.vertices,sizeof(ASN_SM_Vertex), terramesh.numVertices);
        DrawTerrain->SetDataOffset(ASN_DATA_UV_BUFF, ((long)&terramesh.vertices[0].tex) - ((long)&terramesh.vertices [0]));
        //DrawTerrain->SetIndexBuffer(terramesh.faces,sizeof(ASN_SM_Triangle),2/*terramesh.numFaces*/);

        myBitmap->Release(); //освобождаем ресурс

        CalculateLOD();

        // если текстура высоты у нас та же, что и накладываемая
        // то ее тоже обновить надо бы
        std::string textureFileName=std::string(GetParam("Texture"));
        if(textureFileName==ASN_NOT_FOUND_PARAM) update_texture=true;


    }

    if (update_texture) {
        if (!DrawMaterial) DrawMaterial = DEVICER->AddMaterial( "-" );

        std::string textureFileName=std::string(GetParam("Texture"));
        if(textureFileName==ASN_NOT_FOUND_PARAM)
           DrawMaterial->SetParam("Texture", GetParam("HeightMap") );
        else
           DrawMaterial->SetParam("Texture", textureFileName.c_str() );
        DrawMaterial->SetParam("TextureOnly", "1" );
        DrawMaterial->SetParam("Texture_repeat", "1" );
    }

    // Done
    load_all=true;
    return false;
};

bool CNodeHeightMap::Draw()
{

    if (!load_all) {
            LoadResource();
            if (!load_all) return true;
    };


        DEVICER->PushMatrix();
        DEVICER->MultModelMatrix(NodeMatrix.m_data);

        if (NodeColor) DEVICER->SetColor(*NodeColor);

        DEVICER->SetMaterial( DrawMaterial );

//        DrawTerrain->SetIndexBuffer(
//	lod_terrain->faces_lod[4].faces,
//	sizeof(ASN_SM_Triangle),
//	lod_terrain->faces_lod[4].numFaces/10
//	);

        DEVICER->Draw( DrawTerrain );

        DEVICER->PopMatrix();

    return false;
};

void CNodeHeightMap::SetParam(const char* param_name,const char* param_value){
    std::string ParamName = std::string(param_name);
    std::string ParamValue = std::string(param_value);

    if ( ParamName=="Texture") {
        update_texture=true;
        load_all=false;
    };

    if ( ParamName=="HeightMap") {
        update_mesh=true;
        load_all=false;
    };

    if ( ParamName=="Detail") {
        if (!CheckStrToFloat(param_value)) LOGGER->ErrMsg(2,"Detail error!");
        detail = 1.0;
        detail = StrToFloat(param_value)/100.0;
	//printf("Detail set to: %f\n", detail);
        update_mesh=true;
        load_all=false;
    };

    CNode::SetParam(ParamName.c_str(),ParamValue.c_str());
};

ASNInterface* CNodeHeightMap::GetInterface(const char* interface_name){
if ( strcmp(interface_name,"ASNITerrain") == 0 ) {
 return (ASNITerrain*)this;
    }
    LOGGER->ErrMsgStr(2,std::string("CNodeHeightMap can't support ")+interface_name);
    return NULL;
};


bool CNodeHeightMap::UploadHeightMap(int h_width, int h_height, float* map){
printf("1баааааррр!!!\n");
        width = h_width;
        height = h_height;

        std::string prop_Size=GetParam("SizeFromHeightMap");

        if (prop_Size=="1") {

                SetSize( CVector(width,height,Size.v[2]));
//                setCenterPoint( CVector(myBitmap->width,myBitmap->height,255.0f
        }

        if (HeightMap) free(HeightMap);
        terramesh.Free();
        HeightMap = new float[width*height];

//        unsigned char* img_data = (unsigned char*)myBitmap->data;

        for (int j=0;j<height;j++)
            for (int i=0;i<width;i++)
            {
               HeightMap[j*width+i]=map[j*width+i];
               //printf("%d %d %d\n",j,i,img_data[(j*width+i)*myBitmap->bpp]);
            };

        if (!DrawTerrain) DrawTerrain = DEVICER->AddDrawObject( "-" ); //создаём динамический объект

        terramesh.SetVertexNum(width*height);

        for (int j=0;j<height;j++)
            for (int i=0;i<width;i++)
            {
            terramesh.vertices[j*width+i].pos.v[0]=i*Size.v[0]/width;
            terramesh.vertices[j*width+i].pos.v[1]=j*Size.v[1]/height;
            terramesh.vertices[j*width+i].pos.v[2]=HeightMap[j*width+i]*Size.v[2];

            terramesh.vertices[j*width+i].tex.v[0]=(double)i/width;
            terramesh.vertices[j*width+i].tex.v[1]=(double)j/height;

            };

   terramesh.SetIndexNum(width*height*2);

    uint baseIndex=0;
    for(int i=0, index=0; i<height-1; i++)
    {
        for(int j=0; j<width-1; j++, baseIndex++) {
            // 0, 2...
            int* indexes = terramesh.faces[index++].index;
            indexes[0]=baseIndex;
            indexes[1]=baseIndex+1;
            indexes[2]=baseIndex+width;
            // 1, 3...
            indexes = terramesh.faces[index++].index;
            indexes[0]=baseIndex+1;
            indexes[1]=baseIndex+width;
            indexes[2]=baseIndex+width+1;
        };
        baseIndex++;
    };

        DrawTerrain->SetDataBuffer(terramesh.vertices,sizeof(ASN_SM_Vertex), terramesh.numVertices);
        DrawTerrain->SetDataOffset(ASN_DATA_UV_BUFF, ((long)&terramesh.vertices[0].tex) - ((long)&terramesh.vertices [0]));
        DrawTerrain->SetIndexBuffer(terramesh.faces,sizeof(ASN_SM_Triangle),terramesh.numFaces);

        //printf("баааааррр!!!\n");
        // если текстура высоты у нас та же, что и накладываемая
        // то ее тоже обновить надо бы
        std::string textureFileName=std::string(GetParam("Texture"));
        if(textureFileName==ASN_NOT_FOUND_PARAM) { load_all=false; update_texture=true; }

        CalculateLOD();

        return false;

};

const float* CNodeHeightMap::GetHeightMap(int& i_width, int& i_height){
    i_width=width;
    i_height=height;
    return HeightMap;
}

const float CNodeHeightMap::GetHeight(float pos_x, float pos_y){
float calc_height;

    if (pos_x<0) return NAN;
    if (pos_y<0) return NAN;
    if (pos_x>Size.v[0]) return NAN;
    if (pos_y>Size.v[1]) return NAN;

    int pos1 = (int)floor(pos_y*height/Size.v[1])*width + (int)floor(pos_x*width/Size.v[0]);
    int pos2 = (int)floor(pos_y*height/Size.v[1]+1.0)*width + (int)floor(pos_x*width/Size.v[0]);
    int pos3 = abs((int)floor(pos_y*height/Size.v[1]-1.0))*width + (int)floor(pos_x*width/Size.v[0]);
    int pos4 = (int)floor(pos_y*height/Size.v[1])*width + (int)floor(pos_x*width/Size.v[0]+1.0);
    int pos5 = (int)floor(pos_y*height/Size.v[1])*width + abs((int)floor(pos_x*width/Size.v[0]-1.0) );

    calc_height = (HeightMap[pos1]+HeightMap[pos2]+HeightMap[pos3]+HeightMap[pos4]+HeightMap[pos5])*Size.v[2]/5.0;

    return calc_height;

}

bool CNodeHeightMap::SetHeight(float pos_x, float pos_y, float height){
    return false;
}


void CNodeHeightMap::CalculateLOD(){

    printf("calc LOD...");

    lod_terrain->Free();

    //lod_terrain->index_buffer=terramesh.faces;
    //lod_terrain->terrain_width=width;
    //lod_terrain->terrain_height=height;

    //lod_terrain->point_min=CVector( 0, 0, 0);
    //lod_terrain->point_max=Size;

    lod_terrain->MakeLOD(width,height,HeightMap);

    printf("ok\n");

};
