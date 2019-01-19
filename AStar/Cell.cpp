#include "../../stdafx.h"
#include "Cell.h"


Cell::Cell()
: m_fTotalCost(0)
, m_fCostFromStart(0)
, m_fCostToGoal(0)
, m_pParentCell(NULL)
, m_bIsOpen(true)
{
}


Cell::~Cell()
{

}

void Cell::ZeroReset()
{
	m_fTotalCost = m_fCostFromStart = m_fCostToGoal = 0;
		m_pParentCell = NULL;
		m_bIsOpen = true;
		m_sAttribute = "none";
}