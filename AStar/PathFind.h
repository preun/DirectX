#pragma once
#include "Cell.h"

class Cell;

class PathFind
{
	GET(vector<Cell*>, m_vNaviCell, NaviCell);
	//vector<Cell> m_vNaviCell;

public:
	PathFind();
	~PathFind();

	void Setup(vector<D3DXVECTOR3>& Vertex);
	//void Update();
	//void Render();

							//���̵��� ����		�ڱ��ڽ�	
	//ST_CELL* MakeNeighborCell(D3DXVECTOR3 rayPos, int index);

	//�̿� ���� �Լ�
	void MakeNeighborCell(int index);

	//�������� ������ ���⼭		���� �ε���			
	Cell** SubMakeNeighborCell(int myCellIndex);

	//��� �ִ� ĭ�� �ε��� ���ϱ�
	int GetCellIndex(D3DXVECTOR3 pos);
};

