#include "../../../stdafx.h"
#include "NearMonster.h"
#include "../MonsterUseHeader.h"

NearMonster::NearMonster()
{
}


NearMonster::~NearMonster()
{

}

void NearMonster::Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon)
{
	//부모의 셋업을 호출해라
	MonsterParent::Setup(map, spawnPos, isSummon);
}

void NearMonster::SetupStat()
{

}

void NearMonster::SetupSkill()
{

}

void NearMonster::Attack()
{
	if (PCHARACTER)
	{
		//내가 타겟팅이 되었으면 공격함수가 실행이 될테니 카운트 증가 시작
		//소환몹은 이런거 없다.
		

		//플레이어 죽으면 기본행동으로
		if (PCHARACTER->GetIsDead())
		{
			m_eState = MS_IDLE;
			ChangeAni();
			return;
		}

		if (!m_bIsSummon)	m_nTargetingCount++;

		//플레이어와의 거리를 계산하고
		float length = GetDistance(*m_pModel->GetPosition(), *CHARACTER->GetPosition());

		//사거리를 벗어나면
		if (length > RANGE(m_pMonsterStat))
		{
			//필드몹은 A*적용하여 이동
			if (!m_bIsSummon)
			{
				if (m_eState == MS_ATTACK)
				{
					m_eState = MS_MOVEFORATTACK;
					ChangeAni();
				}
				MoveForAttack();
			}
			//소환몹은 
			else
			{
				//스태이트와 애니메이션 교체 후 무브함수 실행을 위하여 함수 종료
				if (m_eState == MS_ATTACK)
				{
					m_eState = MS_RUN;
					ChangeAni();
				}
				return;
			}
		}
		//사거리 안에 있으면
		else
		{
			//플레이어와의 방향벡터를 구하고
			ChangeRot();

			
			if (m_eState == MS_MOVEFORATTACK)
			{
				m_eState = MS_ATTACK;
				ChangeAni();
			}
			//플레이어 공격기능 설정
			m_eState = MS_ATTACK;
			if (m_pModel->IsAnimationPercent(ATKSPEED(m_pMonsterStat)))
			{
				float tatalRate = PHYRATE(m_pMonsterStat) + MAGICRATE(m_pMonsterStat) + CHERATE(m_pMonsterStat);
				float tatalDamage = tatalRate * ATK(m_pMonsterStat);
				PCHARACTER->CalculDamage(tatalDamage);
			}
		}

		//200이 될동안 더이상의 공격이 없다면
		if (m_nTargetingCount >= 200)
		{
			m_pModel->SetShaderRimPower(0.0f);
			m_eState = MS_IDLE;
			ChangeAni();
			m_nTargetingCount = 0;
			m_bIsTargeting = false;
		}
	}
}

void NearMonster::SkillUse()
{

}

void NearMonster::Move()
{

	if (m_nCount >= m_nPatternChangeCount)
	{
		MoveReset(false, m_nMaxMoveCount, m_nMinMoveCount);
	}

	if (m_eState == MS_RUN)
	{
		D3DXVECTOR3 tempPos = *m_pModel->GetPosition() + m_vDir* SPEED(m_pMonsterStat);
		tempPos.y = m_pMap->GetHeight(tempPos.x, tempPos.z);

		//못가는 곳이다.
		if (tempPos.y < 0)
		{
			MoveReset(true);
		}
		else
		{
			m_pModel->SetPosition(tempPos);
		}

	}

	m_nCount++;
}

void NearMonster::SummonMove()
{
	if (m_eState == MS_RUN)
	{
		m_vDir = *CHARACTER->GetPosition() - *m_pModel->GetPosition();
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		D3DXVECTOR3 tempPos = *m_pModel->GetPosition() + m_vDir* SPEED(m_pMonsterStat);
		tempPos.y = m_pMap->GetHeight(tempPos.x, tempPos.z);

		ChangeRot();

		m_pModel->SetPosition(tempPos);

		float length = GetDistance(*m_pModel->GetPosition(), *CHARACTER->GetPosition());

		if (length < RANGE(m_pMonsterStat))
		{
			m_eState = MS_ATTACK;
			ChangeAni();
		}
	}
}

void NearMonster::DropItemSetup()
{

}