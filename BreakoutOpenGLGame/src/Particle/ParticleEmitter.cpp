#include "ParticleEmitter.h"
#include <memory>
#include <random>
#include <glm/glm.hpp>
#include "Particle.h"

void ParticleEmitter::Init(const ParticleEmitterInitOptions& options)
{
	GameObject::Init(options);
	mNumParticles = options.NumParticles;
	mNumNewParticlesEachUpdate = options.NumNewParticlesEachUpdate;
	mParticles = std::make_unique<Particle[]>(options.NumParticles);

	std::random_device rd;
	mRNG = std::mt19937(rd());

	std::uniform_real_distribution<> dist(options.MinParticleLifetimeSeconds, options.MaxParticleLifetimeSeconds);

	Particle::ParticleInitOptions particleInitOptions;
	particleInitOptions.Sprite = options.Sprite;
	particleInitOptions.Position = options.Position;
	particleInitOptions.Size = options.Size;
	for (unsigned int i = 0; i < options.NumParticles; i++)
	{
		float lifetime = dist(mRNG);
		particleInitOptions.LifetimeSeconds = lifetime;
		mParticles[i].Init(particleInitOptions);
	}
}

void ParticleEmitter::Update(float dt, glm::vec2 offset)
{
	unsigned int numRespawnedParticles = 0;
	for (unsigned int i = 0; i < mNumParticles; i++)
	{
		mParticles[i].SetLeftLifetime(mParticles[i].GetLeftLifetime() - dt);
		if (mParticles[i].IsAlive())
		{
			glm::vec2 particlePos = mParticles[i].GetPosition();
			glm::vec4 particleColor = mParticles[i].GetColor();

			particlePos -= mParticles[i].GetVelocity() * dt;
			particleColor.a -= 2.5f * dt;

			//mParticles[i].SetPosition(particlePos);
			mParticles[i].SetColor(particleColor);
		}
		else if (numRespawnedParticles < mNumNewParticlesEachUpdate)
		{
			SpawnParticle(&mParticles[i], offset);
			numRespawnedParticles++;
		}
	}
}

void ParticleEmitter::SpawnParticle(Particle* particle, glm::vec2 offset)
{
	std::uniform_real_distribution<> dist1(0.0f, 5.0f);
	std::uniform_real_distribution<> dist2(0.0f, 1.5f);

	float random = dist1(mRNG);
	float rColor = dist2(mRNG);

	particle->SetPosition(mPosition + offset + random);
	particle->SetColor(glm::vec4(rColor, rColor, rColor, 1.0f));
	particle->RenewLifetime();
	particle->SetVelocity(mVelocity * 0.1f);
}
