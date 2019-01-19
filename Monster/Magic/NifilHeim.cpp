#include "../MonsterUseHeader.h"
#include "NifilHeim.h"


NifilHeim::NifilHeim()
{
}


NifilHeim::~NifilHeim()
{

}

void NifilHeim::Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon )
{
	//�𵨺��� �����
	//MODELMANAGER->AddModel("��������", "Model/Enemy/NifilHeim/", "NifilHeim.x", MODELTYPE_X);
	m_pModel = MODELMANAGER->GetModel("��������", MODELTYPE_X);

	m_fUIMoveX = 50.0f;
	m_fUIMoveY = 6.0f;

	//m_bIsTargeting = true;

	//�θ��� �¾��� ȣ���ض�
	MagicMonster::Setup(map, spawnPos, isSummon);

	//���� �ڽ� 
	ST_SIZEBOX box;
	box.highX = 50.0f;
	box.highY = 240.0f;
	box.highZ = 50.0f;
	box.lowX = -50.0f;
	box.lowY = 20.0f;
	box.lowZ = -50.0f;


	m_pModel->SetScale(D3DXVECTOR3(0.02f, 0.02f, 0.02f));

	m_pModel->CreateBound(box);
	m_pModel->SetBoundSphere(m_pModel->GetOrigBoundSphere().center, 90.0f);

	//��ų
	m_pSkill = SKILL->GetSkill("NifilHeim Skill");
}

void NifilHeim::SetupStat()
{
	m_nMinMoveCount = NRand(400,600);
	m_nMaxMoveCount = NRand(100,300);

	ZeroMemory(m_pMonsterStat, sizeof(STATUS));

	CURRENTHP(m_pMonsterStat) = MAXHP(m_pMonsterStat) = 150;
	ATK(m_pMonsterStat) = 38;
	PHYRATE(m_pMonsterStat) = 0.8f;
	MAGICRATE(m_pMonsterStat) = 1.3f;
	CHERATE(m_pMonsterStat) = 0.8f;
	ATKSPEED(m_pMonsterStat) =0.7f;

	DEF(m_pMonsterStat) = 5;
	AGI(m_pMonsterStat) = 5.0f;
	HIT(m_pMonsterStat) = 5.0f;
	SPEED(m_pMonsterStat) = 0.03f;
	RANGE(m_pMonsterStat) = 10.0f;
}

void NifilHeim::SetupSkill()
{
	ZeroMemory(&m_stSkill, sizeof(ST_SKILL));//
	m_stSkill.fDamage = 100;				 //
	m_stSkill.fDamageDelay = 1.85f;			 //
	m_stSkill.fDamageInterval = 0.1;		 //
	m_stSkill.fMaxLength = 100;				 //
	m_stSkill.fAngle = 360;					 //
	m_stSkill.nMaxTarget = 5;				 //
	m_stSkill.nDamageCount = 100;			 //
	m_stSkill.isAutoRot = true;				 //
	m_stSkill.fYOffset = 13.5f;					 //
	m_stSkill.fBuffTime = -1;				 //
	m_stSkill.fParticleTime = 10;			 //
	m_stSkill.fParticleSpeed = 0.05;		 //
	m_stSkill.fEffectTime = 1.85f;				 //

	m_fSkillCoolTimeCount = 0;
	m_nSkillCooltime = 50;
}


//���� ���� �����Լ�
//void NifilHeim::Attack()
//{
//
//}

//���� ���� ��ų�Լ�
void NifilHeim::SkillUse()
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
//void NifilHeim::Move()
//{
//
//}


void NifilHeim::DropItemSetup()
{
	m_vItemID.push_back(ITEM_NULL);
	m_vItemID.push_back(NORMAL_RECOVERYBOOK);
	m_vItemID.push_back(NORMAL_STAFF);
	m_vItemID.push_back(SWORDMAN_NORMAL_ARMOR);
	m_vItemID.push_back(SWORDMAN_NORMAL_GAUNTLET);
	m_vItemID.push_back(BABARIAN_RARE_ARMOR);
	m_vItemID.push_back(KNIGHT_NORMAL_HELMET);
	m_vItemID.push_back(VALKYRIE_RARE_HELMET);
}
