#pragma once
class Cell
{
	D3DXVECTOR3 m_vVertex[3];		//하나의 삼각형중 세 점을 담을 변수
	//D3DXVECTOR3 center;			
	D3DXVECTOR3 m_vVertexCenter[3];//사이드 라인의 중점
	//
	float m_fArrivalCost[3];		//중점 - 사이드간 거리
	//
	Cell* m_pNeighborCell[3];	//나랑 이웃한 삼각형의 포인터

	
	//하나의 삼각형중 세 점을 담을 변수
	//SYNTHESIZE_REF(D3DXVECTOR3, m_vVertex[3], Vertex);

	//삼각형의 센터
	SYNTHESIZE(D3DXVECTOR3, m_vCenter, Center);

	//사이드 라인의 중점
	//SYNTHESIZE(D3DXVECTOR3, m_vVertexCenter, VertexCenter);

	////중점 - 사이드간 거리
	//SYNTHESIZE_REF(float, m_fArrivalCost[3], ArrivalCost);

	//나랑 이웃한 삼각형의 포인터
	//SYNTHESIZE_REF(Cell*, m_pNeighborCell[3], NeighborCell);

	//총 코스트
	SYNTHESIZE(float, m_fTotalCost, TotalCost);

	//시작부터 현재까지의 코스트
	SYNTHESIZE(float, m_fCostFromStart, CostFromStart);

	//시작부터 끝까지의 코스트
	SYNTHESIZE(float, m_fCostToGoal, CostToGal);

	//내 타일 오기 전의 타일
	SYNTHESIZE(Cell*, m_pParentCell, ParentCell);

	//갈수 있는 타일이냐
	SYNTHESIZE(bool, m_bIsOpen, IsOpen);

	//타일의 속성(출발, 도착 등 설정을 위한 변수)
	SYNTHESIZE(string, m_sAttribute, Attribute);

	//float m_fTotalCost;
	//float m_fCostFromStart;
	//float m_fCostToGoal;

	//bool m_bIsOpen;
	//Cell* m_cParentCell;
public:
	Cell();
	~Cell();

	void Setup();
	void Update();
	void Render();

	//삼각형 세점 겟셋
	void SetVertex(const D3DXVECTOR3* pVertex) {
		for (int i = 0; i < 3; i++)
		{
			m_vVertex[i] = pVertex[i];
		}
	}

	D3DXVECTOR3* GetVertex(){ return m_vVertex; }

	//사이드 라인의 중점 겟셋
	void SetVertexCenter(const D3DXVECTOR3* pVertexCenter) {
		for (int i = 0; i < 3; i++)
		{
			m_vVertexCenter[i] = pVertexCenter[i];
		}
	}

	D3DXVECTOR3* GetVertexCenter(){ return m_vVertexCenter; }

	//중점 - 사이드간 거리
	void SetArrivalCost(const float* pArrivalCost)
	{
		for (int i = 0; i < 3; i++)
		{
			m_fArrivalCost[i] = pArrivalCost[i];
		}
	}

	float* GetArrivalCost(){ return m_fArrivalCost; }

	//나랑 이웃한 삼각형의 포인터
	void SetNeighborCell( Cell** ppCell)
	{
		for (int i = 0; i < 3; i++)
		{
			m_pNeighborCell[i] = (ppCell[i]);
		}
	}

	Cell** GetNeighborCell(){ return m_pNeighborCell; }


	void ZeroReset();
	/*
	D3DXVECTOR3 vertex[3];		//하나의 삼각형중 세 점을 담을 변수

	D3DXVECTOR3 vertexCenter[3];//사이드 라인의 중점
	//
	float arrivalCost[3];		//중점 - 사이드간 거리
	//
	Cell* neighborCell[3];	//나랑 이웃한 삼각형의 포인터
	*/

};

