#include "../MonsterUseHeader.h"
#include "../Magic/MagicCircle.h"
#include "RealFinalboss.h"

RealFinalboss::RealFinalboss()
{
}

RealFinalboss::~RealFinalboss()
{
	for (int i = 0; i < STONENUM; ++i)
	{
		SAFE_DELETE(m_vMagicCircle[i]);
		SAFE_DELETE(m_vEffectObject[i]);
	}

	SAFE_DELETE(m_pSkill);
	SAFE_DELETE(m_pSkill2);
	SAFE_DELETE(m_pPassive);
	
	for (int i = 0; i < HT_END; ++i)
	{
		SAFE_DELETE(m_pBuffPrticle[i]);
	}
}

void RealFinalboss::SetupBoss(Map* map, D3DXVECTOR3 pos)
{
	//모델 바꿔야함
	m_pModel = MODELMANAGER->GetModel("최종보스", MODELTYPE_X);

	BossParent::SetupBoss(map, pos);

	m_eBossState = BS_ENTER;

	//m_eBossState = BS_PASSIVE;

	ChangeAni();
	//판정 박스 
	ST_SIZEBOX box;
	box.highX = 200.0f;
	box.highY = 1000.0f;
	box.highZ = 200.0f;
	box.lowX = -200.0f;
	box.lowY = 0.0f;
	box.lowZ = -200.0f;


	m_pModel->SetScale(D3DXVECTOR3(0.025f, 0.025f, 0.025f));

	m_pModel->CreateBound(box);
	m_pModel->SetBoundSphere(m_pModel->GetOrigBoundSphere().center - D3DXVECTOR3(0,330,0), 300.0f);

	//피흡 바꿔야함
	m_pPassive = SKILL->GetSkill("RealBossPassive");
	
	m_pSkill = SKILL->GetSkill("RealBossSkill");

	//2스킬 셋업
	SetupSkill();

	SetupPassive();

	m_bUsingSkill = m_bUsingSkill = m_bUsingPassive = m_bDropStone = false;
	m_fSkillCoolTimeCount = m_fSkillCoolTimeCount2 = m_fPassiveCooltimeCount = 0;
	m_bIsTargeting = true;

	m_vMagicCircle.resize(STONENUM);
	m_vEffectObject.resize(STONENUM);
	for (int i = 0; i < STONENUM; ++i)
	{
		m_vMagicCircle[i] = new MagicCircle;
		m_vMagicCircle[i]->Setup();

		m_vEffectObject[i] = new EffectObject;
	}

	m_pModel->SetPosition(D3DXVECTOR3(pos.x, pos.y, pos.z));


	ZeroMemory(&m_stEffect, sizeof(ST_EFFECT));
	m_stEffect.dir = D3DXVECTOR3(0,-1,0);
	m_stEffect.SetAlpha(255, 255, 255);
	m_stEffect.isSphere = true;
	m_stEffect.SetSpeed(0.9f, 0, 1.5f);
	m_stEffect.SetScale(3, 3, 3);
	m_stEffect.height = 3;
	m_stEffect.time = 3.5;
	m_stEffect.pModel = MODELMANAGER->GetModel("보스돌", MODELTYPE_OBJ);
	//effect.SetMotorSpeed(1, 1, 1);
	//effect.mot = D3DXVECTOR3(0, 0, 1);
	//m_stEffect.tex = TEXTUREMANAGER->GetTexture("돌");
	
	BoolInit();

	CAMERA->SetCamOffset(D3DXVECTOR3(0, 15, 40));
	CAMERA->SetTarget(m_pModel->GetPosition(), m_pModel->GetRotation());
	CAMERA->Cinematic(D3DXVECTOR2(85.0f, 60.0f), D3DXVECTOR2(-10.0f, 10.0f), 40, 1,10);

	for (int i = 0; i < HT_END; ++i)
	{
		m_pBuffPrticle[i] = NULL;
	}
}

