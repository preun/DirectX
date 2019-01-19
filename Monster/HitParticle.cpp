#include "../../stdafx.h"
#include "HitParticle.h"



HitParticle::HitParticle()
{
}


HitParticle::~HitParticle()
{
	for (int i = 0; i < m_vHitParticle.size(); ++i)
	{
		SAFE_DELETE(m_vHitParticle[i]);
	}
}

void HitParticle::Setup(string ParticleName)
{
	m_sParticleName = ParticleName;
	
}

void HitParticle::Update(D3DXVECTOR3 pos, float rotY)
{
	for (int i = 0; i < m_vHitParticle.size(); ++i)
	{
		m_vHitParticle[i]->SetPosition(pos);
		m_vHitParticle[i]->SetRotation(D3DXVECTOR3(0, rotY, 0));

		m_vHitParticle[i]->Update();
		m_vHitParticle[i]->ApplyWorld();
	}
}

void HitParticle::Render()
{
	for (int i = 0; i < m_vHitParticle.size(); ++i)
	{
		m_vHitParticle[i]->Render();		
	}
}

void HitParticle::AddParticle(D3DXVECTOR3 pos, float rotY)
{
	//벡터를 돌아서 파티클 중에 내가 재활용할수 있는 파티클이 있다면
	for (int i = 0; i < m_vHitParticle.size(); ++i)
	{
		//1회만 돌고 
		if (m_vHitParticle[i]->IsDie())
		{
			m_vHitParticle[i]->TimeReset();
			m_vHitParticle[i]->SetPosition(pos);
			m_vHitParticle[i]->SetRotation(D3DXVECTOR3(0, rotY, 0));

			return;
		}
	}

	//여기에 왔다면 새로 넣어야 한다.
	Particle* tempParticle;
	tempParticle = PARTICLE->GetParticle(m_sParticleName);
	m_vHitParticle.push_back(tempParticle);

}
