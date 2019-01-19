#include "../MonsterUseHeader.h"
#include "Elizabeth.h"

Elizabeth::Elizabeth()
{

}


Elizabeth::~Elizabeth()
{
	//SAFE_DELETE(m_pModel);
	//SAFE_DELETE(m_pSkill);
}

void Elizabeth::Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon)
{
	//모델부터 만들고
	//MODELMANAGER->AddModel("엘리자베스", "Model/Enemy/Elizabeth/", "Elizabeth.x", MODELTYPE_X);
	m_pModel = MODELMANAGER->GetModel("엘리자베스", MODELTYPE_X);

	m_fUIMoveX = 50.0f;
	m_fUIMoveY = 5.0f;

	//부모의 셋업을 호출해라
	NearMonster::Setup(map, spawnPos, isSummon);

	
	//MODELMANAGER->AddModel("엘리자베스", "Model/Enemy/Assis/", "Assis.x", MODELTYPE_X);
	//MODELMANAGER->AddModel("엘리자베스", "Model/Enemy/DarkHell/", "DarkHell.x", MODELTYPE_X);
	//MODELMANAGER->AddModel("엘리자베스", "Model/Enemy/PhantomKnight/", "PhantomKnight.x", MODELTYPE_X);
	//MODELMANAGER->AddModel("엘리자베스", "Model/Enemy/Nike/", "Nike.x", MODELTYPE_X);



	//판정 박스 
	ST_SIZEBOX box;
	box.highX = 50.0f;
	box.highY = 180.0f;
	box.highZ = 50.0f;
	box.lowX = -50.0f;
	box.lowY = 10.0f;
	box.lowZ = -50.0f;
	
	

	m_pModel->SetScale(D3DXVECTOR3(0.02f, 0.02f, 0.02f));	

	m_pModel->CreateBound(box);
	m_pModel->SetBoundSphere(m_pModel->GetOrigBoundSphere().center, 80.0f);

	//스킬
	m_pSkill = SKILL->GetSkill("Elizabeth Skill");

	/*
	if (INPUT->KeyDown('R'))
	{
		ST_SKILL skill;
		ZeroMemory(&skill, sizeof(ST_SKILL));
		skill.fDamage = 10;	//타당 데미지
		skill.fDamageDelay = 0;	//첫타 시작시간
		skill.fDamageInterval = 0.1; //주기
		skill.fMaxLength = 10;	//사거리
		skill.nDamageCount = 10;	//횟수
		//스킬 사용후 바뀐 값들 처리
		m_pSkill->Prepare(m_pCharacter,
			m_pMM->GetMonsterVector()[0], 
			m_pMM->GetMonsterVector(),	//
			skill,			//내가 갖고 있는 스킬
			SKILLO_MONSTER); //스킬 사용대상자
	}

	//스킬사용
	if (INPUT->KeyDown('T'))
		m_pSkill->Trigger();
	*/
}

void Elizabeth::SetupStat()
{
	m_nMinMoveCount = NRand(800,900);
	m_nMaxMoveCount = NRand(300,400);
	ZeroMemory(m_pMonsterStat, sizeof(STATUS));

	CURRENTHP(m_pMonsterStat) = MAXHP(m_pMonsterStat)  = 100;
	ATK(m_pMonsterStat) = 25;
	PHYRATE(m_pMonsterStat) = 0.9f;
	MAGICRATE(m_pMonsterStat) = 1.3f;
	CHERATE(m_pMonsterStat) = 1.0f;
	ATKSPEED(m_pMonsterStat) = 0.5f;

	DEF(m_pMonsterStat) = 4;
	AGI(m_pMonsterStat) = 10.0f;
	HIT(m_pMonsterStat) = 10.0f;
	SPEED(m_pMonsterStat) = 0.05f;
	RANGE(m_pMonsterStat) = 1.5f;
	//PHYRATE(m_pMonsterStat) = 

	//m_pMonsterStat.CURRENTHP = m_pMonsterStat.MAXHP = 100;


	/*
	int		nCurrentHP;		//캐릭터(적, 플레이어) 현재 피
		int		nMaxHp;			//캐릭터(적, 플레이어) 최대 피

		int		nCurrentStam;	//캐릭터 현재 스테미나
		int		nMaxStam;		//캐릭터 전체 스테미나

		int		nAtk;			//기본공격력
		float	fPhyRate;		//물리계수
		float	fMagicRate;		//마법계수
		float	fCheRate;		//화학계수
		float	fAtkSpeed;		//공격속도 (장비속도에 * 연산)

		int		nDef;			//방어력
		float	fAgi;			//회피력
		float	fHit;			//명중률
		float	fSpeed;			//이동속도
		float	fRange;			//공격 거리
		float	fScale;			//공격 범위
	*/
}

