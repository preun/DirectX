#include "MonsterUseHeader.h"
#include "MonsterManager.h"
#include "TotalMonster.h"
#include "../Map.h"


MonsterManager::MonsterManager()
{

}


MonsterManager::~MonsterManager()
{
	//
	/*
	GET(vector<MonsterParent*>, m_vMM, MonsterVector);
	SET(vector<D3DXVECTOR3>, m_vSpawnSpot, SpawnSpat);


	Map* m_pMap;

	CharacterParant** m_ppCharacter;
	*/
	//SAFE_DELETE(m_pMap);
	for (int i = 0; i < m_vMM.size(); i++)
	{
		SAFE_DELETE(m_vMM[i]);
	}

	m_pMap = NULL;
	m_ppCharacter = NULL;

}

void MonsterManager::Setup(Map* map, CharacterParant** character)
{
	m_pMap = map;
	m_ppCharacter = character;
	
	for (int i = 0; i < m_pMap->GetSpawnEnemy().size(); ++i)
	{
		m_vSpawnIndex.push_back(i);
	}

	Shuffle();

	m_nDieMonsterNum = m_bAppearMiddleBoss = m_bAppearKeyMonster = m_bIsSpeedwagon = 0;

	m_nKeyMonsterIndex = -1;

}

void MonsterManager::Update(int stage)
{
	assert(m_vSpawnSpot.size() > 0 && "������� ���Ͱ� �����ϴ�.");

	char temp[222];
	sprintf_s(temp, sizeof(temp), "���� ���� �� : %d", m_nDieMonsterNum);
	TEXT->Add(temp, 100, 100, 20);

	for (int i = 0; i < m_vMM.size(); i++)
	{
		if (m_vMM[i]->GetIsResPawn())
		{

			//������ ���̸� ���� �ð����� ���ư���.
			m_vMM[i]->RespawnUpdate();

			//������ Ư���ð��� �Ǹ� �� �οﰪ�� false�� �ǰ� ��ȯ��Ű�� ���̵ǹǷ�
			//Ư�� ��ġ�� ����� ��Ų��.
			if (!m_vMM[i]->GetIsResPawn())
			{
				//m_vMM[i]->~MonsterParent();

				int spotSize = m_vSpawnSpot.size();

				srand(time(NULL));

				//���Ϳ��� ���� �ε��� ����
				int spotIndex = rand() % spotSize;

				m_vMM[i]->Respawn(m_vSpawnSpot[spotIndex]);

				if (FRand(0, 1) <= KEYMONSTERCHANCE && !m_bAppearKeyMonster && (stage == 0 || stage || 2))
				{
					m_bAppearKeyMonster = true;
					m_nKeyMonsterIndex = i;
					m_vMM[i]->MakeKeyMonster();

				}

				//�¾� ȣ���Ͽ� �ٽ� ������Ѵ�.
				//m_vMM[i]->Setup(m_pMap, m_vSpawnSpot[spotIndex]);


			}
		}
		else
		{
			m_vMM[i]->Update();
		}
	}

	if (stage == 1 && m_nDieMonsterNum >= 10 && !m_bAppearKeyMonster)
	{
		MakeMiddleBoss(NULL);
		m_bAppearKeyMonster = true;
		m_nKeyMonsterIndex = 20;
	}

	//1Ű ������ ���� ����(1�������� �����ں���, 2�������� ��������, 3�������� �ʸ���
	if (INPUT->KeyDown('1'))
	{
		MakeMonSet1(stage);
	}

	//2Ű ������ ���� ����(1�������� �ƽý�, 2�������� ��ũ��, 3�������� ����
	if (INPUT->KeyDown('2'))
	{
		MakeMonSet2(stage);
	}
	//�������϶� Ű�� �����ٸ� ��ų�� �����ֱ� ����Ű
	if (m_bIsSpeedwagon && INPUT->KeyDown('3'))
	{
		m_vMM[0]->SetIsSpeedwagon(true);
		m_vMM[0]->SetAbleSkill(true);
		m_vMM[0]->SkillPrepare();
	}

	if (INPUT->KeyDown('4') && stage == 1)
	{
		m_bAppearKeyMonster = true;
		m_nKeyMonsterIndex = m_vMM.size();

		MakeMiddleBoss(NULL);
		
		//m_vMM[m_nKeyMonsterIndex]->MakeKeyMonster();
	}

	if (INPUT->KeyDown('5') /*&& (stage == 1 || stage == 3)*/)
	{
		if (m_nKeyMonsterIndex == -1) return;

		DamageMonster(m_nKeyMonsterIndex, 11111);
	}
	//�������̶��
	if (m_bIsSpeedwagon)
	{
		m_vMM[0]->ShowStat();
	}
}

