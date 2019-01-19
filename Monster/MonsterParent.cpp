#include "MonsterUseHeader.h"
#include "MonsterParent.h"
#include "../AStar/AStar.h"
#include "../Item/DropManager.h"
#include "../SkillManager.h"
#include "Boss/BossParent.h"
#include "HitParticle.h"
#include "MonsterManager.h"

MonsterParent::MonsterParent()
	: m_pModel(NULL)
	, m_bSpecial(false)
	, m_pNpc(NULL)
{
}

MonsterParent::~MonsterParent()
{
	SAFE_DELETE(m_pAStar);
	SAFE_DELETE(m_pModel);


	m_pMap = NULL;
	m_ppCharacter = NULL;
	m_pDropManager = NULL;
	m_pNpc = NULL;
	SAFE_DELETE(m_pSkill);
	SAFE_DELETE(m_pMonsterStat);
	SAFE_DELETE(m_pDamageUI);
	SAFE_DELETE(m_pHitParticle);
	SAFE_DELETE(m_pHPBar);

	/*
	GET(Model*, m_pModel, Model);
	SET(CharacterParant**, m_ppCharacter, Character);
	SET(DropManager*, m_pDropManager, DropManager);
	*/
}

void MonsterParent::Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon)
{
	//�ʱ�ȭ
	m_nPrePosIndex = -1;

	m_bIsSummon = isSummon;

	m_vDir = D3DXVECTOR3(0, 0, 1);

	m_nCount = NRand(0, 10);
	m_nPatternChangeCount = m_bKeyMonster= m_bIsSpeedwagon = 0;
	m_bUsingSkill =  false;

	m_pAStar = new AStar;
	m_pAStar->SetCurrentCell(map->GetNavMesh());
	//m_pAStar->SetCell(0, 0);
	//D3DXVECTOR3 temp =  m_pAStar->GetNextCell();

	m_nResPawnCount = m_bIsRespawn = m_bAbleSkill = 0;
	m_eState = MS_IDLE;
	m_pMap = map;
	m_pModel->SetPosition(D3DXVECTOR3(spawnPos.x, m_pMap->GetHeight(spawnPos.x, spawnPos.z), spawnPos.z));
	ChangeAni();

	m_pMonsterStat = new STATUS;
	SetupStat();
	SetupSkill();
	DropItemSetup();
	m_fAlphaCount = 0;

	m_pHPBar = new UIObject;

	//TEXTUREMANAGER->AddTexture("MonBackBar", "./Model/Enemy/UI/���.jpg");
	//TEXTUREMANAGER->AddTexture("MonFrontBar", "./Model/Enemy/UI/����Ʈ��.jpg");

	m_pHPBar->SetTexture(TEXTUREMANAGER->GetTexture("MonFrontBar"));

	D3DXVECTOR3 UIPos = *m_pModel->GetPosition();

	UIPos.x -= m_fUIMoveX;
	UIPos.y -= m_fUIMoveY;

	auto temp = Convert3DTo2D(UIPos);
	UIPos.x = temp.x;
	UIPos.y = temp.y;
	UIPos.z = 0;

	m_pHPBar->SetPosition(UIPos);

	UIObject* backBar = new UIObject;

	backBar->SetPosition(D3DXVECTOR3(0, 0, 0.1));
	backBar->SetTexture(TEXTUREMANAGER->GetTexture("MonBackBar"));


	m_pHPBar->AddChild(backBar);

	m_pDamageUI = new DamageUI;
	m_pDamageUI->Setup(false);

	m_pHitParticle = new HitParticle;
	m_pHitParticle->Setup("���ͱ⺻�ǰ�");
	//m_pHitParticle = PARTICLE->GetParticle("���ͱ⺻�ǰ�");

	//npc ���� �οﰪ ���� �Լ�
	IsAppear();

	MoveReset(false, m_nMaxMoveCount, m_nMinMoveCount);

	if (isSummon) m_bAppearNPC = false;
	//ST_SIZEBOX box;
}