void RealFinalboss::Update()
{
	if (m_pBuffPrticle[0])
	{
		/*m_pHand*/
		/*
		HT_LD,
		HT_LT,
		HT_RD,
		HT_RT,
		HT_END		
		*/
		for (int i = 0; i < HT_END; ++i)
		{
			D3DXVECTOR3 pos(0, 0, 0);
			D3DXVec3TransformCoord(&pos, &pos, m_pHand[i]);
			m_pBuffPrticle[i]->SetPosition(pos);
			//m_pBuffPrticle[i]->ApplyWorld();
			m_pBuffPrticle[i]->World();
			m_pBuffPrticle[i]->Update();
		}

		
	}

	if (INPUT->KeyDown('L'))
	{
		m_eBossState = BS_CASTING;
		//ChangeAni();
		SkillPrepare();
	}

	m_fPassiveCooltimeCount += TIME->GetElapsedTime();

	if (m_fPassiveCooltimeCount >= m_stPassive.fBuffTime && m_bUsingPassive)
	{
		m_bUsingPassive = false;

		for (int i = 0; i < HT_END; i++)
		{
			SAFE_DELETE(m_pBuffPrticle[i]);
		}
	}
	if (CAMERA->IsActionEnd())
		CAMERA->SetTarget(CHARACTER->GetPosition(), CHARACTER->GetRotation());

	if (m_bUsingSkill2)
	{
		for (int i = 0; i < STONENUM; ++i)
		{
			m_vMagicCircle[i]->Update();
			if (m_vEffectObject[i])
			m_vEffectObject[i]->Update();
		}
	}

	if (m_eBossState == BS_ATTACK && m_bDropStone)
	{
		m_vMagicCircle[0]->Update();
		if (m_vEffectObject[0])
			m_vEffectObject[0]->Update();
	}
	

	if (m_pPassive) m_pPassive->Update();
		

	BossParent::Update();

}

void RealFinalboss::Render()
{
	//Debug();

	if (m_bUsingSkill2)
	{
		
		for (int i = 0; i < STONENUM; ++i)
		{
			m_vMagicCircle[i]->Render();
			if(m_vEffectObject[i])
			m_vEffectObject[i]->Render();
		}
	}

	if (m_pModel && (m_eState != MS_NONE && m_eBossState != BS_NONE))
	{
		if (IsEnter() || m_eBossState == BS_SKILL1 || m_eBossState == BS_DIE)
			m_pModel->Render();
		else
			m_pModel->DummyRender();

		if (m_bIsTargeting) m_pHPBar->Render();

		if (m_eBossState == BS_ATTACK && m_bDropStone)
		{
			m_vMagicCircle[0]->Render();
			if (m_vEffectObject[0])
				m_vEffectObject[0]->Render();
		}
	}

	if (m_pHPBar) m_pHPBar->Render();

	if (m_pPassive) m_pPassive->Render();

	if (m_pBuffPrticle[0])
	{
		for (int i = 0; i < HT_END; ++i)
		{
			m_pBuffPrticle[i]->Render();
		}
	}
}

void RealFinalboss::SetupStat()
{
	m_nMinMoveCount = 0;
	m_nMaxMoveCount = 0;

	ZeroMemory(m_pMonsterStat, sizeof(STATUS));

	CURRENTHP(m_pMonsterStat) = 2000;
	MAXHP(m_pMonsterStat) = 2000;
	ATK(m_pMonsterStat) = 20;
	PHYRATE(m_pMonsterStat) = 1.3f;
	MAGICRATE(m_pMonsterStat) = 0.8f;
	CHERATE(m_pMonsterStat) = 1.1f;
	ATKSPEED(m_pMonsterStat) = 0.75f;

	DEF(m_pMonsterStat) = 5;
	AGI(m_pMonsterStat) = 10.0f;
	HIT(m_pMonsterStat) = 10.0f;
	RANGE(m_pMonsterStat) = 35.0f;
}

void RealFinalboss::SetupSkill()
{
	//스킬2
	ZeroMemory(&m_stSkill, sizeof(ST_SKILL));
	m_stSkill.fDamage = 100;
	m_stSkill.fDamageDelay =1.0f;
	m_stSkill.fDamageInterval = 0.3f;
	m_stSkill.fMaxLength = 15;
	//m_stSkill.fMaxLength = 1500;
	m_stSkill.fAngle = 360;
	m_stSkill.nMaxTarget = 5;
	m_stSkill.nDamageCount = 3;
	m_stSkill.isAutoRot = true;
	m_stSkill.fYOffset = 1;
	m_stSkill.fBuffTime = 0;
	m_stSkill.fParticleTime = 0;
	m_stSkill.fParticleSpeed = 0;
	m_stSkill.fEffectTime = 0;

	m_nSkillCooltime = 80;
}

