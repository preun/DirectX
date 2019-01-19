#pragma once
#include "Cell.h"

class PathFind;

class AStar
{
	PathFind* m_pPathFind;
	//현재 맵의 셀
	vector<Cell*> m_vCurrentCell;
	//전체 타일
	vector<Cell*> m_vTotalList;
	vector<Cell*>::iterator m_viTotalList;
	
	//갈수 있는 타일 벡터
	vector<Cell*> m_vOpenList;
	vector<Cell*>::iterator m_viOpenList;
	
	//최종적으로 제일 가까운 경로 담는 벡터
	vector<Cell*> m_vCloseList;
	vector<Cell*>::iterator m_viCloseList;
	
	Cell* m_pStartCell;
	Cell* m_pEndCell;
	Cell* m_pCurrentCell;
	
	//tagCell* _currentGameCell;

public:
	AStar();
	~AStar();

	//현재 맵의 서페이스 정보 세팅
	//맵 넘어갈때 한번만 해주면 된다.
	void SetCurrentCell(vector<D3DXVECTOR3> Vertex);

	//출발, 도착 셀 및 초기화단계
	void SetCell(int MyCellIndex, int TargetIndex);

	//갈 수 있는 길을 검색해서 추가하는 함수
	vector<Cell*> addOpenList(Cell* currentCell);

	//길 찾기 함수
	void pathFinder(Cell* currentCell);

	D3DXVECTOR3 GetNextCell(OUT vector<D3DXVECTOR3>* path);
	

	int GetCellIndex(D3DXVECTOR3 pos);

	//디버그용 렌더
	void Render(int MyCellIndex, int TargetIndex, D3DXVECTOR3* pos);
};

