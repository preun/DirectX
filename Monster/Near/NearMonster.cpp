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
	//�θ��� �¾��� ȣ���ض�
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
		//���� Ÿ������ �Ǿ����� �����Լ��� ������ ���״� ī��Ʈ ���� ����
		//��ȯ���� �̷��� ����.
		

		//�÷��̾� ������ �⺻�ൿ����
		if (PCHARACTER->GetIsDead())
		{
			m_eState = MS_IDLE;
			ChangeAni();
			return;
		}

		if (!m_bIsSummon)	m_nTargetingCount++;

		//�÷��̾���� �Ÿ��� ����ϰ�
		float length = GetDistance(*m_pModel->GetPosition(), *CHARACTER->GetPosition());

		//��Ÿ��� �����
		if (length > RANGE(m_pMonsterStat))
		{
			//�ʵ���� A*�����Ͽ� �̵�
			if (!m_bIsSummon)
			{
				if (m_eState == MS_ATTACK)
				{
					m_eState = MS_MOVEFORATTACK;
					ChangeAni();
				}
				MoveForAttack();
			}
			//��ȯ���� 
			else
			{
				//������Ʈ�� �ִϸ��̼� ��ü �� �����Լ� ������ ���Ͽ� �Լ� ����
				if (m_eState == MS_ATTACK)
				{
					m_eState = MS_RUN;
					ChangeAni();
				}
				return;
			}
		}
		//��Ÿ� �ȿ� ������
		else
		{
			//�÷��̾���� ���⺤�͸� ���ϰ�
			ChangeRot();

			
			if (m_eState == MS_MOVEFORATTACK)
			{
				m_eState = MS_ATTACK;
				ChangeAni();
			}
			//�÷��̾� ���ݱ�� ����
			m_eState = MS_ATTACK;
			if (m_pModel->IsAnimationPercent(ATKSPEED(m_pMonsterStat)))
			{
				float tatalRate = PHYRATE(m_pMonsterStat) + MAGICRATE(m_pMonsterStat) + CHERATE(m_pMonsterStat);
				float tatalDamage = tatalRate * ATK(m_pMonsterStat);
				PCHARACTER->CalculDamage(tatalDamage);
			}
		}

		//200�� �ɵ��� ���̻��� ������ ���ٸ�
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

		//������ ���̴�.
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