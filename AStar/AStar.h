#pragma once
#include "Cell.h"

class PathFind;

class AStar
{
	PathFind* m_pPathFind;
	//���� ���� ��
	vector<Cell*> m_vCurrentCell;
	//��ü Ÿ��
	vector<Cell*> m_vTotalList;
	vector<Cell*>::iterator m_viTotalList;
	
	//���� �ִ� Ÿ�� ����
	vector<Cell*> m_vOpenList;
	vector<Cell*>::iterator m_viOpenList;
	
	//���������� ���� ����� ��� ��� ����
	vector<Cell*> m_vCloseList;
	vector<Cell*>::iterator m_viCloseList;
	
	Cell* m_pStartCell;
	Cell* m_pEndCell;
	Cell* m_pCurrentCell;
	
	//tagCell* _currentGameCell;

public:
	AStar();
	~AStar();

	//���� ���� �����̽� ���� ����
	//�� �Ѿ�� �ѹ��� ���ָ� �ȴ�.
	void SetCurrentCell(vector<D3DXVECTOR3> Vertex);

	//���, ���� �� �� �ʱ�ȭ�ܰ�
	void SetCell(int MyCellIndex, int TargetIndex);

	//�� �� �ִ� ���� �˻��ؼ� �߰��ϴ� �Լ�
	vector<Cell*> addOpenList(Cell* currentCell);

	//�� ã�� �Լ�
	void pathFinder(Cell* currentCell);

	D3DXVECTOR3 GetNextCell(OUT vector<D3DXVECTOR3>* path);
	

	int GetCellIndex(D3DXVECTOR3 pos);

	//����׿� ����
	void Render(int MyCellIndex, int TargetIndex, D3DXVECTOR3* pos);
};

