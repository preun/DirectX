#pragma once
#include "../MonsterParent.h"

class MagicCircle;

class MagicMonster :
	public MonsterParent
{

protected:
	//공격시 생성되는 구
	MagicCircle* m_pMagicCircle;

	Particle* m_pParticle;
	//마법타입은 공격중 멀어져도 지속적인 공격이 되어야 한다.
	bool m_bIsAttack;

protected:
	 
	virtual void SetupStat() override;

	//마법 몬스터 공격함수
	virtual void Attack() override;
	//마법 몬스터 스킬함수
	virtual void SkillUse() override;
	//마법 몬스터 기본 이동함수
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

