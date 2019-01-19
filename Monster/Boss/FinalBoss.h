#pragma once
#include "BossParent.h"

class FinalBoss :
	public BossParent
{
private:

	EffectObject * m_pCasting;

protected:


	//virtual void ChangeAni() override;

	virtual void Pattern() override;

	virtual void SetupStat() override;
	virtual void SetupSkill() override;

	virtual void SetupSkill2() override;

	virtual void Attack() override;
	virtual void Move() override;
	virtual void SkillUse() override;
	//패시브스킬 함수
	virtual void Passive() override;
	//패시브스킬 발동 조건 함수
	bool AbleSummon();
	//스킬 2 함수
	virtual void Skill2() override;
	//스킬 2 캐스팅함수
	virtual void Casting() override;
	//캐스팅시 실행되는 함수
	virtual void CastingCancel() override;

	void MoveSpawn();

public:
	FinalBoss();
	virtual ~FinalBoss();

	virtual void SetupBoss(Map* map, D3DXVECTOR3 pos) override;

	virtual void Render() override;
	
};

