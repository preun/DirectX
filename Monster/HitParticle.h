#pragma once

class HitParticle
{
private:

	vector<Particle*> m_vHitParticle;
	string m_sParticleName;

public:
	HitParticle();
	~HitParticle();

	void Setup(string ParticleName);
	void Update(D3DXVECTOR3 pos, float rotY);
	void Render();

	void AddParticle(D3DXVECTOR3 pos, float rotY);

};