void MonsterParent::SetupBoss(Map* map, D3DXVECTOR3 pos)
{
	m_pHitParticle = NULL;
	m_pMap = map;
	m_pModel->SetPosition(D3DXVECTOR3(pos.x, m_pMap->GetHeight(pos.x, pos.z), pos.z));
	m_pMonsterStat = new STATUS;

	SetupStat();
	SetupSkill();

	m_pHPBar = new UIObject;

	//TEXTUREMANAGER->AddTexture("BossBackBar", "./Model/Enemy/UI/�������.jpg");
	//TEXTUREMANAGER->AddTexture("BossFrontBar", "./Model/Enemy/UI/��������Ʈ��.jpg");

	m_pHPBar->SetTexture(TEXTUREMANAGER->GetTexture("BossFrontBar"));

	m_pHPBar->SetPosition(D3DXVECTOR3(0,0,0));

	UIObject* backBar = new UIObject;

	backBar->SetPosition(D3DXVECTOR3(0, 0, 0.1));
	backBar->SetTexture(TEXTUREMANAGER->GetTexture("BossBackBar"));
	m_fAlphaCount = m_bAbleSkill = m_bIsSpeedwagon = 0;

	m_pHPBar->AddChild(backBar);

	m_pDamageUI = new DamageUI;
	m_pDamageUI->Setup(false);

	m_bAppearNPC = false;
	m_bKeyMonster = true;
}

void MonsterParent::SetupStat()
{

}

void MonsterParent::Update()
{
	
	//if (INPUT->KeyDown('L'))
	//{
	//	
	//	
	//	m_eState = MS_SKILL;
	//	ChangeAni();
	//}
	m_fAlphaCount += TIME->GetElapsedTime();

	if (m_fAlphaCount >= 0.2) //m_pModel->SetShaderAlpha(1.0f);
		m_pModel->SetShaderColorOffset(0);

	if (m_bIsTargeting)
	{
		float tempF = (float)CURRENTHP(m_pMonsterStat) / MAXHP(m_pMonsterStat);


		m_pHPBar->SetScale(D3DXVECTOR3(tempF, 1, 1));

		D3DXVECTOR3 UIPos = *m_pModel->GetPosition();
		UIPos.y += m_fUIMoveY;

		auto temp = Convert3DTo2D(UIPos);
		UIPos.x = temp.x - m_fUIMoveX;
		UIPos.y = temp.y;
		UIPos.z = 0;
		m_pHPBar->SetPosition(UIPos);

		m_pHPBar->Update();

	}

	if (!DEBUG)
	{
		m_fSkillCoolTimeCount += TIME->GetElapsedTime();

		if(!m_bIsSpeedwagon)	m_bAbleSkill = AbleSkill();

		if (m_bAbleSkill)
		{
			if (m_eState != MS_SKILL)
			{
				m_eState = MS_SKILL;
				ChangeAni();
			}
			m_eState = MS_SKILL;
			m_bAbleSkill = false;
			return;
		}

		if (INPUT->KeyDown('O'))
		{
			m_eState = MS_ATTACK;
		}
		switch (m_eState)
		{
			//�⺻ �� �����̴� �����϶� move�Լ� ȣ���ؼ� �ൿ
		case MS_IDLE: case MS_RUN:
		{
			
			if (!m_bIsSummon)	  Move();

			//��ȯ���̸� ��ȯ�� �̵�
			else
			{
				int a = 10;
				SummonMove();
			}
		}
			break;
			//��ų�����̸� ��ų���� �Լ� ȣ��
		case MS_SKILL:
		{
						 SkillUse();
		}
			break;
			//���ݻ����̸� ���ݻ��� ȣ��
		case MS_ATTACK: case MS_MOVEFORATTACK:
		{
							Attack();
		}
			break;
		default:
			break;
		}


		if (!m_bIsRespawn)
		{
			if (m_pModel)
			{
				m_pModel->World();
				m_pModel->Update();
			}
		}

		m_pSkill->Update();

		if (m_pHitParticle)
		{
			D3DXVECTOR3 tempVec = *m_pModel->GetPosition();
			float rotY = 0;
			if (!m_bSpecial)
			{				
				rotY = GetAngle(m_pModel->GetPosition()->x, m_pModel->GetPosition()->z,
					CHARACTER->GetPosition()->x, CHARACTER->GetPosition()->z);

				rotY -= D3DX_PI / 2;

				tempVec.y += 2.0f;				
			}

			m_pHitParticle->Update(tempVec, rotY);

			
		}
	}
	//ChangeAni();
}