void MonsterManager::Render()
{
	assert(m_vSpawnSpot.size() > 0 && "������� ���Ͱ� �����ϴ�.");

	//TEXT->Render();
	for (int i = 0; i < m_vMM.size(); i++)
	{
		m_vMM[i]->Render();
	}

	if (m_bIsSpeedwagon)
	{
		TEXT->Render();
	}
}


//���� ����� �Լ�
void MonsterManager::MakeMonster(DropManager* pDropManager, int stage)
{
	switch (stage)
	{
	case 0:
		Stage1(pDropManager);
		break;

	case 1:
		Stage2(pDropManager);
		break;

	case 2:
		Stage3(pDropManager);
		break;

	case 3:
		Stage4(pDropManager);
		break;

	default:
		break;
	}
	/*
	MakeElizabeth(pDropManager);

	MakeAssis(pDropManager);
	//
	MakeDarkHell(pDropManager);
	//
	MakeNifilHeim(pDropManager);

	MakeNerisa(pDropManager);

	MakeNike(pDropManager);
	//
	//
	//MakeFinalBoss(pDropManager);
	*/
	
}

void MonsterManager::MakeMonster()
{
	
	Summon();
	//MakeElizabeth(NULL,0, true);
	//
	//MakeAssis(NULL, 0, true);
	//
	//MakeDarkHell(NULL, 0, true);
	//
	//MakeNifilHeim(NULL, 0, true);
	//
	//MakeNerisa(NULL, 0, true);
	//
	//MakeNike(NULL, 0, true);
}

//����
void MonsterManager::MakeElizabeth(DropManager* pDropManager, int spawnIndex, bool isSummon)
{

	//���� ����
	MonsterParent* elizabeth;
	elizabeth = new Elizabeth;
	elizabeth->Setup(m_pMap, m_vSpawnSpot[m_vSpawnIndex[spawnIndex]], isSummon);
	elizabeth->SetCharacter(m_ppCharacter);
	elizabeth->SetDropManager(pDropManager);

	m_vMM.push_back(elizabeth);
}

void MonsterManager::MakeAssis(DropManager* pDropManager, int spawnIndex, bool isSummon)
{
	

	//���� ����
	MonsterParent* assis;
	assis = new Assis;
	assis->Setup(m_pMap, m_vSpawnSpot[m_vSpawnIndex[spawnIndex]], isSummon);
	assis->SetCharacter(m_ppCharacter);
	assis->SetDropManager(pDropManager);

	m_vMM.push_back(assis);
}


//���Ÿ�
void MonsterManager::MakeNerisa(DropManager* pDropManager, int spawnIndex, bool isSummon)
{
	

	//���� ����
	MonsterParent* nerisa;
	nerisa = new Nerisa;
	nerisa->Setup(m_pMap, m_vSpawnSpot[m_vSpawnIndex[spawnIndex]], isSummon);
	nerisa->SetCharacter(m_ppCharacter);
	nerisa->SetDropManager(pDropManager);

	m_vMM.push_back(nerisa);
}

void MonsterManager::MakeNike(DropManager* pDropManager, int spawnIndex, bool isSummon)
{
	

	//���� ����
	MonsterParent* nike;
	nike = new Nike;
	nike->Setup(m_pMap, m_vSpawnSpot[m_vSpawnIndex[spawnIndex]], isSummon);
	nike->SetCharacter(m_ppCharacter);
	nike->SetDropManager(pDropManager);

	m_vMM.push_back(nike);
}


