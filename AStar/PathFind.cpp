#include "../../stdafx.h"
#include "PathFind.h"
#include "Cell.h"

#define FONTSIZE 20
PathFind::PathFind()
{
}


PathFind::~PathFind()
{

}

void PathFind::Setup(vector<D3DXVECTOR3>& Vertex)
{
	//���ڰ� ������ ����ó��
	assert(Vertex.size() >= 0 && "���ڰ��� ���ͻ���� 0�Դϴ�.");

	//���ڰ� �����ŭ �޸� �Ҵ��ϰ� 
	m_vNaviCell.resize(Vertex.size() / 3);

	int index = 0;
	for (int i = 0; i < Vertex.size(); i += 3)
	{

		//�ϳ��� �ﰢ�� ���� �ֱ�
		//Cell* tempCell = new Cell;
		m_vNaviCell[index] = new Cell;

		D3DXVECTOR3 tempVertex[3];
		tempVertex[0] = Vertex[i];
		tempVertex[1] = Vertex[i+1];
		tempVertex[2] = Vertex[i+2];

		m_vNaviCell[index]->SetVertex(tempVertex);
	
		//m_vNaviCell[index]->vertex[0] = Vertex[i];
		//m_vNaviCell[index]->vertex[1] = Vertex[i+1];
		//m_vNaviCell[index]->vertex[2] = Vertex[i+2];

		//�ڵ� ���̰� ��������� ���� �˱� ���ϱ� ���� �̸� ����
		//D3DXVECTOR3 plusV = m_vNaviCell[index].vertex[0] + m_vNaviCell[index].vertex[1] + m_vNaviCell[index].vertex[2];
		D3DXVECTOR3 plusV =tempVertex[0] + tempVertex[1] + tempVertex[2];
		
		//float x = m_vNaviCell[index].vertex[0].x + m_vNaviCell[index].vertex[1].x + m_vNaviCell[index].vertex[2].x;
		//float y = m_vNaviCell[index].vertex[0].y + m_vNaviCell[index].vertex[1].y + m_vNaviCell[index].vertex[2].y;
		//float z = m_vNaviCell[index].vertex[0].z + m_vNaviCell[index].vertex[1].z + m_vNaviCell[index].vertex[2].z;

		//������ǥ 
		//m_vNaviCell[index].center =
		//	D3DXVECTOR3(plusV/3.0f);
		m_vNaviCell[index]->SetCenter(plusV / 3.0f);

		//���̵���� 3�� ������ ���� �ݺ���
		D3DXVECTOR3 tempVertexCenter[3];
		float tempArrivalCost[3];
		for (int j = 0; j < 3; j++)
		{
			//�ڵ� ���̰� ��������� ���� �˱� ���ϱ� ���� �̸� ����
			//D3DXVECTOR3 plusV2 = m_vNaviCell[index].vertex[j % 3] + m_vNaviCell[index].vertex[(j + 1) % 3];
			D3DXVECTOR3 plusV2 = tempVertex[j % 3] + tempVertex[(j + 1) % 3];

			//���̵� ������ ����
			//m_vNaviCell[index].vertexCenter[j] = plusV2 / 2.0f;
			tempVertexCenter[j] = plusV2 / 2.0f;

			//�������� ���̵� ���ͱ����� ���͸� ã��
			D3DXVECTOR3 costVector = m_vNaviCell[index]->GetCenter();
			costVector = costVector - tempVertexCenter[j];

			//������ ������ ��ƶ�
			//m_vNaviCell[index].arrivalCost[j] = D3DXVec3Length(&m_vNaviCell[index].vertexCenter[j]);
			tempArrivalCost[j] = D3DXVec3Length(&costVector);
		}	

		m_vNaviCell[index]->SetVertexCenter(tempVertexCenter);
		m_vNaviCell[index]->SetArrivalCost(tempArrivalCost);

		//m_vNaviCell[index] = tempCell;

		index++;
	}

	

	//������ �ﰢ�� �⺻ NULL �ʱ�ȭ
	//for (int i = 0; i < m_vNaviCell.size(); i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		m_vNaviCell[i]->m_pN[j] = NULL;
	//	}
	//}

	//�̿� ������ֱ�
	for (int i = 0; i < m_vNaviCell.size(); i++)
	{
		MakeNeighborCell(i);
		//i���� �ﰢ������ ���� �ﰢ���� �̰��̴�!!
		//m_vNaviCell[i].neighborCell[0] = MakeNeighborCell(m_vNaviCell[i].vertexCenter[0],i);
		//m_vNaviCell[i].neighborCell[1] = MakeNeighborCell(m_vNaviCell[i].vertexCenter[1], i);
		//m_vNaviCell[i].neighborCell[2] = MakeNeighborCell(m_vNaviCell[i].vertexCenter[2], i);
	}
	
	//����� �ߴ�����
	//int a = 10;
	
}

