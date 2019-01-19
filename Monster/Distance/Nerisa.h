#pragma once
#include "DistanceMonster.h"

class Nerisa :
	public DistanceMonster
{

private:

	virtual void SetupStat() override;
	virtual void SetupSkill() override;
	//원거리 몬스터 스킬함수
	virtual void SkillUse() override;

	virtual void DropItemSetup() override;

public:
	Nerisa();
	virtual ~Nerisa();

	virtual void Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon = false) override;

};