//패시브 설정
void RealFinalboss::SetupSkill2()
{
	
}

void RealFinalboss::SetupPassive()
{
	ZeroMemory(&m_stPassive, sizeof(ST_SKILL));
	m_stPassive.fDamage = 0;
	m_stPassive.fDamageDelay = 0;
	m_stPassive.fDamageInterval = 0;
	m_stPassive.fMaxLength = 0;
	m_stPassive.fAngle = 0;
	m_stPassive.nMaxTarget = 0;
	m_stPassive.nDamageCount = 0;
	m_stPassive.isAutoRot = true;
	m_stPassive.fYOffset = 1;
	m_stPassive.fBuffTime = 60;
	m_stPassive.fParticleTime = m_pModel->GetAnimationPeriod("PASSIVE");
	m_stPassive.fParticleSpeed = 0;
	m_stPassive.fEffectTime = 3;


	m_nPassiveCooltime = 120;
}
//void RealFinalboss::ChangeAni()
//{
//	switch (m_eBossState)
//	{
//	case BS_ENTER:
//		m_pModel->SetAnimation("ENTER");
//		break;
//	case BS_IDLE:
//		m_pModel->SetAnimation("IDLE");
//		break;
//	case BS_RUN:
//		m_pModel->SetAnimation("RUN");
//		break;
//	case BS_PASSIVE:
//		m_pModel->SetAnimation("PASSIVE");
//		break;
//	case BS_ATTACK:
//		m_pModel->SetAnimation("ATTACK");
//		break;
//	case BS_SKILL1:
//		m_pModel->SetAnimation("SKILL2");
//		break;
//	case BS_SKILL2:
//		m_pModel->SetAnimation("SKILL1");
//		break;
//	case BS_CASTING:
//		m_pModel->SetAnimation("SKILL2_CASTING");
//		break;
//	case BS_DIE:
//		m_pModel->SetAnimation("DIE");
//		break;
//	case BS_NONE:
//		break;
//	default:
//		break;
//	}
//
//	if (m_eBossState == BS_CASTING)
//	{
//		m_pModel->SetAnimationSpeed(0.5f);
//	}
//	else m_pModel->SetAnimationSpeed(1.0f);
//}

void RealFinalboss::Pattern()
{
	HandMatInit();

	//패시브가 발동 가능한 상태면
	//bool plesawf = ;
	if (AblePassive())
	{
		m_fPassiveCooltimeCount = 0;
		BoolInit();
		BuffDecide();
		m_bUsingPassive = true;
		PassivePrepare();
		m_eBossState = BS_PASSIVE;
		ChangeAni();
	}

	else if (AbleSkill())
	{
		m_bUsingSkill = true;
		SkillPrepare();
		m_eBossState = BS_CASTING;
		m_pModel->SetAnimation("BS_ENTER4");
		m_pModel->SetAnimationSpeed(0.5f);
		//ChangeAni();
	}
	//char temp[222];
	//sprintf_s(temp, sizeof(temp), "%f",
	//	GetDistance(*m_pModel->GetPosition(), *CHARACTER->GetPosition()));
	//TEXT->Add(temp, 100, 100, 20);

	//if (AbleSkill() && !m_bUsingSkill)
	//{
	//	m_eBossState = BS_CASTING;
	//	ChangeAni();
	//}
	//
	//else if (AbleSkill2())
	//{
	//	m_eBossState = BS_SKILL2;
	//	ChangeAni();
	//}

	switch (m_eBossState)
	{
	case BS_ENTER: case BS_ENTER1: case BS_ENTER2: case BS_ENTER3:
	{
		EnterAni();
	}
	break;

	case BS_PASSIVE:
		Passive();
		break;

	case BS_ATTACK:
		Attack();
		break;

	case BS_SKILL1:
		SkillUse();
		break;

	case BS_SKILL2:
		Skill2();
		break;

	case BS_CASTING:
		Casting();
		break;

	case BS_STUN:
		if (m_pModel->IsAnimationEnd())
		{
			m_eBossState = BS_ATTACK;
			ChangeAni();
		}
		break;

	case BS_DIE:
	{
		if (m_pModel->IsAnimationEnd())
		{
			m_eBossState = BS_NONE;
			//	m_eState = MS_NONE;
		}
	}
	default:
		break;
	}
}