//void PathFind::Update()
//{
//	
//	D3DXVECTOR3 vertex[3];		//�ϳ��� �ﰢ���� �� ���� ���� ����
//	D3DXVECTOR3 center;			//�ﰢ���� ����
//	D3DXVECTOR3 vertexCenter[3]; //���̵� ������ ����
//
//	float arrivalCost[3];
//	
//	char test[1111];
//	sprintf_s(test, sizeof(test), "��1 : %f, %f, %f  ��2 : %f, %f, %f  ��3 : %f, %f, %f",
//		
//		m_vNaviCell[0].vertex[0].x, m_vNaviCell[0].vertex[0].y, m_vNaviCell[0].vertex[0].z,
//		m_vNaviCell[0].vertex[1].x, m_vNaviCell[0].vertex[1].y, m_vNaviCell[0].vertex[1].z,
//		m_vNaviCell[0].vertex[2].x, m_vNaviCell[0].vertex[2].y, m_vNaviCell[0].vertex[2].z
//		);
//	
//	TEXT->Add(test, 10, 10, FONTSIZE);
//	
//	sprintf_s(test, sizeof(test), "���� �� : %f, %f, %f",
//	
//		m_vNaviCell[0].center.x, m_vNaviCell[0].center.y, m_vNaviCell[0].center.z
//		);
//	
//	TEXT->Add(test, 10, 50, FONTSIZE);
//	
//	sprintf_s(test, sizeof(test), "���̵� ����1 : %f, %f, %f  ���̵� ����2 : %f, %f, %f  ���̵� ����3 : %f, %f, %f",
//	
//		m_vNaviCell[0].vertexCenter[0].x, m_vNaviCell[0].vertexCenter[0].y, m_vNaviCell[0].vertexCenter[0].z,
//		m_vNaviCell[0].vertexCenter[1].x, m_vNaviCell[0].vertexCenter[1].y, m_vNaviCell[0].vertexCenter[1].z,
//		m_vNaviCell[0].vertexCenter[2].x, m_vNaviCell[0].vertexCenter[2].y, m_vNaviCell[0].vertexCenter[2].z
//		);
//	
//	TEXT->Add(test, 10, 90, FONTSIZE);
//	
//	sprintf_s(test, sizeof(test), "��1�� cost : %f ��2�� cost : %f  ��3�� cost :  %f",
//	
//		m_vNaviCell[0].arrivalCost[0],
//		m_vNaviCell[0].arrivalCost[1],
//		m_vNaviCell[0].arrivalCost[2]
//		);
//	
//	TEXT->Add(test, 10, 130, FONTSIZE);
//	
//}

//void PathFind::Render()
//{
//	TEXT->Render();
//	���� ���ڿ�, ��ǥ, ���� ũ��
//	
//}

//ST_CELL* PathFind::MakeNeighborCell(D3DXVECTOR3 rayPos, int index)
//{
//	for (int i = 0; i < m_vNaviCell.size(); i++)
//	{
//		//�ڱ� �ڽ��̸� ��Ƽ��
//		if (i == index) continue;
//
//		
//		if (D3DXIntersectTri(
//			&m_vNaviCell[i].vertex[0],
//			&m_vNaviCell[i].vertex[1],
//			&m_vNaviCell[i].vertex[2],
//			&D3DXVECTOR3(rayPos.x+0.1f, 10000, rayPos.z+0.1f),
//			&D3DXVECTOR3(0, -1, 0),
//			NULL, NULL, NULL))
//		{
//			return &m_vNaviCell[i];
//
//			//return;
//		}
//	}
//
//	return NULL;
//
//}