//����
void MonsterManager::MakeDarkHell(DropManager* pDropManager, int spawnIndex, bool isSummon)
{
	

	//���� ����
	MonsterParent* darkHell;
	darkHell = new DarkHell;
	darkHell->Setup(m_pMap, m_vSpawnSpot[m_vSpawnIndex[spawnIndex]], isSummon);
	darkHell->SetCharacter(m_ppCharacter);
	darkHell->SetDropManager(pDropManager);

	m_vMM.push_back(darkHell);
}

void MonsterManager::MakeNifilHeim(DropManager* pDropManager, int spawnIndex, bool isSummon)
{
	

	//���� ����
	MonsterParent* nifilHeim;
	nifilHeim = new NifilHeim;
	nifilHeim->Setup(m_pMap, m_vSpawnSpot[m_vSpawnIndex[spawnIndex]], isSummon);
	nifilHeim->SetCharacter(m_ppCharacter);
	nifilHeim->SetDropManager(pDropManager);

	m_vMM.push_back(nifilHeim);
}

void MonsterManager::MakeMiddleBoss(DropManager* pDropManager)
{

	MonsterParent* finalBoss;

	finalBoss = new FinalBoss;
	finalBoss->SetupBoss(m_pMap, m_pMap->GetSpawnBoss());
	finalBoss->SetCharacter(m_ppCharacter);
	finalBoss->SetDropManager(pDropManager);
	dynamic_cast<FinalBoss*>(finalBoss)->SetMM(this);
	//finalBoss->SetupBoss(m_pMap, m_vSpawnSpot[0]);

	m_vMM.push_back(finalBoss);

}

void MonsterManager::MakeFinalBoss(DropManager* pDropManager)
{

	MonsterParent* realFinalBoss;

	realFinalBoss = new RealFinalboss;
	realFinalBoss->SetCharacter(m_ppCharacter);
	realFinalBoss->SetupBoss(m_pMap, m_pMap->GetSpawnBoss());
	realFinalBoss->SetDropManager(pDropManager);
	//dynamic_cast<FinalBoss*>(realFinalBoss)->SetMM(this);
	//finalBoss->SetupBoss(m_pMap, m_vSpawnSpot[0]);

	m_vMM.push_back(realFinalBoss);

}

void MonsterManager::Shuffle()
{
	srand(time(NULL));

	for (int i = 0; i < 300; i++) {
		int rnd1 = NRand(0, m_vSpawnIndex.size()-1);
		int rnd2 = NRand(0, m_vSpawnIndex.size()-1);

		int temp = m_vSpawnIndex[rnd2];
		m_vSpawnIndex[rnd2] = m_vSpawnIndex[rnd1];
		m_vSpawnIndex[rnd1] = temp;
	}
}

//����
void MonsterManager::Stage1(DropManager* pDropManager)
{
//	MakeMiddleBoss(pDropManager);

	for (int i = 0; i < 20; i+=2)
	{
		float rnd = FRand(0, 1);
		
		MakeElizabeth(pDropManager, i % m_vSpawnIndex.size() );
		if (/*rnd <= KEYMONSTERCHANCE &&*/ !m_bAppearKeyMonster)
		{
			m_vMM[i]->MakeKeyMonster();
			m_nKeyMonsterIndex = i;
			m_bAppearKeyMonster = true;
		}

		 rnd = FRand(0, 1);
		MakeAssis(pDropManager, (i+1) % m_vSpawnIndex.size());
		if (rnd <= KEYMONSTERCHANCE && !m_bAppearKeyMonster)
		{
			m_vMM[i]->MakeKeyMonster();
			m_nKeyMonsterIndex = i+1;
			m_bAppearKeyMonster = true;
		}
	}
}

//����
void MonsterManager::Stage2(DropManager* pDropManager)
{
	for (int i = 0; i < 20; i += 2)
	{
		MakeDarkHell(pDropManager, i % m_vSpawnIndex.size());

		MakeNifilHeim(pDropManager, (i + 1) % m_vSpawnIndex.size());
	}
}

