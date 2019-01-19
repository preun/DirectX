#pragma once
#include "BossParent.h"

#define STONENUM 5
#define RNDMIN -10
#define RNDMAX 10

//struct ST_HANDMAT
//{
//	D3DXMATRIX* LeftHand1;	//���� �Ʒ� ��
//	D3DXMATRIX* LeftHand2;	//���� �� ��
//	D3DXMATRIX* RightHand1;	//������ �Ʒ� ��
//	D3DXMATRIX* RightHand2;	//������ �� ��
//};

struct ST_HANDSPHERE
{
	ST_SPHERE Hand;	//���� �Ʒ� ��
	bool IsOnceAttack;		//�� �տ� �¾ҳ�

	//ST_SPHERE LeftHand2; 	//���� �� ��
	//bool IsOnceAttack2;		//�� �տ� �¾ҳ�
	//
	//ST_SPHERE RightHand1;	//������ �Ʒ� ��
	//bool IsOnceAttack3;		//�� �տ� �¾ҳ�
	//
	//ST_SPHERE RightHand2;	//������ �� ��
	//bool IsOnceAttack4;		//�� �տ� �¾ҳ�
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

	//�� �������� ��ġ�� ������
	vector<MagicCircle*> m_vMagicCircle;

	//�� �����ִ� ����Ʈ������Ʈ Ŭ����
	vector<EffectObject*> m_vEffectObject;

	Particle* m_pBuffPrticle[HT_END];

	//�յ��� ��Ʈ���� ����ü
	//ST_HANDMAT m_stHandMat;
	D3DXMATRIX* m_pHand[HT_END];
	//��Ʈ������ �̿��� �յ��� ����ü
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
	//�нú꽺ų �Լ�
	virtual void Passive() override;
	//�нú꽺ų �ߵ� ���� �Լ�
	//bool AbleSummon();
	//��ų 2 �Լ�
	virtual void Skill2() override;

	virtual void CastingCancel() override;
	//��ų 2 ĳ�����Լ�
	//virtual void Casting() override;

	//���� �ִϸ��̼� ��������
	void EnterAni();
	//�� ���� ��Ʈ���� ����
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