void RealFinalboss::Attack()
{
	if (PCHARACTER->GetIsDead())
	{
		m_eState = MS_IDLE;
		ChangeAni();
		return;
	}

	MakeSphere();


	//if (AbleSummon())
	//{
	//	m_eBossState = BS_PASSIVE;
	//	ChangeAni();
	//	Passive();
	//
	//	//공격 스킵하고 패시브 발동
	//	return;
	//}

	float length = GetDistance(*m_pModel->GetPosition(), *CHARACTER->GetPosition());

	//공격 모션중에 플레이어가 벗어나도 공격모션 및 판정진행해라
	if (length > RANGE(m_pMonsterStat))
	{
		if (!m_bIsAttack)
		{
			m_eBossState = BS_SKILL2;
			ChangeAni();
			
			return;
		}
	
	}

	//m_bUsingSkill2 = false;

	//사거리 안에 들어왔는데 이제 공격을 시작할려고 한다면
	if (!m_bIsAttack)
	{
		//방향 및 각도를 구하고		
		//ChangeRot();

		D3DXVECTOR3 dir =
			*CHARACTER->GetPosition() - *m_pModel->GetPosition();

		//각도를 구하고
		float angle = GetAngle(0, 0, dir.x, dir.z);


		//angle -= D3DX_PI / 2;

		//각도만큼 로테이션
		m_pModel->SetRotation(D3DXVECTOR3(0, angle, 0));

		//첫 판정후 이제 공격중으로 바꿔줌으로서 보스의 회전을 막는다.
		m_bIsAttack = true;

	}
	
	if (HandCollision())
	{
		if (m_pModel->IsAnimationPercent(0.35)
			|| m_pModel->IsAnimationPercent(0.7))
			BoolInit();
		CAMERA->Shake(0.1f, 0.3f);
		float tatalRate = PHYRATE(m_pMonsterStat) + MAGICRATE(m_pMonsterStat) + CHERATE(m_pMonsterStat);
		float tatalDamage = tatalRate * ATK(m_pMonsterStat);

		PCHARACTER->CalculDamage(tatalDamage);
	}
	ST_SPHERE target = CHARACTER->GetBoundSphere();

	
	ST_SPHERE stone;
	if (m_vEffectObject[0])
	{
		if (m_vEffectObject[0]->GetInfo().pModel)
		{
			stone = m_vEffectObject[0]->GetInfo().pModel->GetBoundSphere();

			if (IntersectSphere(target, stone))
			{
				D3DXVECTOR3 temp = *CHARACTER->GetPosition();
				temp.y += 1.0f;
				//m_vMagicCircle[i]->SetParticlePos(temp);
				m_vMagicCircle[0]->ChangeParticle("흙먼지", temp);

				//m_vMagicCircle[0]->ChangeParticle("흙먼지");
				//m_vMagicCircle[0]->SetParticlePos(*m_pModel->GetPosition());
				CAMERA->Shake(0.5f, 0.5f);
				PCHARACTER->CalculDamage(100);
				SAFE_DELETE(m_vEffectObject[0]);
			}
		}
	}
	

	if (m_pModel->IsAnimationPercent(0.8))
	{
		DropTheStone();
	}
	
	//플레이어 공격기능 설정
	//if (m_pModel->IsAnimationPercent(ATKSPEED(m_pMonsterStat)))
	//{
	//	if (m_pMagicCircle->PlayerCollision(
	//		*CHARACTER->GetPosition(),
	//		CHARACTER->GetBoundSphere().radius))
	//	{
	//
	//		
	//		PCHARACTER->CalculDamage(tatalDamage);
	//	}
	//
	//
	//	//float tatalRate = PHYRATE(m_pMonsterStat) + MAGICRATE(m_pMonsterStat) + CHERATE(m_pMonsterStat);
	//	//float tatalDamage = tatalRate * ATK(m_pMonsterStat);
	//	//PCHARACTER->CalculDamage(tatalDamage);
	
	if (m_pModel->IsAnimationEnd())
	{
		BoolInit();
		m_bIsAttack = false;
	}
}

