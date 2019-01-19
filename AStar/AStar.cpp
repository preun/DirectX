#include "../../stdafx.h"
#include "AStar.h"
#include "PathFind.h"
#include "Cell.h"

AStar::AStar()
{
}


AStar::~AStar()
{

}

void AStar::SetCurrentCell(vector<D3DXVECTOR3> Vertex)
{

	m_pPathFind = new PathFind;
	m_pPathFind->Setup(Vertex);

	m_vCurrentCell = m_pPathFind->GetNaviCell();
}

void AStar::SetCell(int MyCellIndex, int TargetIndex)
{
	//기본 초기화 단계
	//밑에서 셀들의 속성을 변경하는걸 되돌리는작업
	for (int i = 0; i < m_vCurrentCell.size(); i++)
	{
		m_vCurrentCell[i]->ZeroReset();
	}

	/*
	멤버 변수 확인용 주석

	SYNTHESIZE(float, m_fTotalCost, TotalCost);
	
	//시작부터 현재까지의 코스트
	SYNTHESIZE(float, m_fCostFromStart, CostFromStart);
	
	//시작부터 끝까지의 코스트
	SYNTHESIZE(float, m_fCostToGoal, CostToGal);
	
	//내 타일 오기 전의 타일
	SYNTHESIZE(Cell*, m_pParentCell, ParentCell);
	
	//갈수 있는 타일이냐
	SYNTHESIZE(bool, m_bIsOpen, IsOpen);
	*/
	//담아둔 벡터 클리어 하고 다시 담는다.
	m_vTotalList.clear();
	m_vOpenList.clear();
	m_vCloseList.clear();

	//시작타일
	//m_pStartCell = m_vCurrentCell[0];
	m_pStartCell = m_vCurrentCell[MyCellIndex];
	m_pStartCell->SetAttribute("start");
	
	//엔드타일
	//m_pEndCell = m_vCurrentCell[m_vCurrentCell.size() - 1];
	m_pEndCell = m_vCurrentCell[TargetIndex];
	m_pEndCell->SetAttribute("end");

	//현재
	m_pCurrentCell = m_pStartCell;

	for (int i = 0; i < m_vCurrentCell.size(); i++)
	{
		
		//나중에 나의 인덱스를 인자로 받아서 스타트로 설정

		//if (i == 0)
		//if (i == MyCellIndex)
		//{
		//	//m_vCurrentCell[i]->SetAttribute("start");
		//	m_vTotalList.push_back(m_vCurrentCell[i]);
		//}
		////일단 강제로 마지막 셀이 도착지
		//else if (i == m_vCurrentCell.size() - 1)
		//{
		//	//m_vCurrentCell[i]->SetAttribute("end");
		//	m_vTotalList.push_back(m_vCurrentCell[i]);
		//}
		////나머지 다 넣어준다
		//else
		//{
			m_vTotalList.push_back(m_vCurrentCell[i]);
		//}
	}
}

vector<Cell*> AStar::addOpenList(Cell* currentCell)
{
	//삼각형 하나에 인접 삼각형은 3개이므로
	for (int i = 0; i < 3; i++)
	{
		//인접 삼각형이 없다면 다음으로 넘어가라
		if (!currentCell->GetNeighborCell()[i]) continue;

		//인접 삼각형이 있다면 일단 템프변수를 반들고
		Cell* tempCell = currentCell->GetNeighborCell()[i];

		//갈수 없는 셀, 시작, 끝 타일은 제외한다.
		if (!tempCell->GetIsOpen()) continue;
		if (tempCell->GetAttribute() == "start") continue;
		if (tempCell->GetAttribute() == "wall") continue;

		//상위노드는 현재 셀이다.
		tempCell->SetParentCell(m_pCurrentCell);

		//체크용 불값
		bool addCell = true;
		for (m_viOpenList = m_vOpenList.begin();
			m_viOpenList != m_vOpenList.end(); ++m_viOpenList)
		{
			//한 번 체크 된 애는 중복 검출 하지 않는다
			if (*m_viOpenList == tempCell)
			{
				addCell = false;
				break;
			}
		}

		//이미 오픈리스트에 들어가있으면 컨티뉴
		if (!addCell) continue;

		m_vOpenList.push_back(tempCell);

	}

	return m_vOpenList;
}