//���Ÿ�
void MonsterManager::Stage3(DropManager* pDropManager)
{
	for (int i = 0; i < 20; i += 2)
	{
		float rnd = FRand(0, 1);

		MakeNerisa(pDropManager, i % m_vSpawnIndex.size());

		if (/*rnd <= KEYMONSTERCHANCE &&*/ !m_bAppearKeyMonster)
		{
			m_vMM[i]->MakeKeyMonster();
			m_nKeyMonsterIndex = i;
			m_bAppearKeyMonster = true;
		}

		rnd = FRand(0, 1);
		MakeNike(pDropManager, (i + 1) % m_vSpawnIndex.size());

		if (rnd <= KEYMONSTERCHANCE && !m_bAppearKeyMonster)
		{
			m_vMM[i]->MakeKeyMonster();
			m_nKeyMonsterIndex = i + 1;
			m_bAppearKeyMonster = true;
		}
	}
}

void MonsterManager::Stage4(DropManager* pDropManager)
{
	m_bAppearKeyMonster = true;
	m_nKeyMonsterIndex = 0;
	MakeFinalBoss(pDropManager);
}

void MonsterManager::Summon()
{
	MonsterParent* assis;
	assis = new Assis;
	assis->Setup(m_pMap, m_pMap->GetSpawnBoss(), true);
	assis->SetCharacter(m_ppCharacter);
	assis->SetDropManager(NULL);

	m_vMM.push_back(assis);
}

void MonsterManager::MakeMonSet1(int stage)
{
	


	if (stage != 3)
	{
		m_nKeyMonsterIndex = -1;
		m_bAppearKeyMonster = false;
		//�ϴ� ���͸Ŵ��� ���� �� �а�
		for (int i = 0; i < m_vMM.size(); i++)
		{
			SAFE_DELETE(m_vMM[i]);
		}
		m_vMM.clear();

		m_bIsSpeedwagon = true;

		switch (stage)
		{
		case 0:
			MakeElizabeth(NULL, 0, false);
			break;
		case 1:
			MakeNifilHeim(NULL, 0, false);
			break;
		case 2:
			MakeNerisa(NULL, 0, false);
			break;
		default:
			break;
		}
	}
}

void MonsterManager::MakeMonSet2(int stage)
{

	if (stage != 3)
	{
		m_nKeyMonsterIndex = -1;
		m_bAppearKeyMonster = false;

		//�ϴ� ���͸Ŵ��� ���� �� �а�
		for (int i = 0; i < m_vMM.size(); i++)
		{
			SAFE_DELETE(m_vMM[i]);
		}
		m_vMM.clear();

		m_bIsSpeedwagon = true;

		switch (stage)
		{
		case 0:
			MakeAssis(NULL, 0, false);
			break;
		case 1:
			MakeDarkHell(NULL, 0, false);
			break;
		case 2:
			MakeNike(NULL, 0, false);
			break;
		default:
			break;
		}
	}
}

void MonsterManager::DeleteSummonMonster()
{
	

	for (int i = 0; i < m_vMM.size();++i)
	{
		//��ȯ�ȸ��̳�
		if (m_vMM[i]->GetIsSummon())
		{
			m_vMM[i]->CalculDamage(11111);
			//SAFE_DELETE(m_vMM[i]);
			//m_vMM.erase(m_vMM.begin() + i);
		}
		//��ȯ�� ���̸� �ε��� ����
		//else ++i;
	}
}

bool MonsterManager::isKeyMonsterDie()
{
	if (m_nKeyMonsterIndex == -1) return false;



		if (m_vMM[m_nKeyMonsterIndex]->IsDie()) return true;
	

	return false;
}

void MonsterManager::SetNpc(Npc * npc)
{
	for (int i = 0; i < m_vMM.size(); ++i)
	{
		m_vMM[i]->SetNPC(npc);
	}
}

void MonsterManager::DamageMonster(int monsterIndex, float damage)
{
	//�ش� ���Ͱ� ������ ���¶�� ������ ���� ����
	if (m_vMM[monsterIndex]->GetIsResPawn())
	{
		return;
	}
	m_vMM[monsterIndex]->CalculDamage(damage, &m_nDieMonsterNum);
}
