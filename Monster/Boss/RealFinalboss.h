#pragma once
#include "BossParent.h"

#define STONENUM 5
#define RNDMIN -10
#define RNDMAX 10

//struct ST_HANDMAT
//{
//	D3DXMATRIX* LeftHand1;	//왼쪽 아래 손
//	D3DXMATRIX* LeftHand2;	//왼쪽 위 손
//	D3DXMATRIX* RightHand1;	//오른쪽 아래 손
//	D3DXMATRIX* RightHand2;	//오른쪽 위 손
//};

struct ST_HANDSPHERE
{
	ST_SPHERE Hand;	//왼쪽 아래 손
	bool IsOnceAttack;		//이 손에 맞았냐

	//ST_SPHERE LeftHand2; 	//왼쪽 위 손
	//bool IsOnceAttack2;		//이 손에 맞았냐
	//
	//ST_SPHERE RightHand1;	//오른쪽 아래 손
	//bool IsOnceAttack3;		//이 손에 맞았냐
	//
	//ST_SPHERE RightHand2;	//오른쪽 위 손
	//bool IsOnceAttack4;		//이 손에 맞았냐
};

enum HAND_TIYE
{
	HT_LD,
	HT_LT,
	HT_RD,
	HT_RT,
	HT_END
};

class RealFinalboss :
	public BossParent
{
protected:

	//돌 떨어지는 위치의 마법진
	vector<MagicCircle*> m_vMagicCircle;

	//돌 떨궈주는 이펙트오브젝트 클래스
	vector<EffectObject*> m_vEffectObject;

	Particle* m_pBuffPrticle[HT_END];

	//손들의 매트리스 구조체
	//ST_HANDMAT m_stHandMat;
	D3DXMATRIX* m_pHand[HT_END];
	//매트리스를 이용한 손들의 구조체
	ST_HANDSPHERE m_stHandSphere[HT_END];

	ST_EFFECT m_stEffect;

	Skill* m_pPassive;

	ST_SKILL m_stPassive;

	float m_fPassiveCooltimeCount;

	int m_nPassiveCooltime;

	bool m_bUsingPassive;
	bool m_bDropStone;
	//virtual void ChangeAni() override;

private:

	virtual void Pattern() override;

	virtual void SetupStat() override;
	virtual void SetupSkill() override;
	virtual void SetupSkill2() override;
	void SetupPassive();

	virtual void Attack() override;
	virtual void Move() override;
	virtual void SkillUse() override;
	//패시브스킬 함수
	virtual void Passive() override;
	//패시브스킬 발동 조건 함수
	//bool AbleSummon();
	//스킬 2 함수
	virtual void Skill2() override;

	virtual void CastingCancel() override;
	//스킬 2 캐스팅함수
	//virtual void Casting() override;

	//등장 애니메이션 순차실행
	void EnterAni();
	//팔 들의 메트리스 지정
	void HandMatInit();

	void DropTheStones();
	void DropTheStone();

	bool IsEnter() {
		if (m_eBossState == BS_ENTER ||
			m_eBossState == BS_ENTER1 ||
			m_eBossState == BS_ENTER2 ||
			m_eBossState == BS_ENTER3)
			return true;
		return false;
	}

	void MakeSphere();
	void Debug();

	bool HandCollision();
	void BoolInit()
	{
		for (int i = 0; i < 4; ++i)
		{
			m_stHandSphere[i].IsOnceAttack = false;
		}
		//m_stHandSphere.IsOnceAttack1 =
		//	m_stHandSphere.IsOnceAttack2 =
		//	m_stHandSphere.IsOnceAttack3 =
		//	m_stHandSphere.IsOnceAttack4 = false;
	}

	bool AblePassive();
	void BuffDecide();

	void PassivePrepare()
	{

		m_pPassive->Prepare(PCHARACTER,
			this,
			NULL,
			m_stPassive,
			SKILLO_MONSTER);


	}

public:
	RealFinalboss();
	virtual ~RealFinalboss();

	virtual void SetupBoss(Map* map, D3DXVECTOR3 pos) override;
	virtual void Update() override;
	virtual void Render() override;
};

