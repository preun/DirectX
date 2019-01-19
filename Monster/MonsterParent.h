#pragma once
#include "../Status.h"
#include "../SkillManager.h"
//���͵��� �⺻ Ʋ�� �Ǵ� Ŭ�����̴�.

class Map;
class CharacterParant;
class AStar;
class DropManager;
class UIObject;
class Skill;
class DamageUI;
class BossParent;
class Npc;
class HitParticle;

enum MON_STATE{
	MS_IDLE,			//���ڸ��� �ִ� ����
	MS_RUN,				//�������� �ȴ� ����
	MS_SKILL,			//��ų ��� ����
	MS_ATTACK,			//���� ����
	MS_MOVEFORATTACK,	//���� �� �̵��ؾ� �ϴ� ����
	MS_DIE,				//���� ����
	MS_NONE,			//�װ��� �ִϾȵ����� ���� ��
};

class MonsterParent
{

	//�������������� Ȯ��
	//���̸� �׾ ��������
	GET(bool, m_bIsRespawn, IsResPawn);
	GET(Model*, m_pModel, Model);
	SET(CharacterParant**, m_ppCharacter, Character);
	SET(DropManager*, m_pDropManager, DropManager);
	SYNTHESIZE(STATUS*, m_pMonsterStat, MosterStat);
	GET(bool, m_bIsSummon,IsSummon);
	SET(Npc*, m_pNpc, NPC);
	SET(bool, m_bAbleSkill, AbleSkill);
	SET(bool, m_bIsSpeedwagon, IsSpeedwagon);
	//SET(bool, m_bKeyMonster, KeyMonster)
	//Npc*		m_pNpc;
	//SET(bool, m_bIsTargeting, IsTargeting);

protected:

	//ü�� �� �̵��ϱ� ���� ����
	float m_fUIMoveX;
	float m_fUIMoveY;

	//Ÿ������ �Ǿ���(�������� �Ծ���)
	bool m_bIsTargeting;
	//�����ൿ �ʱ�ȭ ī��Ʈ
	int m_nTargetingCount;

	//������ ��ȯ�ؼ� ������ ���̳�
	

	//HP UI��
	UIObject* m_pHPBar;

	//���Ͱ� ���� ����
	D3DXVECTOR3		m_vDir;

	//�̵� ���Ͻ� �ൿ �ٲ�� ī��Ʈ
	int				m_nPatternChangeCount;
	int				m_nMinMoveCount, m_nMaxMoveCount;

	int				m_nCount;
	//���� ����
	//STATUS m_pMonsterStat;

	//�������ɋ����� �ð�
	int m_nResPawnCount;

	float m_fAlphaCount;

	//���� ����
	MON_STATE m_eState;
	//���Ͱ� ���� �ִ� ���� ��
	Map* m_pMap;

	//������ A*
	AStar* m_pAStar;
	//A* �� ���
	vector<D3DXVECTOR3> m_vPath;
	//�÷��̾��� ���� ������
	int m_nPrePosIndex;
	//��� ������ ���
	vector<int> m_vItemID;

	//������ ���� ��ų
	Skill* m_pSkill;

	ST_SKILL m_stSkill;

	float	m_fSkillCoolTimeCount;
	int		m_nSkillCooltime;
	bool	m_bUsingSkill;

	DamageUI* m_pDamageUI;

	//Particle*	m_pHitParticle;
	HitParticle* m_pHitParticle;

	
	bool		m_bSpecial;

	//npc �����Ű�� ���ͳ�
	bool		m_bAppearNPC;
	//���� ������ �Ѿ Ű ���ͳ�
	bool		m_bKeyMonster;

	//bool		m_bAbleSkill;

	/*
	int			nMaxTarget;
	float		fMinLength;
	float		fMaxLength;
	float		fAngle;

	float		fDamage;
	int			nDamageCount;
	float		fDamageInterval;
	float		fDamageDelay;

	float		fBuffTime;
	STATUS		buffStatus;

	float		fYOffset;
	bool		isAutoRot;
	float		fParticleTime;
	float		fEffectTime;
	*/
protected:
	virtual void SetupStat();
	virtual void SetupSkill();

	virtual void Attack();
	//���� ��ų�Լ�
	virtual void SkillUse();
	//���� �⺻ �̵��Լ�
	virtual void Move();
	virtual void SummonMove();
	//���� �� ��������� �����Լ�
	virtual void DropItemSetup();
	
	
	//������ ���� �����Լ� (�ݴ������� �����ϳ�)
	void MoveReset(bool isReverse, int max= 0 , int min= 0);
	//�����Ҽ� �ִ� �Ÿ����� ���� �Լ�
	POINT MoveForAttack();
	void ItemDrop();

	virtual void SetCurrentHP(int hp, int* deathCount);
	

	virtual void ChangeAni();

	void ChangeRot();
	
	//��ų��밡���ϳ�
	virtual bool AbleSkill();
	

	//npc �����Ű�� ���ͳ�
	void IsAppear();


public:
	MonsterParent();
	virtual ~MonsterParent();

	

	void CalculDamage(float damage, int* deathCount = NULL);

	virtual void Setup(Map* map, D3DXVECTOR3 spawnPos, bool isSummon = false);

	virtual void SetupBoss(Map* map, D3DXVECTOR3 pos);

	virtual void Update();

	virtual void RespawnUpdate();

	virtual void Render();

	void SkillPrepare();

	void SetAttak() { m_eState = MS_ATTACK; ChangeAni(); }
	void SetSkill() { m_eState = MS_SKILL;  ChangeAni();  SkillPrepare(); }
	
	virtual void Respawn(D3DXVECTOR3 spawnPos);

	void MakeKeyMonster();

	//Ű���� �׾���
	virtual bool IsDie();

	void ShowStat();


};

