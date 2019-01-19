#pragma once
#include "../Status.h"
#include "../SkillManager.h"
//몬스터들의 기본 틀이 되는 클래스이다.

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
	MS_IDLE,			//제자리에 있는 상태
	MS_RUN,				//비전투에 걷는 상태
	MS_SKILL,			//스킬 사용 상태
	MS_ATTACK,			//전투 상태
	MS_MOVEFORATTACK,	//전투 중 이동해야 하는 상태
	MS_DIE,				//죽은 상태
	MS_NONE,			//죽고나서 애니안돌리기 위한 값
};

class MonsterParent
{

	//리스폰상태인지 확인
	//참이면 죽어서 리스폰중
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

	//체력 바 이동하기 위한 변수
	float m_fUIMoveX;
	float m_fUIMoveY;

	//타겟팅이 되었냐(데미지를 입었냐)
	bool m_bIsTargeting;
	//공격행동 초기화 카운트
	int m_nTargetingCount;

	//보스가 소환해서 생성된 몹이냐
	

	//HP UI바
	UIObject* m_pHPBar;

	//몬스터가 가는 방향
	D3DXVECTOR3		m_vDir;

	//이동 패턴시 행동 바뀌는 카운트
	int				m_nPatternChangeCount;
	int				m_nMinMoveCount, m_nMaxMoveCount;

	int				m_nCount;
	//몬스터 스탯
	//STATUS m_pMonsterStat;

	//리스폰될떄까지 시간
	int m_nResPawnCount;

	float m_fAlphaCount;

	//몬스터 상태
	MON_STATE m_eState;
	//몬스터가 현재 있는 곳의 맵
	Map* m_pMap;

	//몬스터의 A*
	AStar* m_pAStar;
	//A* 총 경로
	vector<D3DXVECTOR3> m_vPath;
	//플레이어의 이전 포지션
	int m_nPrePosIndex;
	//드랍 아이템 목록
	vector<int> m_vItemID;

	//각각의 몬스터 스킬
	Skill* m_pSkill;

	ST_SKILL m_stSkill;

	float	m_fSkillCoolTimeCount;
	int		m_nSkillCooltime;
	bool	m_bUsingSkill;

	DamageUI* m_pDamageUI;

	//Particle*	m_pHitParticle;
	HitParticle* m_pHitParticle;

	
	bool		m_bSpecial;

	//npc 등장시키는 몬스터냐
	bool		m_bAppearNPC;
	//다음 맵으로 넘어갈 키 몬스터냐
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
	//몬스터 스킬함수
	virtual void SkillUse();
	//몬스터 기본 이동함수
	virtual void Move();
	virtual void SummonMove();
	//몬스터 별 드랍아이템 세팅함수
	virtual void DropItemSetup();
	
	
	//움직임 관련 리셋함수 (반대편으로 가야하냐)
	void MoveReset(bool isReverse, int max= 0 , int min= 0);
	//공격할수 있는 거리까지 가는 함수
	POINT MoveForAttack();
	void ItemDrop();

	virtual void SetCurrentHP(int hp, int* deathCount);
	

	virtual void ChangeAni();

	void ChangeRot();
	
	//스킬사용가능하냐
	virtual bool AbleSkill();
	

	//npc 등장시키는 몬스터냐
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

	//키몬스터 죽었냐
	virtual bool IsDie();

	void ShowStat();


};

