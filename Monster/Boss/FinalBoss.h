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
	//�нú꽺ų �Լ�
	virtual void Passive() override;
	//�нú꽺ų �ߵ� ���� �Լ�
	bool AbleSummon();
	//��ų 2 �Լ�
	virtual void Skill2() override;
	//��ų 2 ĳ�����Լ�
	virtual void Casting() override;
	//ĳ���ý� ����Ǵ� �Լ�
	virtual void CastingCancel() override;

	void MoveSpawn();

public:
	FinalBoss();
	virtual ~FinalBoss();

	virtual void SetupBoss(Map* map, D3DXVECTOR3 pos) override;

	virtual void Render() override;
	
};

