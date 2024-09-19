#pragma once

#include "Manager/ResourceManager.h"
#include "Renderer/SpriteRenderer.h"
#include "Game/GameObject.h"

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

private:
	void InitResources();
	void InitPlayer();

	ResourceManager mResourceManager;
	SpriteRenderer mSpriteRenderer;
	GameObject mPlayer;

	int mWidth, mHeight;
	bool mKeys[NUM_KEYS];
	unsigned int mLevelIndex;

	GameState mState;
};
