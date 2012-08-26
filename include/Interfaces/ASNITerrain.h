#ifndef _I_ASN_TERRAIN_H
#define _I_ASN_TERRAIN_H

// ��������� � ��������
class ASNITerrain: public ASNInterface {
    public:
    const char* GetType(){ return "ASNITerrain"; }

    // �������� ��������� �� ����� �����, �� ������ � ������
    virtual const float* GetHeightMap(int& width, int& height)=0;

    // ��������� ����� �����
    virtual bool UploadHeightMap(int h_width, int h_height, float* map)=0;

    // �������� ���������� �������� ������
    // ��������! pos_x � pos_y - �� ���������� ������ � HeightMap
    // � ���������� ����� �� �����������, ������� ��������
    // �� ����� ����� � ��������, ������������ ���������� Size ����
    // 0,0 - ������ ��������� ����
    virtual const float GetHeight(float pos_x, float pos_y)=0;
    
    // ������������� ������ ������
    // �� ������-�� ����� ������� ��� �������� ����� ��������� �� ����� �����
    // �� ���� � �������� ������������ ����������� ������������ ��������
    // �� OCTREE, �� �� �� �������� ��������� ������ ������ � 
    // ����� ����� ����������� ���������� ��� ������ � ������, � ��� ��� :(
    virtual bool SetHeight(float pos_x, float pos_y, float height)=0;

    };
    
#endif