void MonsterParent::RespawnUpdate()
{
	//char test[111];
	//
	//sprintf_s(test, sizeof(test), "%d", m_nResPawnCount);
	//TEXT->Add(test, 10, 10, 20);
	if (m_bIsTargeting)
	{
		float tempF = (float)CURRENTHP(m_pMonsterStat) / MAXHP(m_pMonsterStat);


		m_pHPBar->SetScale(D3DXVECTOR3(tempF, 1, 1));

		D3DXVECTOR3 UIPos = *m_pModel->GetPosition();
		UIPos.y += m_fUIMoveY;

		auto temp = Convert3DTo2D(UIPos);
		UIPos.x = temp.x - m_fUIMoveX;
		UIPos.y = temp.y;
		UIPos.z = 0;
		m_pHPBar->SetPosition(UIPos);

		m_pHPBar->Update();

	}

	if (m_pHitParticle)
	{
		D3DXVECTOR3 tempVec = *m_pModel->GetPosition();
		float rotY = 0;
		if (!m_bSpecial)
		{
			rotY = GetAngle(m_pModel->GetPosition()->x, m_pModel->GetPosition()->z,
				CHARACTER->GetPosition()->x, CHARACTER->GetPosition()->z);

			rotY -= D3DX_PI / 2;

			tempVec.y += 2.0f;
		}

		m_pHitParticle->Update(tempVec, rotY);


	}

	m_nResPawnCount++;

	m_pModel->World();
	m_pModel->Update();

	if (m_eState == MS_DIE)
	{
		m_pModel->SetShaderFadeCut(m_pModel->GetCurrentAnimationPercent());

		if (m_pModel->IsAnimationEnd())
		{
			m_eState = MS_NONE;
			ItemDrop();
		}
	}
	//if (m_pModel->IsAnimationEnd() && m_eState == MS_DIE)
	//{
	//	m_eState = MS_NONE;
	//	//ChangeAnimation();
	//}

	if (m_nResPawnCount >= 300)
	{
		m_bIsRespawn = false;
		m_nResPawnCount = 0;
	}
}
void MonsterParent::Render()
{
	/*

	D3DXVECTOR3 UIPos = *m_pModel->GetPosition();
	//UIPos.x -= m_fUIMoveX;
	//UIPos.y += m_fUIMoveY;

	auto temp = Convert3DTo2D(UIPos);
	UIPos.x = temp.x - m_fUIMoveX;
	UIPos.y = temp.y - m_fUIMoveY;
	UIPos.z = 0;
	m_pHPBar->SetPosition(UIPos);
	*/
	if (m_pModel && m_eState != MS_NONE)
	{
		m_pDamageUI->Render(*m_pModel->GetPosition());
		m_pModel->Render();
		if (DEBUG)
		{
			POINT temp = MoveForAttack();

			m_pAStar->Render(temp.y, temp.x, CHARACTER->GetPosition());
		}

		if (m_eState == MS_SKILL)
		{
			m_pSkill->Render();
		}

		//UIPos.x = temp.x - m_fUIMoveX;
		//UIPos.y = temp.y - m_fUIMoveY;
		//UIPos.z = 0;
		//m_pHPBar->SetPosition(UIPos);

		
			if (m_bIsTargeting) m_pHPBar->Render();

			if (m_pHitParticle) m_pHitParticle->Render();
		
	}


}

void MonsterParent::ChangeAni()
{
	switch (m_eState)
	{
	case MS_IDLE:
		m_pModel->SetAnimation("IDLE");
		break;
	case MS_RUN: case MS_MOVEFORATTACK:
		m_pModel->SetAnimation("RUN");
		break;
	case MS_SKILL:
		m_pModel->SetAnimation("SKILL");
		break;
	case MS_ATTACK:
		m_pModel->SetAnimation("ATTACK");
		break;
	case MS_DIE:
		m_pModel->SetAnimation("DIE");
		break;
	default:
		break;
	}
}

