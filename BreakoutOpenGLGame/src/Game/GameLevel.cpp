#include <memory>
#include <glm/glm.hpp>
#include "GameLevel.h"
#include "Game/BrickGameObject.h"

GameLevel::GameLevel(GameLevel&& other) noexcept
{
	mNumBricks = other.mNumBricks;
	mNumRows = other.mNumRows;
	mNumColumns = other.mNumColumns;
	mUnitWidth = other.mUnitWidth;
	mUnitHeight = other.mUnitHeight;
	mBricks = std::move(other.mBricks);
}

void GameLevel::Init(const InitOptions& options)
{
	mNumRows = options.NumRows;
	mNumColumns = options.NumColumns;

	unsigned int numBlocks = 0;
	for (unsigned int i = 0; i < options.NumRows * options.NumColumns; i++)
	{
		numBlocks += options.TileData[i] > 0;
	}

	mBricks.reset();
	mBricks = std::make_unique<BrickGameObject[]>(numBlocks);
	mNumBricks = numBlocks;

	float unitWidth = GetUnitWidth(options.LevelWidth);
	float unitHeight = GetUnitHeight(options.LevelHeight);
	mUnitWidth = unitWidth;
	mUnitHeight = unitHeight;

	unsigned int blockIndex = 0;
	for (unsigned int y = 0; y < options.NumRows; y++)
	{
		for (unsigned int x = 0; x < options.NumColumns; x++)
		{
			unsigned int index = options.NumColumns * y + x;
			unsigned int tileCode = options.TileData[index];

			if (tileCode <= 0)
			{
				continue;
			}

			BrickGameObject::BrickInitOptins initOptions;
			initOptions.Position = glm::vec2(unitWidth * x, unitHeight * y);
			initOptions.Size = glm::vec2(unitWidth, unitHeight);
			initOptions.Sprite = options.blockTexture;

			if (tileCode == 1) // solid brick
			{
				initOptions.IsSolid = true;
				initOptions.Sprite = options.solidBlockTexture;
				initOptions.Color = glm::vec4(0.8f, 0.8f, 0.7f, 1.0f);
			}
			else if (tileCode == 2)
			{
				initOptions.Color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
			}
			else if (tileCode == 3)
			{
				initOptions.Color = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
			}
			else if (tileCode == 4)
			{
				initOptions.Color = glm::vec4(0.8f, 0.8f, 0.4f, 1.0f);
			}
			else if (tileCode == 5)
			{
				initOptions.Color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
			}

			mBricks[blockIndex++].Init(initOptions);
		}
	}
}

bool GameLevel::IsCompleted() const
{
	for (unsigned int i = 0; i < mNumBricks; i++)
	{
		if (!mBricks[i].IsSolid() && !mBricks[i].IsDestroyed())
		{
			return false;
		}
	}
	return true;
}

void GameLevel::Reset()
{
	for (unsigned int i = 0; i < mNumBricks; i++)
	{
		mBricks[i].Reset();
	}
}

void GameLevel::Resize(unsigned int levelWidth, unsigned int levelHeight)
{
	float unitWidth = GetUnitWidth(levelWidth);
	float unitHeight = GetUnitHeight(levelHeight);

	for (unsigned int i = 0; i < mNumBricks; i++)
	{
		glm::vec2 position = mBricks[i].GetPosition();

		position.x *= unitWidth / mUnitWidth;
		position.y *= unitHeight / mUnitHeight;

		mBricks[i].SetPosition(position);
		mBricks[i].SetSize(glm::vec2(unitWidth, unitHeight));
	}

	mUnitWidth = unitWidth;
	mUnitHeight = unitHeight;
}

float GameLevel::GetUnitWidth(unsigned int levelWidth) const
{
	return static_cast<float>(levelWidth) / static_cast<float>(mNumColumns);
}

float GameLevel::GetUnitHeight(unsigned int levelHeight) const
{
	return static_cast<float>(levelHeight) / static_cast<float>(mNumRows);
}