void AStar::pathFinder(Cell* currentCell)
{
	float tempTotalCost = 5000;
	Cell* tempCell = NULL;

	for (int i = 0; i < addOpenList(currentCell).size(); ++i)
	{
		/*
		사이트에 있는것
		deltax = (목표점.x - 현제셀중점.x)
		deltay = (목표점.y - 현제셀중점.y)
		deltaz = (목표점.z - 현제셀중점.z)
		max(max(deltax, deltay), deltaz)

		일단 렝스값으로
		*/
		//목표점 - 현재 셀 중점
		D3DXVECTOR3 tempVec;
		tempVec = m_pEndCell->GetCenter() - m_vOpenList[i]->GetCenter();
		m_vOpenList[i]->SetCostToGal(
			D3DXVec3Length(&tempVec)
			);

		D3DXVECTOR3 center1 = m_vOpenList[i]->GetParentCell()->GetCenter();
		D3DXVECTOR3 center2 = m_vOpenList[i]->GetCenter();

		m_vOpenList[i]->SetCostFromStart(
			D3DXVec3Length(&(center1 - center2)));

		m_vOpenList[i]->SetTotalCost(m_vOpenList[i]->GetCostToGal()
			+ m_vOpenList[i]->GetCostFromStart());

		if (tempTotalCost > m_vOpenList[i]->GetTotalCost())
		{
			tempTotalCost = m_vOpenList[i]->GetTotalCost();
			tempCell = m_vOpenList[i];
		}

		//중복방지
		bool addObj = true;
		for (m_viOpenList = m_vOpenList.begin();
			m_viOpenList != m_vOpenList.end(); ++m_viOpenList)
		{
			if (*m_viOpenList == tempCell)
			{
				addObj = false;
				break;
			}
		}

		//검사 완료 셀
		m_vOpenList[i]->SetIsOpen(false);
		if (!addObj) continue;

		m_vOpenList.push_back(tempCell);
	}

		if (tempCell->GetAttribute() == "end")
		{
			while (m_pCurrentCell->GetParentCell() != NULL)
			{

				m_pCurrentCell = m_pCurrentCell->GetParentCell();
			}
			return;
		}
		m_vCloseList.push_back(tempCell);
		//_vDibugList.push_back(tempTile);

		for (m_viOpenList = m_vOpenList.begin();
			m_viOpenList != m_vOpenList.end(); ++m_viOpenList)
		{
			if (*m_viOpenList == tempCell)
			{
				m_viOpenList = m_vOpenList.erase(m_viOpenList);
				break;
			}
		}

		m_pCurrentCell = tempCell;
		pathFinder(m_pCurrentCell);
}

D3DXVECTOR3 AStar::GetNextCell(OUT vector<D3DXVECTOR3>* path)
{
	if (!DEBUG)	path->clear();
	if (m_pStartCell == m_pEndCell)
	{
		if (!DEBUG)	(*path).push_back(D3DXVECTOR3(-1, -1, -1));
		return D3DXVECTOR3(-1, -1, -1);
	}

	pathFinder(m_pCurrentCell);
	
	if (m_vCloseList.size() <= 0)
	{
		if(!DEBUG) (*path).push_back(D3DXVECTOR3(-1, -1, -1));
		return D3DXVECTOR3(-1, -1, -1);
	}

	if (!DEBUG)
	{
		
		//path->resize(m_vCloseList.size());

		for (int i = 0; i<m_vCloseList.size(); ++i)
		{
			(*path).push_back( m_vCloseList[i]->GetCenter());
		}

		(*path).push_back(D3DXVECTOR3(-1, -1, -1));
	}

	return m_vCloseList[0]->GetCenter();
}

int AStar::GetCellIndex(D3DXVECTOR3 pos)
{
	return m_pPathFind->GetCellIndex(pos);
}

void AStar::Render(int MyCellIndex, int TargetIndex, D3DXVECTOR3* pos)
{
	
	if (DEBUG)
	{
		SetCell(MyCellIndex, TargetIndex);
		GetNextCell(NULL);
		vector<D3DXVECTOR3> tempVector;
		tempVector.push_back(D3DXVECTOR3(m_vCurrentCell[MyCellIndex]->GetCenter().x, m_vCurrentCell[MyCellIndex]->GetCenter().y + 2 , m_vCurrentCell[MyCellIndex]->GetCenter().z));
		for (int i = 0; i < m_vCloseList.size(); i++)
		{
			tempVector.push_back(D3DXVECTOR3(m_vCloseList[i]->GetCenter().x, m_vCloseList[i]->GetCenter().y+2, m_vCloseList[i]->GetCenter().z));
		}

		tempVector.push_back(D3DXVECTOR3((*pos).x, (*pos).y+2, (*pos).z));
		tempVector.push_back(D3DXVECTOR3((*pos).x, (*pos).y+2, (*pos).z));
		tempVector.push_back(D3DXVECTOR3((*pos).x, (*pos).y+2, (*pos).z));
		D3DXMATRIX matW;
		D3DXMatrixIdentity(&matW);
		DEVICE->SetTransform(D3DTS_WORLD, &matW);
		//DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		DEVICE->SetTexture(0, NULL);
		DEVICE->DrawPrimitiveUP(D3DPT_LINESTRIP,
			//몇개의 삼각형인지
			tempVector.size()-2,
			//첫 주소
			&tempVector[0],
			//크기
			sizeof(D3DXVECTOR3));
	}
}