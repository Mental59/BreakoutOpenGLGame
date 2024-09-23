#pragma once

#include <memory>
#include <random>
#include <glm/glm.hpp>
#include "Game/GameObject.h"
#include "Particle.h"

class ParticleEmitter : public GameObject
{
public:
	struct ParticleEmitterInitOptions : GameObjectInitOptions
	{
		unsigned int NumParticles = 500;
		unsigned int NumNewParticlesEachUpdate = 2;
		float MinParticleLifetimeSeconds = 0.8f;
		float MaxParticleLifetimeSeconds = 1.2f;
	};

	ParticleEmitter() = default;

	void Init(const ParticleEmitterInitOptions& options);

	inline unsigned int GetNumParticles() const { return mNumParticles; }
	inline const Particle* GetParticles() const { return mParticles.get(); }

	void Update(float dt, glm::vec2 offset);
	void SpawnParticle(Particle* particle, glm::vec2 offset);

private:
	std::mt19937 mRNG;
	unsigned int mNumParticles;
	unsigned int mNumNewParticlesEachUpdate;
	std::unique_ptr<Particle[]> mParticles;
};