void RealFinalboss::Move()
{
	
}

void RealFinalboss::Passive()
{
	//버프스킬
	m_pPassive->Trigger();

	if (m_pModel->IsAnimationEnd())
	{
		//m_fPassiveCooltimeCount = 0;
		m_eBossState = BS_ATTACK;
		ChangeAni();
	}
	
}

void RealFinalboss::SkillUse()
{
	
	m_pSkill->Trigger();
	
	if (m_pModel->IsAnimationEnd())
	{

		m_bUsingSkill = false;
		m_fSkillCoolTimeCount = 0;
		m_eBossState = BS_ATTACK;
		ChangeAni();
	}
}

void RealFinalboss::Skill2()
{
	//m_bDropStone = false;
	ST_SPHERE target = CHARACTER->GetBoundSphere();

	for (int i = 0; i < m_vEffectObject.size(); ++i)
	{
		ST_SPHERE stone;
			if (m_vEffectObject[i])
			{
				if (m_vEffectObject[i]->GetInfo().pModel)
				{
					stone = m_vEffectObject[i]->GetInfo().pModel->GetBoundSphere();

					if (IntersectSphere(target, stone))
					{

						D3DXVECTOR3 temp = *CHARACTER->GetPosition();
						temp.y += 1.0f;
						//m_vMagicCircle[i]->SetParticlePos(temp);
						m_vMagicCircle[i]->ChangeParticle("흙먼지", temp);
						CAMERA->Shake(0.5f, 0.5f);
						PCHARACTER->CalculDamage(100);
						SAFE_DELETE(m_vEffectObject[i]);
					}
				}
			}
	}
	if (m_pModel->IsAnimationPercent(0.65f))
	{
		DropTheStones();
		//돌을 떨구자
	}

	if (m_pModel->IsAnimationEnd())
	{
		m_eBossState = BS_ATTACK;
		ChangeAni();
	}
	

}

void RealFinalboss::CastingCancel()
{
	m_bUsingSkill = false;
	m_fSkillCoolTimeCount = 0;

	m_eBossState = BS_STUN;
	ChangeAni();
}

//void RealFinalboss::Casting()
//{
//	if (m_pModel->IsAnimationEnd())
//		//if (m_pModel->IsAnimationPercent(0.5f))
//	{
//		m_eBossState = BS_SKILL1;
//		ChangeAni();
//		//	m_pModel->SetAnimationPosition(0.5f);
//	}
//}

void RealFinalboss::EnterAni()
{
	switch (m_eBossState)
	{
	case BS_ENTER:
		if (m_pModel->IsAnimationEnd())
		//if (m_pModel->IsAnimationPercent(98))
		{
			m_eBossState = BS_ENTER1;
			ChangeAni();
		}
		break;
	case BS_ENTER1:	//case BS_ENTER2:	
		//if (m_pModel->IsAnimationEnd())
		if (m_pModel->IsAnimationPercent(0.45f)
			|| m_pModel->IsAnimationPercent(0.47f))
		{
			m_eBossState = BOSS_STATE(m_eBossState+1);
			ChangeAni();
			
		}
		break;
	case BS_ENTER2:	//case BS_ENTER2:	
					//if (m_pModel->IsAnimationEnd())
		if (m_pModel->IsAnimationPercent(0.8f))
		{
			m_eBossState = BOSS_STATE(m_eBossState + 1);
			CAMERA->Shake(0.5f, 3.5f);
			ChangeAni();
		}
		break;
	case BS_ENTER3:
		if (m_pModel->IsAnimationEnd())
		//if (m_pModel->IsAnimationPercent(98))
		{
			m_eBossState = BS_ATTACK;
			ChangeAni();
		}
		break;
	default:
		break;
	}
}

