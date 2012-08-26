#ifndef _SLTERRAINDEF_H
#define _SLTERRAINDEF_H

#define MAXTERRAIN 256
#define METERSPERPIXEL 1
#define MAXTERRAINPOINT ((MAXTERRAIN-1)*METERSPERPIXEL)
#define DEFAULTWATERMARK 20

#define GROUNDMATERIALNAME "Ground"

struct SLSQUAREMETER {
    BYTE heightField;
    BYTE heightMultiplyFactor;
    BYTE waterHeightOfSim; // in meters
    BYTE landParcels;
    BYTE forSale;
    BYTE publicEditObject;
    BYTE publicEditLand;
    BYTE safeLand;
    BYTE flyingAllowed;
    BYTE createLandMark;
    BYTE outsideScriptsAllowed;
    BYTE originakHeightField;
    BYTE originakHeightMultiplyFactor;
};

typedef
    SLSQUAREMETER SLTERRAIN[MAXTERRAIN][MAXTERRAIN];


    
#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

#endif

