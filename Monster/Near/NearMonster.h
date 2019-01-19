#pragma once
#include "../MonsterParent.h"

class NearMonster :
	public MonsterParent
{
protected:

	virtual void SetupStat() override;
	virtual void SetupSkill() override;

	//���� ���� �����Լ�
	virtual void Attack() override;
	//���� ���� ��ų�Լ�
	virtual void SkillUse() override;
	//���� ���� �⺻ �̵��Լ�
	virtual void Move() override;
	virtual void SummonMove() override;

	virtual void DropItemSetup() override;
public:
	NearMonster();
	virtual ~NearMonster();

	virtual void Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon = false) override;
	
};

