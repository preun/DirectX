#pragma once

class MonsterParent;
class Map;
class CharacterParant;
class DropManager;
class Npc;

#define KEYMONSTERCHANCE 0.05f

//������� ���͵��� �Ѱ��ϴ� Ŭ����
class MonsterManager
{

	GET(vector<MonsterParent*>, m_vMM, MonsterVector);
	SET(vector<D3DXVECTOR3>, m_vSpawnSpot, SpawnSpat);

private:
	Map* m_pMap;

	CharacterParant** m_ppCharacter;
	vector<int> m_vSpawnIndex;

	int m_nDieMonsterNum;

	bool m_bAppearMiddleBoss;

	int m_nKeyMonsterIndex;
	bool m_bAppearKeyMonster;

	//������ ���� ������ 
	bool m_bIsSpeedwagon;

	//vector<MonsterParent> m_vMM;

private:

	//����
	void MakeElizabeth(DropManager* pDropManager,int spawnIndex, bool isSummon = false);
	void MakeAssis(DropManager* pDropManager, int spawnIndex, bool isSummon = false);

	//���Ÿ�
	void MakeNerisa(DropManager* pDropManager, int spawnIndex, bool isSummon = false);
	void MakeNike(DropManager* pDropManager, int spawnIndex, bool isSummon = false);

	//����
	void MakeDarkHell(DropManager* pDropManager, int spawnIndex, bool isSummon = false);
	void MakeNifilHeim(DropManager* pDropManager, int spawnIndex, bool isSummon = false);

	//�߰�����
	void MakeMiddleBoss(DropManager* pDropManager);

	void MakeFinalBoss(DropManager* pDropManager);
	
	void Shuffle();

	void Stage1(DropManager* pDropManager);
	void Stage2(DropManager* pDropManager);
	void Stage3(DropManager* pDropManager);
	void Stage4(DropManager* pDropManager);

	void Summon();
	//��ǥ�� �Լ�
	void MakeMonSet1(int stage);
	void MakeMonSet2(int stage);
public:
	MonsterManager();
	~MonsterManager();

	void Setup(Map* map, CharacterParant** character);
	void Update(int stage);
	void Render();

	//���� ����� �Լ�
	void MakeMonster(DropManager* pDropManager,  int stage);
	//���������� �нú����� �����Ǵ� �Լ�
	void MakeMonster();
	//���� ���� ����� ��ȯ ���� ���� �Լ�
	void DeleteSummonMonster();

	bool isKeyMonsterDie();

	void SetNpc(Npc* npc);

	void DamageMonster(int monsterIndex, float damage);


};

