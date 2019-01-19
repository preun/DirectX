#include "../MonsterUseHeader.h"
#include "../MonsterManager.h"
#include "BossParent.h"
#include "../Magic/MagicCircle.h"


BossParent::BossParent()
{
}


BossParent::~BossParent()
{
	SAFE_DELETE(m_pMagicCircle);
	SAFE_DELETE(m_pSkill2);
	m_pMM = NULL;

}



void BossParent::SetupBoss(Map* map, D3DXVECTOR3 pos)
{
	MonsterParent::SetupBoss(map, pos);

	m_pMagicCircle = new MagicCircle;
}

void BossParent::Update()
{
	if (INPUT->KeyDown('4'))
		SetCurrentHP(10000,NULL);
	
	

	m_fAlphaCount += TIME->GetElapsedTime();

	if (m_fAlphaCount >= 1) m_pModel->SetShaderAlpha(1.0f);

	m_fSkillCoolTimeCount += TIME->GetElapsedTime();
	m_fSkillCoolTimeCount2 += TIME->GetElapsedTime();

	if (m_pModel && (m_eState != MS_NONE && m_eBossState != BS_NONE))
	{
		m_pModel->World();
		m_pModel->Update();
	}

	//if (INPUT->KeyDown('L'))
	//{
	//	//vector<MonsterParent*> tt;
	//	//m_pSkill->Prepare(PCHARACTER,
	//	//	this,
	//	//	tt,
	//	//	m_stSkill,
	//	//	SKILLO_MONSTER);
	//
	//	m_eBossState = BS_SKILL2;
	//	ChangeAni();
	//}

	if (m_pHPBar)
	{
		float tempF = (float)CURRENTHP(m_pMonsterStat) / MAXHP(m_pMonsterStat);


		m_pHPBar->SetScale(D3DXVECTOR3(tempF, 1, 1));

		m_pHPBar->Update();
	}
	if (m_pSkill) m_pSkill->Update();

	Pattern();

}
		//1536
void BossParent::Render()
{
	m_pDamageUI->Render(*m_pModel->GetPosition());

	if (m_pModel && (m_eState != MS_NONE && m_eBossState != BS_NONE) )
	{
		m_pModel->Render();
		
		if (m_bIsTargeting) m_pHPBar->Render();
	}

	if (m_pHPBar) m_pHPBar->Render();

	if (m_pSkill) m_pSkill->Render();

}

void BossParent::SetCurrentHP(int hp, int* deathCount)
{
	if (m_eBossState == BS_CASTING)
	{
		CastingCancel();
	}

	if (m_eBossState == BS_STUN)
	{
		hp *= 2;
	}

	CURRENTHP(m_pMonsterStat) -= hp;

	m_pModel->SetShaderAlpha(0.5f);
	m_fAlphaCount = 0;
	m_pDamageUI->AddDamage(hp);

	if (CURRENTHP(m_pMonsterStat) <= 0)
	{
		if(deathCount)
		(*deathCount)++;
		m_pModel->SetShaderAlpha(1.0f);
		if (m_pMM)
		m_pMM->DeleteSummonMonster();
		CURRENTHP(m_pMonsterStat) = 0;
		m_bIsDead = true;
		m_bIsRespawn = true;
		m_eState = MS_DIE;
		m_eBossState = BS_DIE;
		ChangeAni();
		ItemDrop();
		//this->Update();
	}
}


void BossParent::SetupStat()
{

}
void BossParent::SetupSkill()
{

}

void BossParent::SetupSkill2()
{
}

void BossParent::Attack()
{

}

void BossParent::SkillUse()
{

}

void BossParent::Skill2()
{
}

void BossParent::Passive()
{
	
	//1회 행동 했을때 idle상태로 바꿔서 다시 행동 판단
	if (m_pModel->IsAnimationEnd())
	{
		m_eBossState = BS_IDLE;
		ChangeAni();
	}
}

void BossParent::Casting()
{
	
	
	if (m_pModel->IsAnimationEnd())
		//if (m_pModel->IsAnimationPercent(0.5f))
	{
		m_eBossState = BS_SKILL1;
		ChangeAni();
		m_pModel->SetAnimationSpeed(1.0f);
		//	m_pModel->SetAnimationPosition(0.5f);
	}
}

