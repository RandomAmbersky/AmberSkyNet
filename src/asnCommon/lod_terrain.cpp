#include "lod_terrain.h"

//void clear_lod_list(sm_lod_list* my_list){
//
//    for (int i=0;i<my_list->num_lods;i++){
//	delete [] my_list->lods[i];
//    };
//
//    delete my_list->lods;
//};


///////////////////////////////////////////////////
///// лирическое отступление насчет LODов /////////
///////////////////////////////////////////////////
/*  я думал обойтись одними индексами, однако этого было недостаточно -
появляются артефакты разности высот.. :(
*/


ASN_SimpleTerrainLOD::ASN_SimpleTerrainLOD(){
//    my_lods=NULL;
};

ASN_SimpleTerrainLOD::~ASN_SimpleTerrainLOD(){
    Free();
};

void ASN_SimpleTerrainLOD::Free(){

    if (faces_lod.size()==0) return;

    printf("free lods...\n");

    std::vector<sm_lod>::iterator it_=faces_lod.begin();

    for (int i=0; i<faces_lod.size(); i++ ){
	printf("remove faces lod %d\n", (*it_).numFaces );
        delete [] (*it_).faces;
	it_++;
	};


};

bool ASN_SimpleTerrainLOD::MakeLOD(int width, int height, float* height_map){

    printf("make lod %d %d\n", width, height );

    int width_l=width;
    int height_l=height;

    Free();

    sm_lod myLod;

    int baseIndexD = 1;

    do {

	myLod.numFaces = width_l*height_l*2;
	myLod.faces = new ASN_SM_Triangle[myLod.numFaces];

        UINT baseIndex=0;
	UINT index=0;
	for(int i=0; i<height_l-1; i++)
        {
        for(int j=0; j<width_l-1; j++) {

            // 0, 2...
            int* indexes = myLod.faces[index++].index;
            indexes[0]=baseIndex;
            indexes[1]=baseIndex+baseIndexD;
            indexes[2]=baseIndex+width*baseIndexD;

            // 1, 3...
            indexes = myLod.faces[index++].index;
            indexes[0]=baseIndex+baseIndexD;
            indexes[1]=baseIndex+width*baseIndexD;
            indexes[2]=baseIndex+width*baseIndexD+baseIndexD;

            baseIndex+=baseIndexD;

    	    };

            baseIndex=baseIndex+width*baseIndexD-width-baseIndexD;
        };

	printf("base index %d\n", baseIndex);

        width_l = width_l / 2;
        height_l = height_l / 2;

	baseIndexD*=2;

	faces_lod.push_back( myLod );

        printf("add faces lod %d %d\n", width_l, height_l);

      } while ( (width_l > 4) && (height_l > 4) );

}
