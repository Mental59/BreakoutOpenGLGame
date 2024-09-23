#include "Particle.h"

void Particle::Init(const ParticleInitOptions& options)
{
	GameObject::Init(options);
	mLifetimeSeconds = options.LifetimeSeconds;
	mLeftLifetimeSeconds = 0.0f;
}
