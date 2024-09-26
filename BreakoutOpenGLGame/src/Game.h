#pragma once

#include "Manager/ResourceManager.h"
#include "Renderer/SpriteRenderer.h"
#include "Game/GameObject.h"
#include "Game/BallGameObject.h"
#include "Manager/RenderManager.h"

static int constexpr NUM_KEYS = 1024;

enum GameState : unsigned char
{
	GAME_ACTIVE = 0,
	GAME_MENU,
	GAME_WIN
};

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

	void CheckCollisions();

private:
	void InitResources();
	void InitPlayer();
	void InitBall();

	void ResetCurrentLevel();

	ResourceManager mResourceManager;
	RenderManager mRenderManager;

	SpriteRenderer mSpriteRenderer;
	GameObject mPlayer;
	BallGameObject mBall;

	int mWidth, mHeight;
	bool mKeys[NUM_KEYS];
	unsigned int mLevelIndex;
	float mShakeTime;

	GameState mState;
};