void RealFinalboss::HandMatInit()
{
	m_pHand[HT_LD] = m_pModel->GetBoneMatrix("Bip001-L-Hand");
	m_pHand[HT_LT]= m_pModel->GetBoneMatrix("Bip002-L-Hand");
	m_pHand[HT_RD] = m_pModel->GetBoneMatrix("Bip001-R-Hand");
	m_pHand[HT_RT] = m_pModel->GetBoneMatrix("Bip002-R-Hand");
}

void RealFinalboss::DropTheStone()
{
	m_bDropStone = true;
	m_bUsingSkill2 = false;
	D3DXVECTOR3 rndPos = *CHARACTER->GetPosition();
			
	m_vMagicCircle[0]->ChangeParticle("마법기본공격", rndPos);
	//m_vMagicCircle[0]->SetParticlePos(rndPos);
	//m_vMagicCircle[0]->GetParticle()->TimeReset();

	SAFE_DELETE(m_vEffectObject[0]);
	m_vEffectObject[0] = new EffectObject;
	m_vEffectObject[0]->Init(m_stEffect, rndPos + D3DXVECTOR3(0, FRand(7.0f, 27.0f), 0));
	m_stEffect.pModel = MODELMANAGER->GetModel("보스돌", MODELTYPE_OBJ);
}

void RealFinalboss::DropTheStones()
{
	m_bUsingSkill2 = true;
	m_bDropStone = false;
	//for (int i = 0; i < STONENUM; ++i)
	//{
	//	SAFE_DELETE(m_vMagicCircle[i]);
	//}
	//
	//m_vMagicCircle.clear();
	/*
	
	static float FRand(float low, float high)
	{
		return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
	}
	*/
	for (int i = 0; i < STONENUM; ++i)
	{
		
		D3DXVECTOR3 rndPos = *CHARACTER->GetPosition();
		rndPos.x += FRand(RNDMIN,RNDMAX);
		rndPos.z += FRand(RNDMIN,RNDMAX);

		/* 	void Init(ST_EFFECT info, D3DXVECTOR3 pos);*/
		
		m_vMagicCircle[i]->ChangeParticle("마법기본공격", rndPos);
		//m_vMagicCircle[i]->SetParticlePos(rndPos);
		//m_vMagicCircle[i]->GetParticle()->TimeReset();

		SAFE_DELETE(m_vEffectObject[i]);
		m_vEffectObject[i] = new EffectObject;
		m_vEffectObject[i]->Init(m_stEffect, rndPos + D3DXVECTOR3(0, FRand(7.0f, 27.0f), 0));
		m_stEffect.pModel = MODELMANAGER->GetModel("보스돌", MODELTYPE_OBJ);
	}
}

void RealFinalboss::MakeSphere()
{
	D3DXVECTOR3 temp;
	for (int i = 0; i < HT_END; ++i)
	{
		temp = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&temp, &temp, m_pHand[i]);
		m_stHandSphere[i].Hand.center = temp;
		m_stHandSphere[i].Hand.radius = 8;
	}
	//D3DXVec3TransformCoord(&temp, &temp, m_stHandMat.LeftHand1);
	//m_stHandSphere.LeftHand1.center = temp;
	//
	//temp = D3DXVECTOR3(0, 0, 0);
	//D3DXVec3TransformCoord(&temp, &temp, m_stHandMat.LeftHand2);
	//m_stHandSphere.LeftHand2.center = temp;
	//
	//temp = D3DXVECTOR3(0, 0, 0);
	//D3DXVec3TransformCoord(&temp, &temp, m_stHandMat.RightHand1);
	//m_stHandSphere.RightHand1.center = temp;
	//
	//temp = D3DXVECTOR3(0, 0, 0);
	//D3DXVec3TransformCoord(&temp, &temp, m_stHandMat.RightHand2);
	//m_stHandSphere.RightHand2.center = temp;
	//
	//m_stHandSphere.LeftHand1.radius =
	//	m_stHandSphere.LeftHand2.radius =
	//	m_stHandSphere.RightHand1.radius =
	//	m_stHandSphere.RightHand2.radius = 8;
}