void MonsterParent::ChangeRot()
{
	D3DXVECTOR3 dir =
		*CHARACTER->GetPosition() - *m_pModel->GetPosition();

	//������ ���ϰ�
	float angle = GetAngle(0, 0, dir.x, dir.z);


	angle -= D3DX_PI / 2;

	//������ŭ �����̼�
	m_pModel->SetRotation(D3DXVECTOR3(0, angle, 0));
}

void MonsterParent::Respawn(D3DXVECTOR3 spawnPos)
{
	m_nResPawnCount = m_bIsRespawn = m_nTargetingCount = 0;
	m_eState = MS_IDLE;
	ChangeAni();
	CURRENTHP(m_pMonsterStat) = MAXHP(m_pMonsterStat);
	m_bIsTargeting = false;
	m_nPrePosIndex = -1;
	IsAppear();

	m_pModel->SetPosition(D3DXVECTOR3(spawnPos.x, m_pMap->GetHeight(spawnPos.x, spawnPos.z), spawnPos.z));

	m_pModel->SetShaderFadeCut(0);
}

void MonsterParent::MakeKeyMonster()
{
	m_bKeyMonster = true;
	m_pModel->SetScale(D3DXVECTOR3(0.035f, 0.035f, 0.035f));
	(*m_pMonsterStat) *= 3;
}

bool MonsterParent::IsDie()
{
	if (m_bKeyMonster && m_eState == MS_NONE) return true;
	return false;
}

void MonsterParent::ShowStat()
{
	/*
		int		nCurrentHP;		//ĳ����(��, �÷�
		int		nMaxHp;			//ĳ����(��, �÷�
	
		float	nCurrentStam;	//ĳ���� ���� ��
		int		nMaxStam;		//ĳ���� ��ü ��
	
		int		nAtk;			//�⺻���ݷ�
		float	fPhyRate;		//�������
		float	fMagicRate;		//�������
		float	fCheRate;		//ȭ�а��
		float	fAtkSpeed;		//���ݼӵ� (���
	
		int		nDef;			//����
		float	fAgi;			//ȸ�Ƿ�
		float	fHit;			//���߷�
		float	fSpeed;			//�̵��ӵ�
		float	fRange;			//���� �Ÿ�
		float	fScale;			//���� ����
	*/
	char temp[1111];
	sprintf_s(
		temp,
		sizeof(temp),
		"HP			: %d / %d \n \
		���ݷ�		: %d \n \
		����		: %d \n \
		���ݰŸ�	: %f \n \
		Ű����	: %d "
		, CURRENTHP(m_pMonsterStat)
		, MAXHP(m_pMonsterStat)
		, ATK(m_pMonsterStat)
		, DEF(m_pMonsterStat)
		, RANGE(m_pMonsterStat)
		, m_bKeyMonster
	);

	TEXT->Add(temp, 10, 140, 30);
}



void MonsterParent::CalculDamage(float damage, int* deathCount)
{
	
	/*
	float	fPhyRate;		//�������
	float	fMagicRate;		//�������
	float	fCheRate;		//ȭ�а��
	*/
	float totalRate =
		PHYRATE(m_pMonsterStat) +
		CHERATE(m_pMonsterStat) +
		MAGICRATE(m_pMonsterStat);

	float totalDamage = totalRate * DEF(m_pMonsterStat);

	totalDamage = damage - totalDamage;

	totalDamage /= 3;

	if (totalDamage <= 1) totalDamage = 1;

	totalDamage = round(totalDamage);

	SetCurrentHP(totalDamage, deathCount);

}

void MonsterParent::Attack()
{

}

void MonsterParent::SkillUse()
{

}

void MonsterParent::Move()
{

}

void MonsterParent::SummonMove()
{

}

