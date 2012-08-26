#ifndef _I_ASN_TERRAIN_H
#define _I_ASN_TERRAIN_H

// интерфейс к террайну
class ASNITerrain: public ASNInterface {
    public:
    const char* GetType(){ return "ASNITerrain"; }

    // получаем указатель на карту высот, ее ширину и высоту
    virtual const float* GetHeightMap(int& width, int& height)=0;

    // загружаем карту высот
    virtual bool UploadHeightMap(int h_width, int h_height, float* map)=0;

    // получаем конкретное значение высоты
    // ¬Ќ»ћјЌ»≈! pos_x и pos_y - не координаты €чейки в HeightMap
    // а координаты точки на поверхности, котора€ рисуетс€
    // по карте высот в масштабе, определ€емом параметром Size ноды
    // 0,0 - начало координат Ќќƒџ
    virtual const float GetHeight(float pos_x, float pos_y)=0;
    
    // устанавливаем высоту €чейки
    // мы вообще-то можем сделать это напр€мую через указатель на карту высот
    // но если в террайне используетс€ оптимизаци€ пространства например
    // по OCTREE, то он не отследит изменение высоты €чейки и 
    // далее будет неправильно определ€ть что попало в камеру, а что нет :(
    virtual bool SetHeight(float pos_x, float pos_y, float height)=0;

    };
    
#endif
