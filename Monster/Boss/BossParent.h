#pragma once
#include "../MonsterParent.h"

class MagicCircle;
class Skill;

enum BOSS_STATE{
	BS_ENTER,			//���� �������
	BS_IDLE,			//���ڸ��� �ִ� ����
	BS_RUN,				//���� �ٴ� ����
	BS_PASSIVE,			//���� �нú� ����
	BS_ATTACK,			//���� ���� ����
	BS_SKILL1,			//��ų1 ����
	BS_SKILL2,			//��ų2 ����
	BS_CASTING,			//��ų2 ĳ������
	BS_STUN,			//���� ���ϻ���
	BS_DIE,				//���� �������
	BS_NONE,			//��� �ִϸ��̼� ������ ���߱� ���� ����
	//�������������� ���� ���°�
	BS_ENTER1,			//���� �ִϸ��̼� ����1
	BS_ENTER2,			//���� �ִϸ��̼� ����2
	BS_ENTER3 			//���� �ִϸ��̼� ����3

};

class BossParent :
	public MonsterParent
{
	SET(MonsterManager*, m_pMM, MM);

protected : 

	//���ݽ� �����Ǵ� ��
	MagicCircle* m_pMagicCircle;

	//������ ������ �־����� �������� ������ �Ǿ�� �Ѵ�.
	bool m_bIsAttack;
	//������ ��� ����
	bool m_bIsDead;

	BOSS_STATE m_eBossState;

	//�������� ������ �ִ� 2��ų
	Skill* m_pSkill2;

	//2��ų ����
	ST_SKILL m_stSkill2;	

	//2��ų ��Ÿ�� ī��Ʈ
	float m_fSkillCoolTimeCount2 ;
	//2��ų ��Ÿ��
	int m_nSkillCooltime2;
	//��밡���ϳ�
	bool m_bUsingSkill2;
	
	/*
	
	UIObject* m_pHPBar;

	D3DXVECTOR3		m_vDir;

	int				m_nPatternChangeCount;
	int				m_nMinMoveCount, m_nMaxMoveCount;

	int				m_nCount;
	//���� ����
	STATUS m_pMonsterStat;

	//����ī��Ʈ
	//int m_nAttackDelay;
	
	//Model* m_pDeathModel;
	
	MON_STATE m_eState;
	Map* m_pMap;

	AStar* m_pAStar;

	//��� ������ ���
	int m_nItemID[MAXITEMNUM];
	*/


protected:

	virtual void RespawnUpdate() override
	{
		Pattern();

		if (m_pHPBar)
		{
			float tempF = (float)CURRENTHP(m_pMonsterStat) / MAXHP(m_pMonsterStat);


			m_pHPBar->SetScale(D3DXVECTOR3(tempF, 1, 1));

			m_pHPBar->Update();
		}
	}

	//���� ���� �¾�
	virtual void SetupStat() override;

	//��ų1 �¾�
	virtual void SetupSkill() override;
	//����2 �¾�
	virtual void SetupSkill2();

	//���� ���� �����Լ�
	virtual void Attack() override;

	//���� ���� ��ų1 ��� �Լ�
	virtual void SkillUse() override;
	//���� ���� ��ų2 ��� �Լ�
	virtual void Skill2() ;
	//���� ���� �нú� �Լ�
	virtual void Passive() ;

	//��ų2 ĳ���� �Լ�
	virtual void Casting() ;

	//���� ���� �⺻ �̵��Լ�
	virtual void Move() override;

	virtual void DropItemSetup() override;

	virtual void ChangeAni() override;

	virtual void Pattern();

	virtual void SetCurrentHP(int hp, int* deathCount) override;

	virtual bool AbleSkill() override;
	bool AbleSkill2();
	void SkillPrepare2();

	virtual void CastingCancel();
	
public:
	BossParent();
	virtual ~BossParent();

	virtual void SetupBoss(Map* map, D3DXVECTOR3 pos) override;
	virtual void Update() override;
	virtual void Render() override;

	virtual bool IsDie() override {
		if (m_bKeyMonster && m_eBossState == BS_NONE) return true;
		return false;
	}
	//virtual void Update() override;
};

