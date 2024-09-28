#include "PowerUpSpawner.h"
#include <random>
#include <glm/glm.hpp>
#include "Game/PowerUp.h"


PowerUpSpawner::PowerUpSpawner() : mSpawnedPowerups()
{
	std::random_device rd;
	mRNG = std::mt19937(rd());
}

void PowerUpSpawner::Init(const PowerUpSwawnerOptions& options)
{
	mSpeedTexture = options.SpeedTexture;
	mStickyTexture = options.StickyTexture;
	mPassThroughTexture = options.PassThroughTexture;
	mPadSizeIncreaseTexture = options.PadSizeIncreaseTexture;
	mConfuseTexture = options.ConfuseTexture;
	mChaosTexture = options.ChaosTexture;
}

void PowerUpSpawner::SpawnAt(glm::vec2 position)
{
	PowerUp powerUp;
	PowerUp::PowerUpInitOptions initOptions;
	initOptions.Position = position;
	initOptions.Size = glm::vec2(60.0f, 20.0f);
	initOptions.Velocity = glm::vec2(0.0f, 150.0f);

	if (ShouldSpawn(1.0f / 75.0f))
	{
		initOptions.Type = PowerUp::PowerUpType::Speed;
		initOptions.Color = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
		initOptions.Sprite = mSpeedTexture;
		initOptions.Duration = 0.0f;
		powerUp.Init(initOptions);
		mSpawnedPowerups.push_back(std::move(powerUp));
		return;
	}
	if (ShouldSpawn(1.0f / 75.0f))
	{
		initOptions.Type = PowerUp::PowerUpType::Sticky;
		initOptions.Color = glm::vec4(1.0f, 0.5f, 1.0f, 1.0f);
		initOptions.Sprite = mStickyTexture;
		initOptions.Duration = 20.0f;
		powerUp.Init(initOptions);
		mSpawnedPowerups.push_back(std::move(powerUp));
		return;
	}
	if (ShouldSpawn(1.0f / 75.0f))
	{
		initOptions.Type = PowerUp::PowerUpType::PassThrough;
		initOptions.Color = glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);
		initOptions.Sprite = mPassThroughTexture;
		initOptions.Duration = 10.0f;
		powerUp.Init(initOptions);
		mSpawnedPowerups.push_back(std::move(powerUp));
		return;
	}
	if (ShouldSpawn(1.0f / 75.0f))
	{
		initOptions.Type = PowerUp::PowerUpType::PadSizeIncrease;
		initOptions.Color = glm::vec4(1.0f, 0.6f, 0.4, 1.0f);
		initOptions.Sprite = mPadSizeIncreaseTexture;
		initOptions.Duration = 0.0f;
		powerUp.Init(initOptions);
		mSpawnedPowerups.push_back(std::move(powerUp));
		return;
	}

	if (ShouldSpawn(1.0f / 15.0f))
	{
		initOptions.Type = PowerUp::PowerUpType::Confuse;
		initOptions.Color = glm::vec4(1.0f, 0.3f, 0.3f, 1.0f);
		initOptions.Sprite = mConfuseTexture;
		initOptions.Duration = 15.0f;
		powerUp.Init(initOptions);
		mSpawnedPowerups.push_back(std::move(powerUp));
		return;
	}
	if (ShouldSpawn(1.0f / 15.0f))
	{
		initOptions.Type = PowerUp::PowerUpType::Chaos;
		initOptions.Color = glm::vec4(0.9f, 0.25f, 0.25f, 1.0f);
		initOptions.Sprite = mChaosTexture;
		initOptions.Duration = 15.0f;
		powerUp.Init(initOptions);
		mSpawnedPowerups.push_back(std::move(powerUp));
		return;
	}
}

void PowerUpSpawner::RemoveUnusedPowerups()
{
	mSpawnedPowerups.erase(
		std::remove_if(
			mSpawnedPowerups.begin(),
			mSpawnedPowerups.end(),
			[](const PowerUp& powerUp) { return powerUp.IsDestroyed() && !powerUp.IsActivated(); }
		),
		mSpawnedPowerups.end()
	);
}

bool PowerUpSpawner::ShouldSpawn(float chance)
{
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	float randomValue = dist(mRNG);
	return randomValue <= chance;
}