void MonsterParent::MoveReset(bool isReverse, int max, int min)
{
	if (!isReverse)
	{

		m_vDir = D3DXVECTOR3(0, 0, -1);
		srand(time(NULL));

		m_nPatternChangeCount = NRand(min, max);

		if (m_eState == MS_IDLE)
		{
			float temp = D3DXToRadian( NRand(0,179) - 90);

			D3DXMATRIX matRotY;
			D3DXMatrixRotationY(&matRotY, temp);

			D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRotY);

			D3DXVec3Normalize(&m_vDir, &m_vDir);

			m_pModel->SetRotation(D3DXVECTOR3(0, temp, 0));
			m_eState = MS_RUN;
			//ChangeAni();
		}
		else
		{
			m_eState = MS_IDLE;
		}
		ChangeAni();
		m_nCount = 0;
	}
	else
	{
		m_vDir.x = -m_vDir.x; m_vDir.z = -m_vDir.z;
		m_pModel->SetRotation(*m_pModel->GetRotation() + D3DXVECTOR3(0, D3DX_PI, 0));
	}
}

POINT MonsterParent::MoveForAttack()
{
	m_eState = MS_MOVEFORATTACK;

	int playerIndex = m_pAStar->GetCellIndex(*CHARACTER->GetPosition());

	int myIndex = m_pAStar->GetCellIndex(*m_pModel->GetPosition());

	if (DEBUG) { return{ playerIndex, myIndex }; }

	D3DXVECTOR3 dir;
	//���� ���� ������
	if (playerIndex == myIndex)
	{
		dir = *CHARACTER->GetPosition()
			- *m_pModel->GetPosition();
	}
	//���� ���� �ƴϸ� ���̽�Ÿ ������ �Ѵ�.
	else
	{
		//assert(playerIndex != -1 && "���� �÷��̾ �� �� ���� ���� ������ �մϴ�.");

		//�÷��̾ ���� ���� ���� �������� ����ó��
		if (playerIndex == -1 || myIndex == -1)
		{
			m_eState = MS_IDLE;


			return { -1, -1 };
		}
	
		//���� ����ִ� �÷��̾��� �ε����� ���� ��� �� ����� �ٸ��ٸ� A*�� ������.
		if (m_nPrePosIndex != playerIndex)
		{
			//�������� ������ ���� �ʱ����� �� �ٲ��ְ�
			m_nPrePosIndex = playerIndex;
			//���̽�Ÿ ���� ���ְ�
			m_pAStar->SetCell(myIndex, playerIndex);

			//��θ� �����´�.
			D3DXVECTOR3 nextCell = m_pAStar->GetNextCell(&m_vPath);
		}

		//0���� �ε����� -1,-1,-1�̸� �ٷ� �̵�����.
		if ( m_vPath[0] == D3DXVECTOR3(-1, -1, -1))
		{			
				dir = *CHARACTER->GetPosition()
					- *m_pModel->GetPosition();			

		}
		//�װ� �ƴ϶�� ������ ���� ���� �Ǵ� ���� �����̴�.
		else
		{
			dir = m_vPath[0] - *m_pModel->GetPosition();
		}
	}
	D3DXVec3Normalize(&dir, &dir);
	if (!DEBUG)
	{

		float angle = GetAngle(0, 0, dir.x, dir.z);


		angle -= D3DX_PI / 2;

		
			D3DXVECTOR3 tempPos = *m_pModel->GetPosition() + dir* SPEED(m_pMonsterStat);
		tempPos.y = m_pMap->GetHeight(tempPos.x, tempPos.z);

		//������ ���̴�.
		if (tempPos.y < 0)
		{
			//���⼭ ���ڸ������� �ǰ���
			//MoveReset(true);
		}
		else
		{
			m_pModel->SetRotation(D3DXVECTOR3(0, angle, 0));
			m_pModel->SetPosition(tempPos);
			if (playerIndex != myIndex && m_vPath[0] != D3DXVECTOR3(-1, -1, -1))
			{
				if (myIndex != m_pAStar->GetCellIndex(*m_pModel->GetPosition()))
				{
					m_vPath.erase(m_vPath.begin());
				}
			}
		}
		
		//m_pModel->SetPosition(*m_pModel->GetPosition() + dir* SPEED(m_pMonsterStat));

		
	}
	return{ -1, -1 };
}

void MonsterParent::DropItemSetup()
{

}

