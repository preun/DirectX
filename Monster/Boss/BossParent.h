#pragma once
#include "../MonsterParent.h"

class MagicCircle;
class Skill;

enum BOSS_STATE{
	BS_ENTER,			//보스 등장상태
	BS_IDLE,			//제자리에 있는 상태
	BS_RUN,				//보스 뛰는 상태
	BS_PASSIVE,			//보스 패시브 상태
	BS_ATTACK,			//보스 공격 상태
	BS_SKILL1,			//스킬1 상태
	BS_SKILL2,			//스킬2 상태
	BS_CASTING,			//스킬2 캐스팅중
	BS_STUN,			//보스 스턴상태
	BS_DIE,				//보스 사망상태
	BS_NONE,			//사망 애니메이션 끝나고 멈추기 위한 상태
	//레알최종보스를 위한 상태값
	BS_ENTER1,			//등장 애니메이션 연결1
	BS_ENTER2,			//등장 애니메이션 연결2
	BS_ENTER3 			//등장 애니메이션 연결3

};

class BossParent :
	public MonsterParent
{
	SET(MonsterManager*, m_pMM, MM);

protected : 

	//공격시 생성되는 구
	MagicCircle* m_pMagicCircle;

	//보스는 공격중 멀어져도 지속적인 공격이 되어야 한다.
	bool m_bIsAttack;
	//보스의 사망 여부
	bool m_bIsDead;

	BOSS_STATE m_eBossState;

	//보스만이 가지고 있는 2스킬
	Skill* m_pSkill2;

	//2스킬 스탯
	ST_SKILL m_stSkill2;	

	//2스킬 쿨타임 카운트
	float m_fSkillCoolTimeCount2 ;
	//2스킬 쿨타임
	int m_nSkillCooltime2;
	//사용가능하냐
	bool m_bUsingSkill2;
	
	/*
	
	UIObject* m_pHPBar;

	D3DXVECTOR3		m_vDir;

	int				m_nPatternChangeCount;
	int				m_nMinMoveCount, m_nMaxMoveCount;

	int				m_nCount;
	//몬스터 스탯
	STATUS m_pMonsterStat;

	//공격카운트
	//int m_nAttackDelay;
	
	//Model* m_pDeathModel;
	
	MON_STATE m_eState;
	Map* m_pMap;

	AStar* m_pAStar;

	//드랍 아이템 목록
	int m_nItemID[MAXITEMNUM];
	*/


protected:

	virtual void RespawnUpdate() override
	{
		Pattern();

		if (m_pHPBar)
		{
			float tempF = (float)CURRENTHP(m_pMonsterStat) / MAXHP(m_pMonsterStat);


			m_pHPBar->SetScale(D3DXVECTOR3(tempF, 1, 1));

			m_pHPBar->Update();
		}
	}

	//보스 스탯 셋업
	virtual void SetupStat() override;

	//스킬1 셋업
	virtual void SetupSkill() override;
	//스킐2 셋업
	virtual void SetupSkill2();

	//보스 몬스터 공격함수
	virtual void Attack() override;

	//보스 몬스터 스킬1 사용 함수
	virtual void SkillUse() override;
	//보스 몬스터 스킬2 사용 함수
	virtual void Skill2() ;
	//보스 몬스터 패시브 함수
	virtual void Passive() ;

	//스킬2 캐스팅 함수
	virtual void Casting() ;

	//근접 몬스터 기본 이동함수
	virtual void Move() override;

	virtual void DropItemSetup() override;

	virtual void ChangeAni() override;

	virtual void Pattern();

	virtual void SetCurrentHP(int hp, int* deathCount) override;

	virtual bool AbleSkill() override;
	bool AbleSkill2();
	void SkillPrepare2();

	virtual void CastingCancel();
	
public:
	BossParent();
	virtual ~BossParent();

	virtual void SetupBoss(Map* map, D3DXVECTOR3 pos) override;
	virtual void Update() override;
	virtual void Render() override;

	virtual bool IsDie() override {
		if (m_bKeyMonster && m_eBossState == BS_NONE) return true;
		return false;
	}
	//virtual void Update() override;
};