void Elizabeth::SetupSkill()
{
	//m_pSkill = new SkillParent;
	ZeroMemory(&m_stSkill, sizeof(ST_SKILL));
	m_stSkill.fDamage = 70;
	m_stSkill.fDamageDelay = 0;
	m_stSkill.fDamageInterval = 0.1;
	m_stSkill.fMaxLength = 7;
	m_stSkill.fAngle = 360;
	m_stSkill.nMaxTarget = 5;
	m_stSkill.nDamageCount = 1;
	m_stSkill.isAutoRot = true;
	m_stSkill.fYOffset = 1;
	m_stSkill.fBuffTime = -1;
	m_stSkill.fParticleTime = 10;
	m_stSkill.fParticleSpeed = 0.05;
	m_stSkill.fEffectTime = 3;

	m_fSkillCoolTimeCount = 0;
	m_nSkillCooltime = 45;

	//m_stSkill.fDamage = 10;
	//m_stSkill.fDamageInterval = 0.1;
	//m_stSkill.fMaxLength = 10;
	//m_stSkill.nDamageCount = 10;
	//m_stSkill.fEffectTime = 3;
	//m_stSkill.fAngle = 60;
}
	/*
	if (INPUT->KeyDown('R'))
	{
		ST_SKILL skill;
		ZeroMemory(&skill, sizeof(ST_SKILL));
		skill.fDamage = 10;	//타당 데미지
		skill.fDamageDelay = 0;	//첫타 시작시간
		skill.fDamageInterval = 0.1; //주기
		skill.fMaxLength = 10;	//사거리
		skill.nDamageCount = 10;	//횟수
		//스킬 사용후 바뀐 값들 처리
		m_pSkill->Prepare(m_pCharacter,
			m_pMM->GetMonsterVector()[0], 
			m_pMM->GetMonsterVector(),	//
			skill,			//내가 갖고 있는 스킬
			SKILLO_MONSTER); //스킬 사용대상자
	}

	//스킬사용
	if (INPUT->KeyDown('T'))
		m_pSkill->Trigger();
	*/

//void Elizabeth::Attack()
//{
//	//일단 예외처리 -> 플레이어 연결이 되었다면
//
//	if (m_pCharacter)
//	{
//		if (m_pCharacter->GetIsDead())
//		{
//			m_eState = MS_IDLE;
//			ChangeAni();
//			return;
//		}
//		//char test1[111];
//		//sprintf_s(test1, sizeof(test1), "플레이어의 체력 : %d, 엘리자베스의 체력 : %d", m_nAttackDelay);
//		//
//		//TEXT->Add(test1, 10, 10, 30);
//		D3DXVECTOR3 tempV = *m_pModel->GetPosition() - *m_pCharacter->GetCharacter()->GetPosition();
//		float length = D3DXVec3Length(&tempV);
//
//		int a = 10;
//		//공격 가능 사거리까지 하면 될듯
//		if (length > RANGE(m_pMonsterStat))
//		{
//			if (m_eState == MS_ATTACK)
//			{
//				m_eState = MS_MOVEFORATTACK;
//				ChangeAni();
//			}
//			MoveForAttack();
//		}
//		else
//		{
//			char test[111];
//			sprintf_s(test, sizeof(test), "공격딜레이 : %d", m_nAttackDelay);
//
//			TEXT->Add(test, 10, 10, 30);
//
//			if (m_eState == MS_MOVEFORATTACK)
//			{
//				m_eState = MS_ATTACK;
//				ChangeAni();
//			}
//			//플레이어 공격기능 설정
//			m_eState = MS_ATTACK;
//			if (m_nAttackDelay >= ATKSPEED(m_pMonsterStat))
//			{
//				float tatalRate = PHYRATE(m_pMonsterStat) + MAGICRATE(m_pMonsterStat) + CHERATE(m_pMonsterStat);
//				float tatalDamage = tatalRate * ATK(m_pMonsterStat);
//				m_pCharacter->CalculDamage(tatalDamage);
//				m_nAttackDelay = 0;
//			}
//			m_nAttackDelay++;
//		}
//	
//	}
//}

void Elizabeth::SkillUse()
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

//void Elizabeth::Move()
//{
//	
//	if (m_nCount == m_nPatternChangeCount/*INPUT->KeyDown('O')*/)
//	{
//		MoveReset(false,800,300);
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
//		//못가는 곳이다.
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
//	//	{
//	//		m_bIsRespawn = true;
//	//		m_eState = MS_DIE;
//	//		ChangeAni();
//	//		SetCurrentHP(1000);
//	//	}
//		
//	
//}

void Elizabeth::DropItemSetup()
{
	//노템
	m_vItemID.push_back(ITEM_NULL);
	m_vItemID.push_back(NORMAL_RECOVERYBOOK);
	m_vItemID.push_back(RARE_OVE);
	m_vItemID.push_back(NORMAL_SIDE);
	m_vItemID.push_back(MAGE_NORMAL_HELMET);
	m_vItemID.push_back(MAGE_NORMAL_GAUNTLET);
	m_vItemID.push_back(VALKYRIE_RARE_ARMOR);
	m_vItemID.push_back(VALKYRIE_RARE_BOOTS);


}

//void Elizabeth::Update()
//{
//	if (INPUT->KeyDown('P'))
//	{
//		m_bIsRespawn = true;
//		m_eState = MS_DIE;
//		ChangeAni();
//
//	}
//	
//		if (!m_bIsRespawn)
//		{
//			if (m_pModel)
//			{
//				m_pModel->World();
//				m_pModel->Update();
//			}
//		}
//		else
//		{
//			//if (m_pDeathModel)
//			//{
//			//	m_pDeathModel->World();
//			//	m_pDeathModel->Update();
//			//}
//		}
//	
//}

