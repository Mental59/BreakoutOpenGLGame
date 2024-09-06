#pragma once

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

	void setKey(int key, bool value)
	{
		if (key >= 0 && key < NUM_KEYS)
		{
			mKeys[key] = value;
		}
	}

	Game(Game& other) = delete;
	Game& operator=(const Game& other) = delete;

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	~Game();

private:
	GameState mState;
	bool mKeys[NUM_KEYS];
	int mWidth, mHeight;
};
