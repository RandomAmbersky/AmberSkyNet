

const float CubeBuffer[]=
{
    /// X, Y, Z      UV        NORMALS
	0, 0, 0,      0,0,       0,0,0,
 	1, 0, 0,       0,1,       0,0,0,
	1,  1, 0,       1,0,       0,0,0,  
	0,  1, 0,      1,1,       0,0,0, 
	0, 0,  1,      0,0,       0,0,0,
	1, 0,  1,       0,1,       0,0,0,
	1,  1,  1,       1,0,       0,0,0,
	0,  1,  1,      1,1,       0,0,0, 
	
};

const int CubeIndex[] =
{
0,1,
1,2,
2,3,
3,0,
4,5,
5,6,
6,7,
7,4,
0,4,
1,5,
2,6,
3,7,
//0,1,2,3,
//4,5,6,7,
/* ������� ������, �� 4 �� ������� ������ */
//	1, 0, 3, 1,3,2,
//	4, 5, 6, 4,6,7,
//	1, 2, 6, 1,6,5,
//	0, 4, 7, 0,7,3,
//	1, 5, 4, 1,4,0,
//	3, 7, 6, 3,6,2,
};



//������ ��� ������
const float boxVertexes[] =
{
	/*x,y,z*/
	-1, -1, -1,
	1, -1, -1,
	1,  1, -1,
	-1,  1, -1,
	-1, -1,  1,
	1, -1,  1,
	1,  1,  1,
	-1,  1,  1,
};

const float boxUVs[] =
{
	/*u,v ���������� ��� ���� ������ ������� �������� */
	0, 0,
	0, 1,
	1, 1,
	1, 0
};
//������� ��� ������ �����
const float boxNormals[] =
{
	0,  0, -1,
	0, 0,  1,
	1,  0,  0,
	-1,  0, 0,
	0,  -1,  0,
	0,  1, 0
};
const int boxQuads[] =
{
	/* ������� ������, �� 4 �� ������� ������ */
	1, 0, 3, 2,
	4, 5, 6, 7,
	1, 2, 6, 5,
	0, 4, 7, 3,
	1, 5, 4, 0,
	3, 7, 6, 2
};

