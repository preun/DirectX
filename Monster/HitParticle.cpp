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
	//���͸� ���Ƽ� ��ƼŬ �߿� ���� ��Ȱ���Ҽ� �ִ� ��ƼŬ�� �ִٸ�
	for (int i = 0; i < m_vHitParticle.size(); ++i)
	{
		//1ȸ�� ���� 
		if (m_vHitParticle[i]->IsDie())
		{
			m_vHitParticle[i]->TimeReset();
			m_vHitParticle[i]->SetPosition(pos);
			m_vHitParticle[i]->SetRotation(D3DXVECTOR3(0, rotY, 0));

			return;
		}
	}

	//���⿡ �Դٸ� ���� �־�� �Ѵ�.
	Particle* tempParticle;
	tempParticle = PARTICLE->GetParticle(m_sParticleName);
	m_vHitParticle.push_back(tempParticle);

}
