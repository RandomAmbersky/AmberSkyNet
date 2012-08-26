#include "IEngine.h"

#include "CNodeRuler.h"
#include "GL/gl.h"
#include "ASN_utils.h"

#include "ILog.h"
#include "IFile.h"
#include "IFileSystem.h"

#define ENGINE Engine // we already have pointer to Engine

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

const float bodyPattern[]=
// I don't know why 'const ASN_SM_Vertex bodyPattern[]' don't work :( - DH
{
	-1,  0, 0,   0,0,  0,0,1,
 	-1, -2, 0,   1,0,  0,0,1,
	50, -2, 0,   1,1,  0,0,1,
 	50,  0, 0,   0,1,  0,0,1,
};

// tEmPoRaRy UGLY TRICK: (~~)
// this declaration MUST BE in include files! (~~)
extern void xyz_to_uv_sphere( float uv[2], const float vertex[3], const float normal[3] );

CNodeRuler::CNodeRuler(IEngine *_Engine):
  CNode(_Engine){
    bodyBuffer=NULL;
    lineBuffer=NULL;
    body=NULL;
    lineCloner=NULL;
    SetParam("BaseClass","Node");
    SetParam("Type","NodeRuler");
    SetParam("Name","-");
};

CNodeRuler::~CNodeRuler(){
    delete bodyBuffer;
    delete lineBuffer;
};

bool CNodeRuler::LoadResource(){
    char err[MAX_PATH+64];
    UINT i;

    if (load_all) return false; // work done

    length=atoi(GetParam("Length"));
    if (length<0.1) length=50;
    int pointNum = 2*(RISKPERMETER*length+1);
    // Ruler body - grey rectangle
    if (!body) {
        body=DEVICER->AddDrawObject(
         std::string("RulerBody"+IntToStr(length)).c_str() );
        // Object name must contain length because
        // different-length rulers must use different draw objects
        if (!bodyBuffer) bodyBuffer=new float[8*4];
        memcpy(bodyBuffer,bodyPattern,4*8*sizeof(float));
        body->SetDrawType(ASN_DRAW_QUADS);
        body->SetDataBuffer((void*)bodyBuffer,sizeof(float)*8,4);
        body->SetDataOffset(ASN_DATA_VERTEX_BUFF,0);
        body->SetDataOffset(ASN_DATA_UV_BUFF, 3*sizeof(float) );
        body->SetDataOffset(ASN_DATA_NORMAL_BUFF, 5*sizeof(float) );
        bodyBuffer[24]=bodyBuffer[16]=length+1;
    };
    // Ruler marks - black lines
    if (!lineCloner) {
        lineCloner=DEVICER->AddDrawObject(
         std::string("RulerMarks"+IntToStr(length)).c_str() );
        if (!lineBuffer) lineBuffer=new float[8*pointNum];
        float* lineP=lineBuffer;
        for (i=0; i<pointNum; i++){
            lineP[0]=float(int(i/2))/RISKPERMETER;
            lineP[1]=0;
            if (int(i/2)*2!=i) {
                // per 1 meter
                if (int((i-1)/2/RISKPERMETER)*2*RISKPERMETER==(i-1))
                    lineP[1]=-1;
                // per 0.5 meter
                else if (int((i-1)/RISKPERMETER)*RISKPERMETER==(i-1))
                    lineP[1]=-0.7;
                // per 0.1 meter
                else
                    lineP[1]=-0.4;
            };
            lineP[2]=0.1;
            lineP[3]=0;
            lineP[4]=0;
            lineP[5]=0;
            lineP[6]=0;
            lineP[7]=1;
            lineP+=8;
        };
        lineCloner->SetDrawType(ASN_DRAW_LINES);
        lineCloner->SetDataBuffer((void*)lineBuffer,sizeof(float)*8,pointNum);
        lineCloner->SetDataOffset(ASN_DATA_VERTEX_BUFF,0);
        lineCloner->SetDataOffset(ASN_DATA_UV_BUFF, 3*sizeof(float) );
        lineCloner->SetDataOffset(ASN_DATA_NORMAL_BUFF, 5*sizeof(float) );
    };
/*    // font test
    DEVICER->BuildFont("SimpleF", "font.bmp");
    DEVICER->SetFont("SimpleF");*/
    // Done
    load_all=true;
    return false;
};

bool CNodeRuler::Draw(){
//printf("draw rules...");
    if (load_all) {
        DEVICER->PushMatrix();
        DEVICER->MultModelMatrix(NodeMatrix.m_data);
        // body
        DEVICER->SetMaterial(NULL);
        DEVICER->SetColor(CVector4(0.35, 0.35, 0.35, 0.95));
        DEVICER->Draw(body);
        // marks
        DEVICER->SetMaterial(NULL);
        DEVICER->SetColor(CVector4(0, 0, 0, 1));
        DEVICER->Draw( lineCloner );
        // digits
        DEVICER->SetScale(CVector(FONTREDUCE, FONTREDUCE, FONTREDUCE));
        for (UINT i=0; i<=length; i++)
            DEVICER->PrintAt(FONTSCALE*i,-1.5*FONTSCALE,IntToStr(i).c_str());
        DEVICER->PopMatrix();
    }
    else return LoadResource();

    return false;
};

void CNodeRuler::SetParam(const char* param_name,const char* param_value){
    std::string ParamName = std::string(param_name);
    std::string ParamValue = std::string(param_value);

    if (ParamName=="Length") load_all=false;
    CNode::SetParam(ParamName.c_str(),ParamValue.c_str());

};

