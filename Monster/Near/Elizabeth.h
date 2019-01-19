#pragma once
#include "NearMonster.h"

class Elizabeth :
	public NearMonster
{

private:

	virtual void SetupStat() override;
	virtual void SetupSkill() override;

	virtual void SkillUse() override;

	virtual void DropItemSetup() override;

public:
	Elizabeth();
	virtual ~Elizabeth();

	virtual void Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon = false) override;
};