void RealFinalboss::Debug()
{
	//DWORD prevFillMode;
	//DEVICE->GetRenderState(D3DRS_FILLMODE, &prevFillMode);
	//DEVICE->SetTexture(0, NULL);
	//DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	//DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//
	//// BoundSphere
	//LPD3DXMESH mesh;
	//D3DXMATRIX matT;
	//
	//for (int i = 0; i < HT_END; i++)
	//{
	//	float radius = m_stHandSphere[i].Hand.radius;
	//	if (radius < 0) radius = 0;
	//	D3DXCreateSphere(DEVICE, radius, 8, 8, &mesh, NULL);
	//	D3DXVECTOR3 pos = m_stHandSphere[i].Hand.center;
	//	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);
	//	DEVICE->SetTransform(D3DTS_WORLD, &matT);
	//	mesh->DrawSubset(0);
	//	SAFE_RELEASE(mesh);
	//}
	//
	//DEVICE->SetRenderState(D3DRS_FILLMODE, prevFillMode);
}

bool RealFinalboss::HandCollision()
{
	for (int i = 0; i < HT_END; ++i)
	{
		if (IntersectSphere(m_stHandSphere[i].Hand, CHARACTER->GetBoundSphere())
			&& !m_stHandSphere[i].IsOnceAttack)
		{
			m_stHandSphere[i].IsOnceAttack = true;
			return true;
		}
	}
	//if (IntersectSphere(m_stHandSphere.LeftHand1, CHARACTER->GetBoundSphere())
	//	&& !m_stHandSphere.IsOnceAttack1)
	//{
	//	m_stHandSphere.IsOnceAttack1 = true;
	//	return true;
	//}
	//
	//if (IntersectSphere(m_stHandSphere.LeftHand2, CHARACTER->GetBoundSphere())
	//	&& !m_stHandSphere.IsOnceAttack2)
	//{
	//	m_stHandSphere.IsOnceAttack2 = true;
	//	return true;
	//}
	//
	//if (IntersectSphere(m_stHandSphere.RightHand1, CHARACTER->GetBoundSphere())
	//	&& !m_stHandSphere.IsOnceAttack3)
	//{
	//	m_stHandSphere.IsOnceAttack3 = true;
	//	return true;
	//}
	//
	//if (IntersectSphere(m_stHandSphere.RightHand2, CHARACTER->GetBoundSphere())
	//	&& !m_stHandSphere.IsOnceAttack4)
	//{
	//	m_stHandSphere.IsOnceAttack4 = true;
	//	return true;
	//}

	return false;
}

bool RealFinalboss::AblePassive()
{
	/*
	if (m_fSkillCoolTimeCount >= m_nSkillCooltime
	&& leng <= m_stSkill.fMaxLength
	&& m_bIsTargeting
	&& !m_bIsRespawn
	&& !m_bUsingSkill
	&& m_pModel->IsAnimationEnd())
	*/
	//패시브 발동 조건
	if (m_pModel->IsAnimationEnd()
		&& m_fPassiveCooltimeCount >= m_nPassiveCooltime
		&& !m_bIsDead
		&& !m_bUsingPassive
		)
	{

		return true;
	}

	return false;
}

void RealFinalboss::BuffDecide()
{
	ZeroMemory(&m_stPassive.buffStatus, sizeof(STATUS));

	int rndNum = rand() % 3;
	rndNum = 0;
	
	string particleName;
	switch (rndNum)
	{
	case 0:
		m_stPassive.buffStatus.chr.nAtk = 2 * ATK(m_pMonsterStat);
		particleName = "공격력 강화";
		//m_pBuffPrticle = PARTICLE->GetParticle("공격력 강화");
		break;

	case 1:
		m_stPassive.buffStatus.chr.nDef =2 * DEF(m_pMonsterStat);
		particleName = "방어력 강화";
		//m_pBuffPrticle = PARTICLE->GetParticle("방어력 강화");
		break;

	case 2:
		m_stPassive.buffStatus.chr.fHit = HIT(m_pMonsterStat);
		particleName = "명중률 강화";
		//m_pBuffPrticle = PARTICLE->GetParticle("명중률 강화");
		break;
	default:
		break;
	}

	for (int i = 0; i < HT_END; ++i)
	{
		SAFE_DELETE(m_pBuffPrticle[i]);
		m_pBuffPrticle[i] = PARTICLE->GetParticle(particleName);
	}
}
