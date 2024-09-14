#pragma once

#include "Manager/ResourceManager.h"
#include "Renderer/SpriteRenderer.h"

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

	void SetKey(int key, bool value);

	Game(Game& other) = delete;
	Game& operator=(const Game& other) = delete;

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	~Game();

private:
	GameState mState;
	ResourceManager mResourceManager;
	SpriteRenderer mSpriteRenderer;
	bool mKeys[NUM_KEYS];
	int mWidth, mHeight;
};
