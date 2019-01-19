#pragma once
#include "../MonsterParent.h"

class DistanceMonster :
	public MonsterParent
{
protected:
	vector<D3DXVECTOR3> m_vWallVertex;

protected:

	//���Ÿ� ���� �����Լ�
	virtual void Attack() override;
	//���Ÿ� ���� ��ų�Լ�
	virtual void SkillUse() override;
	//���Ÿ� ���� �⺻ �̵��Լ�
	virtual void Move() override;
	virtual void SummonMove() override;

	virtual void DropItemSetup() override;

	virtual void SetupStat() override;

	bool AttackPossible();
public:
	DistanceMonster();
	virtual ~DistanceMonster();

	virtual void Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon = false) override;
	

	
};

