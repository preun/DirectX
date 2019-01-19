#include "../MonsterUseHeader.h"
#include "Nike.h"


Nike::Nike()
{
}


Nike::~Nike()
{

}

void Nike::Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon)
{
	//�𵨺��� �����
	MODELMANAGER->AddModel("����", "Model/Enemy/Nike/", "Nike.x", MODELTYPE_X);
	m_pModel = MODELMANAGER->GetModel("����", MODELTYPE_X);

	m_fUIMoveX = 50.0f;
	m_fUIMoveY = 4.0f;

	//m_bIsTargeting = true;

	//�θ��� �¾��� ȣ���ض�
	DistanceMonster::Setup(map, spawnPos, isSummon);

	//���� �ڽ� 
	ST_SIZEBOX box;
	box.highX = 50.0f;
	box.highY = 130.0f;
	box.highZ = 50.0f;
	box.lowX = -50.0f;
	box.lowY = 10.0f;
	box.lowZ = -50.0f;


	m_pModel->SetScale(D3DXVECTOR3(0.02f, 0.02f, 0.02f));

	m_pModel->CreateBound(box);
	m_pModel->SetBoundSphere(m_pModel->GetOrigBoundSphere().center,80.0f);

	//��ų
	m_pSkill = SKILL->GetSkill("Nike Skill");
}

void Nike::SetupSkill()
{
	ZeroMemory(&m_stSkill, sizeof(m_stSkill));

	m_stSkill.nMaxTarget = 1;
	m_stSkill.fMinLength = 0;
	m_stSkill.fMaxLength = 10;
	m_stSkill.fAngle = 360;

	m_stSkill.fDamage = 23; //v
	m_stSkill.nDamageCount = 1;
	m_stSkill.fDamageInterval = 0;
	m_stSkill.fDamageDelay = 0;

	m_stSkill.fBuffTime = -1;//<0;



	//m_stSkill.fYOffset ;
	//m_stSkill.isAutoRot;
	//m_stSkill.fParticleTime;
	//m_stSkill.fParticleSpeed;
	m_stSkill.fEffectTime = 0;

	m_fSkillCoolTimeCount = 0;
	m_nSkillCooltime = 50;

	//	m_stSkill.buffStatus.chr.nCurrentHP = 100; //���� �� ���ȷ� �Ǖ��� ���θ޸�;
}

void Nike::SetupStat()
{
	m_nMinMoveCount = NRand(333,555);
	m_nMaxMoveCount = NRand(100,150);

	ZeroMemory(m_pMonsterStat, sizeof(STATUS));

	CURRENTHP(m_pMonsterStat) = MAXHP(m_pMonsterStat) = 90;
	ATK(m_pMonsterStat) = 20;
	PHYRATE(m_pMonsterStat) = 0.9f;
	MAGICRATE(m_pMonsterStat) = 0.9f;
	CHERATE(m_pMonsterStat) = 1.3f;
	ATKSPEED(m_pMonsterStat) = 0.8f;

	DEF(m_pMonsterStat) = 4;
	AGI(m_pMonsterStat) = 20.0f;
	HIT(m_pMonsterStat) = 20.0f;
	SPEED(m_pMonsterStat) = 0.09f;
	RANGE(m_pMonsterStat) = 7.0f;


}


//���� ���� �����Լ�
//void Nike::Attack()
//{
//
//}

//���� ���� ��ų�Լ�
void Nike::SkillUse()
{
	vector<MonsterParent*> tt;


	if (m_pModel->IsAnimationPercent(0.2))
	{
		//SkillPrepare();

		m_pSkill->Trigger();
	}

	if (m_pModel->IsAnimationPercent(0.4))
	{
		SkillPrepare();

		m_pSkill->Trigger();
	}

	if (m_pModel->IsAnimationPercent(0.8))
	{
		SkillPrepare();

		m_pSkill->Trigger();
	}

	if (m_pModel->IsAnimationEnd())
	{
		m_bUsingSkill = false;
		m_fSkillCoolTimeCount = 0;
		m_eState = MS_ATTACK;
		ChangeAni();
	}
}

//���� ���� �⺻ �̵��Լ�
//void Nike::Move()
//{
//
//}


void Nike::DropItemSetup()
{
	m_vItemID.push_back(ITEM_NULL);
	m_vItemID.push_back(NORMAL_HEALTHPOTION);
	m_vItemID.push_back(NORMAL_ARROW);
	m_vItemID.push_back(ARCHER_NORMAL_HELMET);
	m_vItemID.push_back(ARCHER_NORMAL_ARMOR);
	m_vItemID.push_back(BABARIAN_RARE_GAUNTLET);
	m_vItemID.push_back(BABARIAN_RARE_BOOTS);
}
