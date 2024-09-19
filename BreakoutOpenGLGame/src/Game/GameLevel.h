#pragma once

#include <memory>
#include "Game/GameObject.h"

class Texture2D;

class GameLevel
{
public:
	struct InitOptions
	{
		unsigned int NumRows, NumColums;
		unsigned int LevelWidth, LevelHeight;
		unsigned int* TileData;
		Texture2D* blockTexture;
		Texture2D* solidBlockTexture;
	};

	GameLevel() = default;
	GameLevel(GameLevel&& other) noexcept;

	void Init(const InitOptions& options);
	bool IsCompleted() const;

	inline unsigned int GetNumBricks() const { return mNumBricks; }
	inline GameObject* GetBricks() const { return mBricks.get(); }

private:
	std::unique_ptr<GameObject[]> mBricks;
	unsigned int mNumBricks;
};
