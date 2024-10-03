#pragma once

#include "Manager/ResourceManager.h"
#include "Renderer/SpriteRenderer.h"
#include "Game/GameObject.h"
#include "Game/BallGameObject.h"
#include "Manager/RenderManager.h"
#include "Game/PowerUpSpawner.h"
#include "Game/PowerUp.h"
#include "Renderer/TextRenderer.h"

static int constexpr NUM_KEYS = 1024;

enum GameState : unsigned char
{
	GAME_ACTIVE = 0,
	GAME_MENU,
	GAME_WIN
};

namespace irrklang
{
	class ISoundEngine;
}

class Game
{
public:
	Game(int width, int height);
	Game(Game& other) = delete;
	Game& operator=(const Game& other) = delete;

	void SetKey(int key, bool value);

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void Resize(int width, int height);
private:
	void InitResources();
	void InitPaddle();
	void InitBall();
	void InitPowerUpSpawner();
	void InitAudio();

	void PlaySoundWithVolume(const char* path, float volume, bool loop);

	void CheckCollisions();
	void CheckCollisionsWithBricks();
	void CheckCollisionWithPaddle();
	void CheckCollisionsWithPowerUps();

	void ResetCurrentLevel(bool resetLevel);

	void ActivatePowerUp(const PowerUp& powerUp);
	void UpdatePowerUps(float dt);

	void ActivateSpeedPowerUp();
	void ActivateStickyBallPowerUp();
	void ActivatePassThroughPowerUp();
	void ActiatePadSizeIncreasePowerUp();
	void ActivateConfusePowerUp();
	void ActivateChaosPowerUp();

	void DisableStickyBallPowerUp();
	void DisablePassThroughPowerUp();
	void DisableConfusePowerUp();
	void DisableChaosPowerUp();

	ResourceManager mResourceManager;
	RenderManager mRenderManager;

	PowerUpSpawner mPowerUpSpawner;

	SpriteRenderer mSpriteRenderer;
	TextRenderer mTextRenderer;

	GameObject mPaddle;
	BallGameObject mBall;

	int mWidth, mHeight;
	bool mKeys[NUM_KEYS];
	int mLevelIndex;
	float mShakeTime;

	irrklang::ISoundEngine* mSoundEngine;

	GameState mState;

	unsigned int mLives;
};