void MonsterParent::ItemDrop()
{
	//��������������� ������� �����Ƿ�
	if (m_vItemID.size() <= 0) return;
	//���� ������ ��ȯ�ϴ� ���� �������� ������ �ʴ´�.
	if (!m_pDropManager) return;

	srand(time(NULL));

	//0~99��
	int ranNum = rand() % 100;

	float chancePerNum = 100.0f / m_vItemID.size();
	
	for (int i = 0; i < m_vItemID.size(); i++)
	{
		if (ranNum < chancePerNum * (i + 1))
		{
			m_pDropManager->AddDropItem(m_vItemID[i], *m_pModel->GetPosition());
			break;
		}
	}

}
void MonsterParent::SetCurrentHP(int hp, int* deathCount)
{
	{
		if (m_eState != MS_ATTACK && m_eState != MS_DIE && m_eState != MS_SKILL)
		{
			m_eState = MS_ATTACK;
			ChangeAni();
		}

		if (m_pHitParticle)
		{
			
			D3DXVECTOR3 tempVec = *m_pModel->GetPosition();
			float rotY = 0;;
			if (!m_bSpecial)			{
				
				rotY = GetAngle(m_pModel->GetPosition()->x, m_pModel->GetPosition()->z,
					CHARACTER->GetPosition()->x, CHARACTER->GetPosition()->z);

				rotY -= D3DX_PI / 2;

				tempVec.y += 2.0f;

			//	m_pHitParticle->AddParticle(tempVec, rotY);
			}
				m_pHitParticle->AddParticle(tempVec,rotY);
			

			//D3DXVECTOR3 tempPos = *m_pModel->GetPosition();
			////tempPos.y += 1.5f;
			//m_pHitParticle->SetPosition(tempPos);
			//m_pHitParticle->TimeReset();
			
		}

		m_pModel->SetShaderRimColor(D3DXVECTOR3(255, 0, 0));
		m_pModel->SetShaderRimPower(1.0f);

		//m_pModel->SetShaderAlpha(0.5f);

		m_pModel->SetShaderColorOffset(0.5);

		m_fAlphaCount = 0;

		m_pDamageUI->AddDamage(hp);

		CURRENTHP(m_pMonsterStat) -= hp;
		m_bIsTargeting = true;
		m_nTargetingCount = 0;
		if (CURRENTHP(m_pMonsterStat) <= 0)
		{
			if(deathCount)
			(*deathCount)++;

			if (m_bAppearNPC && m_pNpc && !m_pNpc->GetIsAppear())
				m_pNpc->Init(*m_pModel->GetPosition());

			m_pModel->SetShaderRimPower(0.0f);

			//m_pModel->SetShaderAlpha(1.0f);
			m_pModel->SetShaderColorOffset(0);

			CURRENTHP(m_pMonsterStat) = 0;


			m_bIsRespawn = true;
			m_eState = MS_DIE;
			ChangeAni();			
		}
	}
}
void MonsterParent::SetupSkill()
{
}

bool MonsterParent::AbleSkill()
{
	float leng = GetDistance(*m_pModel->GetPosition(), *CHARACTER->GetPosition());

	//��Ÿ���� ���Ұ� ��ų�� ��밡���Ҷ�
	//�׸��� ���� Ÿ������ �Ǿ����� ��ų ���
	if (m_fSkillCoolTimeCount >= m_nSkillCooltime
		&& leng <= m_stSkill.fMaxLength
		&& m_bIsTargeting
		&& !m_bIsRespawn
		&& !m_bUsingSkill
		&& m_pModel->IsAnimationEnd())
	{
		
		//m_stSkill.fEffectTime = leng / tempEffect.s

		ChangeRot();
		m_bUsingSkill = true;
		SkillPrepare();
		return true;
	}

	return false;
}

void MonsterParent::SkillPrepare(){

	m_pSkill->Prepare(PCHARACTER,
		this,
		NULL,
		m_stSkill,
		SKILLO_MONSTER);

}

void MonsterParent::IsAppear()
{
	//m_bAppearNPC = true;
	float rnd = FRand(0, 1);

	m_bAppearNPC = true;

	//if (rnd <= 0.05)
	//{
	//	if (!m_pNpc || !m_pNpc->GetIsAppear())
	//		m_bAppearNPC = true;
	//	else
	//		m_bAppearNPC = false;
	//}
	//else
	//	m_bAppearNPC = false;
}
