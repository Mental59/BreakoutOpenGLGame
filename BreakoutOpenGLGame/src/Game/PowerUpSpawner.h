#pragma once

#include <vector>
#include <random>
#include <glm/glm.hpp>
#include "Game/PowerUp.h"

class Texture2D;

class PowerUpSpawner
{
public:
	struct PowerUpSwawnerOptions
	{
		Texture2D* SpeedTexture;
		Texture2D* StickyTexture;
		Texture2D* PassThroughTexture;
		Texture2D* PadSizeIncreaseTexture;
		Texture2D* ConfuseTexture;
		Texture2D* ChaosTexture;
	};

	PowerUpSpawner();

	void Init(const PowerUpSwawnerOptions& options);
	void SpawnAt(glm::vec2 position);
	void RemoveUnusedPowerups();
	void Clear();

	inline std::vector<PowerUp>& GetSpawnedPowerUps() { return mSpawnedPowerups; }

private:
	bool ShouldSpawn(float chance);

	std::mt19937 mRNG;
	std::vector<PowerUp> mSpawnedPowerups;

	Texture2D* mSpeedTexture;
	Texture2D* mStickyTexture;
	Texture2D* mPassThroughTexture;
	Texture2D* mPadSizeIncreaseTexture;
	Texture2D* mConfuseTexture;
	Texture2D* mChaosTexture;
};
