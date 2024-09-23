#pragma once

#include "Game/GameObject.h"

class Particle : public GameObject
{
public:
	struct ParticleInitOptions : public GameObjectInitOptions
	{
		float LifetimeSeconds;
	};

	void Init(const ParticleInitOptions& options);

	inline float GetLeftLifetime() const { return mLeftLifetimeSeconds; }
	inline void SetLeftLifetime(float value) { mLeftLifetimeSeconds = value; }
	inline bool IsAlive() const { return mLeftLifetimeSeconds > 0.0f; }
	inline void RenewLifetime() { mLeftLifetimeSeconds = mLifetimeSeconds; }

private:
	float mLifetimeSeconds;
	float mLeftLifetimeSeconds;
};

