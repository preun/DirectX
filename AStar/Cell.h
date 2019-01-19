#pragma once
class Cell
{
	D3DXVECTOR3 m_vVertex[3];		//�ϳ��� �ﰢ���� �� ���� ���� ����
	//D3DXVECTOR3 center;			
	D3DXVECTOR3 m_vVertexCenter[3];//���̵� ������ ����
	//
	float m_fArrivalCost[3];		//���� - ���̵尣 �Ÿ�
	//
	Cell* m_pNeighborCell[3];	//���� �̿��� �ﰢ���� ������

	
	//�ϳ��� �ﰢ���� �� ���� ���� ����
	//SYNTHESIZE_REF(D3DXVECTOR3, m_vVertex[3], Vertex);

	//�ﰢ���� ����
	SYNTHESIZE(D3DXVECTOR3, m_vCenter, Center);

	//���̵� ������ ����
	//SYNTHESIZE(D3DXVECTOR3, m_vVertexCenter, VertexCenter);

	////���� - ���̵尣 �Ÿ�
	//SYNTHESIZE_REF(float, m_fArrivalCost[3], ArrivalCost);

	//���� �̿��� �ﰢ���� ������
	//SYNTHESIZE_REF(Cell*, m_pNeighborCell[3], NeighborCell);

	//�� �ڽ�Ʈ
	SYNTHESIZE(float, m_fTotalCost, TotalCost);

	//���ۺ��� ��������� �ڽ�Ʈ
	SYNTHESIZE(float, m_fCostFromStart, CostFromStart);

	//���ۺ��� �������� �ڽ�Ʈ
	SYNTHESIZE(float, m_fCostToGoal, CostToGal);

	//�� Ÿ�� ���� ���� Ÿ��
	SYNTHESIZE(Cell*, m_pParentCell, ParentCell);

	//���� �ִ� Ÿ���̳�
	SYNTHESIZE(bool, m_bIsOpen, IsOpen);

	//Ÿ���� �Ӽ�(���, ���� �� ������ ���� ����)
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

	//�ﰢ�� ���� �ټ�
	void SetVertex(const D3DXVECTOR3* pVertex) {
		for (int i = 0; i < 3; i++)
		{
			m_vVertex[i] = pVertex[i];
		}
	}

	D3DXVECTOR3* GetVertex(){ return m_vVertex; }

	//���̵� ������ ���� �ټ�
	void SetVertexCenter(const D3DXVECTOR3* pVertexCenter) {
		for (int i = 0; i < 3; i++)
		{
			m_vVertexCenter[i] = pVertexCenter[i];
		}
	}

	D3DXVECTOR3* GetVertexCenter(){ return m_vVertexCenter; }

	//���� - ���̵尣 �Ÿ�
	void SetArrivalCost(const float* pArrivalCost)
	{
		for (int i = 0; i < 3; i++)
		{
			m_fArrivalCost[i] = pArrivalCost[i];
		}
	}

	float* GetArrivalCost(){ return m_fArrivalCost; }

	//���� �̿��� �ﰢ���� ������
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
	D3DXVECTOR3 vertex[3];		//�ϳ��� �ﰢ���� �� ���� ���� ����

	D3DXVECTOR3 vertexCenter[3];//���̵� ������ ����
	//
	float arrivalCost[3];		//���� - ���̵尣 �Ÿ�
	//
	Cell* neighborCell[3];	//���� �̿��� �ﰢ���� ������
	*/

};