void PathFind::MakeNeighborCell(int index)
{
	Cell* tempNeighborCell[3];
	for (int i = 0; i < 3; i++)
	{
		tempNeighborCell[i] = SubMakeNeighborCell(index)[i];
	}

	m_vNaviCell[index]->SetNeighborCell(tempNeighborCell);

	//m_vNaviCell[index]->m_pN[0] = SubMakeNeighborCell(index, 0);
	//m_vNaviCell[index]->neighborCell[1] = SubMakeNeighborCell(index, 1);
	//m_vNaviCell[index]->neighborCell[2] = SubMakeNeighborCell(index, 2);
	//for (int i = 0; i < m_vNaviCell.size(); i++)
		//	{
		//		//�ڱ� �ڽ��̸� ��Ƽ��
		//		if (i == index) continue;
		//
		//		
		//		if (D3DXIntersectTri(
		//			&m_vNaviCell[i].vertex[0],
		//			&m_vNaviCell[i].vertex[1],
		//			&m_vNaviCell[i].vertex[2],
		//			&D3DXVECTOR3(rayPos.x+0.1f, 10000, rayPos.z+0.1f),
		//			&D3DXVECTOR3(0, -1, 0),
		//			NULL, NULL, NULL))
		//		{
		//			return &m_vNaviCell[i];
		//
		//			//return;
		//		}
		//	}
		//
		//	return NULL;
}

Cell** PathFind::SubMakeNeighborCell(int myCellIndex)
{
	Cell* tempNeighborCell[3];
	
	for (int i = 0; i < 3; i++)
	{
		//�켱 �η� �ʱ�ȭ�ϰ�
		tempNeighborCell[i] = NULL;

		//�ʱ� ������ ��ġ�� ���̵������ ����
		//D3DXVECTOR3 rayPos = m_vNaviCell[myCellIndex].vertexCenter[linkCellIndex];
		D3DXVECTOR3 rayPos = m_vNaviCell[myCellIndex]->GetVertexCenter()[i];
		//rayPos.y = 1000;

		//�켱 �ٸ� ������ ��ǥ�� ����
		//1���� 2������ �̿��� ���� ã�� �� 3������ �̿��Ѵ�
		//D3DXVECTOR3 otherV = m_vNaviCell[myCellIndex].vertex[(linkCellIndex + 2) % 3];
		D3DXVECTOR3 otherV = m_vNaviCell[myCellIndex]->GetVertex()[(i + 2) % 3];

		//�ٸ� ���� - ���̵� �߾� �� ���� ���⺤�� ���ϰ�
		D3DXVECTOR3 dir = rayPos - otherV;
		//��ֶ�����
		D3DXVec3Normalize(&dir, &dir);
		//0.01��ȭ
		dir = dir * 0.01f;

		//������ ��ġ���� ���� �ణ ������ �̵�
		rayPos = rayPos + dir;
		rayPos.y = 10000;

		for (int j = 0; j < m_vNaviCell.size(); j++)
		{
			//�ڱ� �ڽ��̸� ��Ƽ��
			if (j == myCellIndex) continue;

			if (D3DXIntersectTri(
				&m_vNaviCell[j]->GetVertex()[0],
				&m_vNaviCell[j]->GetVertex()[1],
				&m_vNaviCell[j]->GetVertex()[2],
				&rayPos,
				&D3DXVECTOR3(0, -1, 0),
				NULL, NULL, NULL))
			{
				//�浹�� �Ͼ���� �浹�� ���� ����
				//�浹�� ���Ͼ���� �⺻ �ʱ�ȭ�� �η� ������ ���� �ȴ�.
				tempNeighborCell[i] = m_vNaviCell[j];
				break;
				//return;
			}

			//����� �ߴ�����
			//int a = 10;

			//if (D3DXIntersectTri(
			//	&m_vNaviCell[j]->vertex[0],
			//	&m_vNaviCell[j]->vertex[1],
			//	&m_vNaviCell[j]->vertex[2],
			//	&rayPos,
			//	&D3DXVECTOR3(0, -1, 0),
			//	NULL, NULL, NULL))
			//{
			//	return &m_vNaviCell[i];
			//
			//	//return;
			//}
		}

		//return NULL;
	}

	return tempNeighborCell;
}

int PathFind::GetCellIndex(D3DXVECTOR3 pos)
{
	for (int j = 0; j < m_vNaviCell.size(); j++)
	{

		if (D3DXIntersectTri(
			&m_vNaviCell[j]->GetVertex()[0],
			&m_vNaviCell[j]->GetVertex()[1],
			&m_vNaviCell[j]->GetVertex()[2],
			&D3DXVECTOR3(pos.x, 10000, pos.z),
			&D3DXVECTOR3(0, -1, 0),
			NULL, NULL, NULL))
		{
			//�浹�� �Ͼ���� �浹�� ���� ����
			return j;
		}
	}

	return -1;
}