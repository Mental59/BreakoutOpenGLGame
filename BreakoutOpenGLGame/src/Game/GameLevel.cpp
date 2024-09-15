#include <memory>
#include <glm/glm.hpp>
#include "GameLevel.h"
#include "Game/SpriteGameObject.h"

GameLevel::GameLevel(GameLevel&& other) noexcept
{
	mNumBricks = other.mNumBricks;
	mBricks = std::move(other.mBricks);
}

void GameLevel::Init(const InitOptions& options)
{
	unsigned int numBlocks = 0;
	for (unsigned int i = 0; i < options.NumRows * options.NumColums; i++)
	{
		numBlocks += options.TileData[i] > 0;
	}

	mBricks.reset();
	mBricks = std::make_unique<SpriteGameObject[]>(numBlocks);
	mNumBricks = numBlocks;

	float unitWidth = static_cast<float>(options.LevelWidth) / static_cast<float>(options.NumColums);
	float unitHeight = static_cast<float>(options.LevelHeight) / static_cast<float>(options.NumRows);

	unsigned int blockIndex = 0;
	for (unsigned int y = 0; y < options.NumRows; y++)
	{
		for (unsigned int x = 0; x < options.NumColums; x++)
		{
			unsigned int index = options.NumColums * y + x;
			unsigned int tileCode = options.TileData[index];

			if (tileCode <= 0)
			{
				continue;
			}

			SpriteGameObject::InitOptions initOptions;
			initOptions.Position = glm::vec2(unitWidth * x, unitHeight * y);
			initOptions.Size = glm::vec2(unitWidth, unitHeight);
			initOptions.Sprite = options.blockTexture;
			if (tileCode == 1) // solid brick
			{
				initOptions.IsSolid = true;
				initOptions.Sprite = options.solidBlockTexture;
				initOptions.Color = glm::vec3(0.8f, 0.8f, 0.7f);
			}
			else if (tileCode == 2)
			{
				initOptions.Color = glm::vec3(0.2f, 0.6f, 1.0f);
			}
			else if (tileCode == 3)
			{
				initOptions.Color = glm::vec3(0.0f, 0.7f, 0.0f);
			}
			else if (tileCode == 4)
			{
				initOptions.Color = glm::vec3(0.8f, 0.8f, 0.4f);
			}
			else if (tileCode == 5)
			{
				initOptions.Color = glm::vec3(1.0f, 0.5f, 0.0f);
			}

			mBricks[blockIndex++].Init(initOptions);
		}
	}
}

bool GameLevel::IsCompleted() const
{
	for (unsigned int i = 0; i < mNumBricks; i++)
	{
		if (!mBricks[i].IsSolid() && !mBricks[i].isDestroyed())
		{
			return false;
		}
	}
	return true;
}