void BossParent::Move()
{

}


void BossParent::DropItemSetup()
{

}

void BossParent::ChangeAni()
{
	switch (m_eBossState)
	{
	case BS_ENTER:
		m_pModel->SetAnimation("ENTER");
		//m_pModel->SetBlendAnimation("ENTER");
		//m_pModel->SetBlendTime(0.1f);
		break;
	case BS_IDLE:
		m_pModel->SetAnimation("IDLE");
		break;
	case BS_RUN:
		m_pModel->SetAnimation("RUN");
		break;
	case BS_PASSIVE:
		m_pModel->SetAnimation("PASSIVE");
		break;
	case BS_ATTACK:
		m_pModel->SetAnimation("ATTACK");
		break;
	case BS_SKILL1:
		m_pModel->SetAnimation("SKILL2");
		break;
	case BS_SKILL2:
		m_pModel->SetAnimation("SKILL1");
		break;
	case BS_CASTING:
		m_pModel->SetAnimation("SKILL2_CASTING");
		break;
	case BS_STUN:
		m_pModel->SetAnimation("STUN");
		m_pModel->SetAnimation(0.4f);
		break;
	case BS_DIE:
		m_pModel->SetAnimation("DIE");
		break;
	case BS_NONE:
		break;
		//이하 레알보스를 위한 체인지 애니메이션
	case BS_ENTER1:
		m_pModel->SetAnimation("ENTER1");
		m_pModel->SetAnimationPosition(0.05f);
		//m_pModel->SetBlendAnimation("ENTER1");
		//m_pModel->SetBlendTime(0.1f);
		break;
	case BS_ENTER2:
		m_pModel->SetAnimation("ENTER2");
		m_pModel->SetAnimationPosition(0.3f);
		//m_pModel->SetBlendAnimation("ENTER2");
		//m_pModel->SetBlendTime(0.1f);
		break;
	case BS_ENTER3:
		m_pModel->SetAnimation("ENTER3");
		m_pModel->SetAnimationPosition(0.17f);
		//m_pModel->SetBlendAnimation("ENTER3");
		//m_pModel->SetBlendTime(0.1f);
		break;
	default:
		break;
	}

	if (m_eBossState == BS_CASTING)
	{
		m_pModel->SetAnimationSpeed(0.5f);
	}
	else m_pModel->SetAnimationSpeed(1.0f);
}
void BossParent::Pattern()
{
	
}

bool BossParent::AbleSkill()
{
	float leng = GetDistance(*m_pModel->GetPosition(), *CHARACTER->GetPosition());

	//쿨타임이 돌았고 스킬이 사용가능할때
	//그리고 내가 타겟팅이 되었을때 스킬 사용
	if (m_fSkillCoolTimeCount >= m_nSkillCooltime
		&& leng <= m_stSkill.fMaxLength
		&& m_bIsTargeting
		&& !m_bIsRespawn
		&& !m_bUsingSkill
		&& m_pModel->IsAnimationEnd()
		&& !m_bUsingSkill)
	{

		//m_stSkill.fEffectTime = leng / tempEffect.s

		ChangeRot();
		m_bUsingSkill = true;
		SkillPrepare();
		return true;
	}

	return false;

	return false;
}

bool BossParent::AbleSkill2()
{
	float leng = GetDistance(*m_pModel->GetPosition(), *CHARACTER->GetPosition());

	//쿨타임이 돌았고 스킬이 사용가능할때
	//그리고 내가 타겟팅이 되었을때 스킬 사용
	if (m_fSkillCoolTimeCount2 >= m_nSkillCooltime2
		&& leng <= m_stSkill2.fMaxLength
		&& m_bIsTargeting
		&& !m_bIsRespawn
		&& !m_bUsingSkill
		&& !m_bUsingSkill2
		&& m_pModel->IsAnimationEnd())
	{
		ChangeRot();
		m_bUsingSkill2 = true;
		SkillPrepare2();
		return true;
	}

	return false;
}

void BossParent::SkillPrepare2(){
	m_pSkill->Prepare(PCHARACTER,
		this,
		NULL,
		m_stSkill2,
		SKILLO_MONSTER);
}

void BossParent::CastingCancel()
{
}
