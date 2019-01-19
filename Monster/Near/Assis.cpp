#include "../MonsterUseHeader.h"
#include "Assis.h"

Assis::Assis()
{
}


Assis::~Assis()
{

}

void Assis::Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon)
{
	//�𵨺��� �����
	//MODELMANAGER->AddModel("�ƽý�", "Model/Enemy/Assis/", "Assis.x", MODELTYPE_X);
	m_pModel = MODELMANAGER->GetModel("�ƽý�", MODELTYPE_X);

	m_fUIMoveX = 50.0f;
	m_fUIMoveY = 3.5f;

	//m_bIsTargeting = true;
	//�θ��� �¾��� ȣ���ض�
	NearMonster::Setup(map, spawnPos, isSummon);

	

	//MODELMANAGER->AddModel("�����ں���", "Model/Enemy/Assis/", "Assis.x", MODELTYPE_X);
	//MODELMANAGER->AddModel("�����ں���", "Model/Enemy/DarkHell/", "DarkHell.x", MODELTYPE_X);
	//MODELMANAGER->AddModel("�����ں���", "Model/Enemy/PhantomKnight/", "PhantomKnight.x", MODELTYPE_X);
	//MODELMANAGER->AddModel("�����ں���", "Model/Enemy/Nike/", "Nike.x", MODELTYPE_X);



	//���� �ڽ� 
	ST_SIZEBOX box;
	box.highX = 50.0f;
	box.highY = 120.0f;
	box.highZ = 50.0f;
	box.lowX = -50.0f;
	box.lowY = 10.0f;
	box.lowZ = -50.0f;


	m_pModel->SetScale(D3DXVECTOR3(0.02f, 0.02f, 0.02f));

	m_pModel->CreateBound(box);
	m_pModel->SetBoundSphere(m_pModel->GetOrigBoundSphere().center, 90.0f);

	//��ų
	m_pSkill = SKILL->GetSkill("Assis Skill");
	//SAFE_DELETE(m_pHitParticle);
	m_pHitParticle->Setup("����");
	//m_pHitParticle = PARTICLE->GetParticle("����");
	m_bSpecial = true;
}

void Assis::SetupStat()
{
	m_nMinMoveCount = NRand(500,600);
	m_nMaxMoveCount = NRand(200,300);

	ZeroMemory(m_pMonsterStat, sizeof(STATUS));

	CURRENTHP(m_pMonsterStat) = MAXHP(m_pMonsterStat) = 150;
	ATK(m_pMonsterStat) = 20;
	PHYRATE(m_pMonsterStat) = 1.3f;
	MAGICRATE(m_pMonsterStat) = 0.8f;
	CHERATE(m_pMonsterStat) = 1.1f;
	ATKSPEED(m_pMonsterStat) = 0.75f;

	DEF(m_pMonsterStat) = 5;
	AGI(m_pMonsterStat) = 10.0f;
	HIT(m_pMonsterStat) = 10.0f;
	SPEED(m_pMonsterStat) = 0.08f;
	RANGE(m_pMonsterStat) = 2.2f;

}

void Assis::SetupSkill()
{
	ZeroMemory(&m_stSkill, sizeof(ST_SKILL));
	m_stSkill.fDamage = 0;
	m_stSkill.fDamageDelay = 0;
	m_stSkill.fDamageInterval = 0;
	m_stSkill.fMaxLength = 0;
	m_stSkill.fAngle = 0;
	m_stSkill.nMaxTarget = 5;
	m_stSkill.nDamageCount = 0;
	m_stSkill.isAutoRot = true;
	m_stSkill.fYOffset = 1;
	m_stSkill.fBuffTime = 30;
	m_stSkill.fParticleTime = m_pModel->GetAnimationPeriod("SKILL");
	m_stSkill.fParticleSpeed = 0;
	m_stSkill.fEffectTime = 3;

	m_stSkill.buffStatus.chr.nAtk = ATK(m_pMonsterStat);

	m_fSkillCoolTimeCount = 0;
	m_nSkillCooltime = 100;
	//���ݷ� 2��
}


//���� ���� �����Լ�
//void Assis::Attack()
//{
//	
//}

//���� ���� ��ų�Լ�
void Assis::SkillUse()
{
	//SkillPrepare();

	m_pSkill->Trigger();

	if (m_pModel->IsAnimationEnd())
	{
		m_bUsingSkill = false;
		m_fSkillCoolTimeCount = 0;
		m_eState = MS_ATTACK;
		ChangeAni();
	}
}

//���� ���� �⺻ �̵��Լ�
//void Assis::Move()
//{
//	if (m_nCount == m_nPatternChangeCount/*INPUT->KeyDown('O')*/)
//	{
//		MoveReset(false, 500, 100);
//	}
//	//char ttest[111];
//	//sprintf_s(ttest, sizeof(ttest), "%f, %f, %f", m_vDir.x, m_vDir.y, m_vDir.z);
//	//TEXT->Add(ttest, 10, 10, 30);
//
//	if (m_eState == MS_RUN)
//	{
//		D3DXVECTOR3 tempPos = *m_pModel->GetPosition() + m_vDir* SPEED(m_pMonsterStat);
//		tempPos.y = m_pMap->GetHeight(tempPos.x, tempPos.z);
//
//		//������ ���̴�.
//		if (tempPos.y < 0)
//		{
//			MoveReset(true);
//		}
//		else
//		{
//			m_pModel->SetPosition(tempPos);
//		}
//
//	}
//
//	m_nCount++;
//
//
//
//	//if (INPUT->KeyDown('L'))
//	//{
//	//	m_bIsRespawn = true;
//	//	m_eState = MS_DIE;
//	//	ChangeAni();
//	//	SetCurrentHP(1000);
//	//}
//}


void Assis::DropItemSetup()
{
	//����
	m_vItemID.push_back(ITEM_NULL);
	m_vItemID.push_back(NORMAL_HEALTHPOTION);
	m_vItemID.push_back(RARE_TWOHANDEDSWORD);
	m_vItemID.push_back(SWORDMAN_NORMAL_HELMET);
	m_vItemID.push_back(SWORDMAN_NORMAL_BOOTS);
	m_vItemID.push_back(KNIGHT_NORMAL_ARMOR);
	m_vItemID.push_back(KNIGHT_NORMAL_GAUNTLET);
}
