#ifndef _CBOUNDING_BOX_H
#define _CBOUNDING_BOX_H


#include "CVector.h"

struct CBoundingBox{

CVector minPoint;
CVector maxPoint;
    CBoundingBox ()
    {
        minPoint=CVector();
        minPoint=CVector();
    }

  inline void EnlargeMe(CVector& Triangle){
    minPoint.Min(Triangle);
    maxPoint.Max(Triangle);
  };

  inline bool PoinInBox(CVector& Point){
    
  if (
  (Point.v[0]>=minPoint.v[0]) & (Point.v[0]<=maxPoint.v[0]) 
& (Point.v[1]>=minPoint.v[1]) &  (Point.v[1]<=maxPoint.v[1])
& (Point.v[2]>=minPoint.v[0]) &  (Point.v[2]<=maxPoint.v[2])
       )  return true;
    return false;
  };

  inline bool CheckIntersect(CVector& Start,CVector& End,CVector& Hit1, CVector& Hit2){

CVector Normal, V1, V2, V3;
CVector Mouse_Pos;

CVector HitPos[2];
HitPos[0]=CVector();
HitPos[1]=CVector();

int HitNum=0;

//printf("min point %f %f %f \n",minPoint.v[0],minPoint.v[1],minPoint.v[2]);
//printf("max point %f %f %f \n",maxPoint.v[0],maxPoint.v[1],maxPoint.v[2]);

//////////////////////// part one

// ---------------- плоскость XYd ------------------
V1=minPoint;
V2=minPoint;
V3=minPoint;

V2.v[0]+=maxPoint.v[0];
V3.v[1]+=maxPoint.v[1]; 

Normal.GetNormal(V1,V2,V3);

Mouse_Pos.GetPlaneIntersect(Start,End,V1,Normal);


if (
(Mouse_Pos.v[0]>minPoint.v[0]) & (Mouse_Pos.v[0]-minPoint.v[0]<maxPoint.v[0])
&
(Mouse_Pos.v[1]>minPoint.v[1]) & (Mouse_Pos.v[1]-minPoint.v[1]<maxPoint.v[1])
)
{HitPos[HitNum]=Mouse_Pos;HitNum++;
//printf("[%s] XYd Mouse hit pos: %f %f %f \n",ParamList.GetStrParam("Name").c_str(),Mouse_Pos.v[0],Mouse_Pos.v[1],Mouse_Pos.v[2]);
}

// ---------------- плоскость XZd ------------------
V1=minPoint;
V2=minPoint;
V3=minPoint;

V2.v[0]+=maxPoint.v[0];
V3.v[2]+=maxPoint.v[2];

Normal.GetNormal(V1,V2,V3);

Mouse_Pos.GetPlaneIntersect(Start,End,V1,Normal);

if (
(Mouse_Pos.v[0]>minPoint.v[0]) & (Mouse_Pos.v[0]-minPoint.v[0]<maxPoint.v[0])
&
(Mouse_Pos.v[2]>minPoint.v[2]) & (Mouse_Pos.v[2]-minPoint.v[2]<maxPoint.v[2])
)
{HitPos[HitNum]=Mouse_Pos;HitNum++;
//printf("[%s] XZd Mouse hit pos: %f %f %f \n",ParamList.GetStrParam("Name").c_str(),Mouse_Pos.v[0],Mouse_Pos.v[1],Mouse_Pos.v[2]);
}

// ---------------- плоскость YZd ------------------
V1=minPoint;
V2=minPoint;
V3=minPoint;

V2.v[1]+=maxPoint.v[1];
V3.v[2]+=maxPoint.v[2];

Normal.GetNormal(V1,V2,V3);

Mouse_Pos.GetPlaneIntersect(Start,End,V1,Normal);

if (
(Mouse_Pos.v[1]>minPoint.v[1]) & (Mouse_Pos.v[1]-minPoint.v[1]<maxPoint.v[1])
&
(Mouse_Pos.v[2]>minPoint.v[2]) & (Mouse_Pos.v[2]-minPoint.v[2]<maxPoint.v[2])
)
{HitPos[HitNum]=Mouse_Pos;HitNum++;
//printf("[%s] YZd Mouse hit pos: %f %f %f \n",ParamList.GetStrParam("Name").c_str(),Mouse_Pos.v[0],Mouse_Pos.v[1],Mouse_Pos.v[2]);
}

//////////////////////// part one


//////////////////////// part tho 

// ---------------- плоскость XYu ------------------
V1=minPoint;
V2=minPoint;
V3=minPoint;

V1.v[2]+=maxPoint.v[2];

V2.v[0]+=maxPoint.v[0];
V2.v[2]+=maxPoint.v[2];

V3.v[1]+=maxPoint.v[1]; 
V3.v[2]+=maxPoint.v[2]; 

Normal.GetNormal(V1,V2,V3);

Mouse_Pos.GetPlaneIntersect(Start,End,V1,Normal);

if (
(Mouse_Pos.v[0]>minPoint.v[0]) & (Mouse_Pos.v[0]-minPoint.v[0]<maxPoint.v[0])
&
(Mouse_Pos.v[1]>minPoint.v[1]) & (Mouse_Pos.v[1]-minPoint.v[1]<maxPoint.v[1])
)
{HitPos[HitNum]=Mouse_Pos;HitNum++;
//printf("[%s] XYu Mouse hit pos: %f %f %f \n",ParamList.GetStrParam("Name").c_str(),Mouse_Pos.v[0],Mouse_Pos.v[1],Mouse_Pos.v[2]);
}


// ---------------- плоскость XZu ------------------
V1=minPoint;
V2=minPoint;
V3=minPoint;

V1.v[1]+=maxPoint.v[1];

V2.v[0]+=maxPoint.v[0];
V2.v[1]+=maxPoint.v[1];

V3.v[1]+=maxPoint.v[1];
V3.v[2]+=maxPoint.v[2];


//( 5 ) ( coolman ) ( d )
Normal.GetNormal(V1,V2,V3);

Mouse_Pos.GetPlaneIntersect(Start,End,V1,Normal);

if (
(Mouse_Pos.v[0]>minPoint.v[0]) & (Mouse_Pos.v[0]-minPoint.v[0]<maxPoint.v[0])
&
(Mouse_Pos.v[2]>minPoint.v[2]) & (Mouse_Pos.v[2]-minPoint.v[2]<maxPoint.v[2])
)
{HitPos[HitNum]=Mouse_Pos;HitNum++;
//printf("[%s] XZu Mouse hit pos: %f %f %f \n",ParamList.GetStrParam("Name").c_str(),Mouse_Pos.v[0],Mouse_Pos.v[1],Mouse_Pos.v[2]);
}

// ---------------- плоскость YZu ------------------
V1=minPoint;
V2=minPoint;
V3=minPoint;

V1.v[0]+=maxPoint.v[0];

V2.v[1]+=maxPoint.v[1];
V2.v[0]+=maxPoint.v[0];

V3.v[2]+=maxPoint.v[2];
V3.v[0]+=maxPoint.v[0];

Normal.GetNormal(V1,V2,V3);

Mouse_Pos.GetPlaneIntersect(Start,End,V1,Normal);

if (
(Mouse_Pos.v[1]>minPoint.v[1]) & (Mouse_Pos.v[1]-minPoint.v[1]<maxPoint.v[1])
&
(Mouse_Pos.v[2]>minPoint.v[2]) & (Mouse_Pos.v[2]-minPoint.v[2]<maxPoint.v[2])
)
{HitPos[HitNum]=Mouse_Pos;HitNum++;
//printf("[%s] YZu Mouse hit pos: %f %f %f \n",ParamList.GetStrParam("Name").c_str(),Mouse_Pos.v[0],Mouse_Pos.v[1],Mouse_Pos.v[2]);
}

///////// end part two 
if (HitNum!=2) return false;
//else 
Hit1=HitPos[0];
Hit2=HitPos[1];
return true;
  }

};
#endif
