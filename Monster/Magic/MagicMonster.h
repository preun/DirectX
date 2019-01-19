#pragma once
#include "../MonsterParent.h"

class MagicCircle;

class MagicMonster :
	public MonsterParent
{

protected:
	//���ݽ� �����Ǵ� ��
	MagicCircle* m_pMagicCircle;

	Particle* m_pParticle;
	//����Ÿ���� ������ �־����� �������� ������ �Ǿ�� �Ѵ�.
	bool m_bIsAttack;

protected:
	 
	virtual void SetupStat() override;

	//���� ���� �����Լ�
	virtual void Attack() override;
	//���� ���� ��ų�Լ�
	virtual void SkillUse() override;
	//���� ���� �⺻ �̵��Լ�
	virtual void Move() override;
	virtual void SummonMove() override;

	virtual void DropItemSetup() override;


public:
	MagicMonster();
	virtual ~MagicMonster();

	virtual void Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon = false) override;

	virtual void Update() override;
	
	virtual void Render() override;

	virtual void Respawn(D3DXVECTOR3 spawnPos) override;
	
};

