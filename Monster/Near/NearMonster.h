#pragma once
#include "../MonsterParent.h"

class NearMonster :
	public MonsterParent
{
protected:

	virtual void SetupStat() override;
	virtual void SetupSkill() override;

	//근접 몬스터 공격함수
	virtual void Attack() override;
	//근접 몬스터 스킬함수
	virtual void SkillUse() override;
	//근접 몬스터 기본 이동함수
	virtual void Move() override;
	virtual void SummonMove() override;

	virtual void DropItemSetup() override;
public:
	NearMonster();
	virtual ~NearMonster();

	virtual void Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon = false) override;
	
};

