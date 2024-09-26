#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glfw3.h>
#include "Game.h"
#include "Manager/ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Game/GameLevel.h"
#include "Game/GameObject.h"
#include "Game/BallGameObject.h"

static unsigned int SPRITE_SHADER_INDEX;
static unsigned int POST_PROCESSING_SHADER_INDEX;
static unsigned int TEXTURE_AWESOMEFACE_INDEX;
static unsigned int TEXTURE_BLOCK_INDEX;
static unsigned int TEXTURE_BLOCK_SOLID_INDEX;
static unsigned int TEXTURE_BACKGROUND_INDEX;
static unsigned int TEXTURE_PADDLE_INDEX;
static unsigned int TEXTURE_PARTICLE_INDEX;

Game::Game(int width, int height) :
	mWidth(width),
	mHeight(height),
	mKeys(),
	mState(GAME_ACTIVE),
	mResourceManager(),
	mSpriteRenderer(),
	mLevelIndex(1),
	mShakeTime(0.0f),
	mPlayer()
{

}

void Game::SetKey(int key, bool value)
{
	if (key >= 0 && key < NUM_KEYS)
	{
		mKeys[key] = value;
	}
}

void Game::Init()
{
	mSpriteRenderer.Init();
	InitResources();
	InitPlayer();
	InitBall();
	mRenderManager.Init(mResourceManager.GetShader(POST_PROCESSING_SHADER_INDEX), mWidth, mHeight);
}

void Game::ProcessInput(float dt)
{
	if (mState == GAME_ACTIVE)
	{
		glm::vec2 playerVelocity = mPlayer.GetVelocity();
		glm::vec2 playerPos = mPlayer.GetPosition();
		glm::vec2 playerSize = mPlayer.GetSize();

		if (mKeys[GLFW_KEY_A] && playerPos.x > 0.0f)
		{
			float displacementX = -playerVelocity.x * dt;

			playerPos.x += displacementX;
			mPlayer.SetPosition(playerPos);

			mBall.FollowPaddle(displacementX);
		}
		if (mKeys[GLFW_KEY_D] && playerPos.x < mWidth - playerSize.x)
		{
			float displacementX = playerVelocity.x * dt;

			playerPos.x += displacementX;
			mPlayer.SetPosition(playerPos);

			mBall.FollowPaddle(displacementX);
		}
		if (mKeys[GLFW_KEY_SPACE])
		{
			mBall.Activate();
		}
	}
}

void Game::Update(float dt)
{
	mBall.Move(dt, static_cast<float>(mWidth));
	mBall.Update(dt);
	CheckCollisions();
	if (mBall.GetPosition().y >= mHeight)
	{
		ResetCurrentLevel();
	}

	if (mShakeTime <= 0.0f)
	{
		mRenderManager.SetShake(false);
	}
	else
	{
		mShakeTime -= dt;
	}
}

void Game::Render()
{
	if (mState == GAME_ACTIVE)
	{
		ShaderProgram* spriteShader = mResourceManager.GetShader(SPRITE_SHADER_INDEX);
		Texture2D* awesomeFaceTexture = mResourceManager.GetTexture2D(TEXTURE_AWESOMEFACE_INDEX);
		Texture2D* backgroundTexture = mResourceManager.GetTexture2D(TEXTURE_BACKGROUND_INDEX);
		GameLevel* currentLevel = mResourceManager.GetLevel(mLevelIndex);

		mRenderManager.Begin();

		mSpriteRenderer.Draw(spriteShader, backgroundTexture, glm::vec2(0.0f), glm::vec2(mWidth, mHeight), 0.0f, glm::vec4(1.0f));
		mSpriteRenderer.DrawGameLevel(spriteShader, currentLevel);
		mSpriteRenderer.DrawGameObject(spriteShader, &mPlayer);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		mSpriteRenderer.DrawParticles(spriteShader, mBall.GetParticleEmitter());
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mSpriteRenderer.DrawGameObject(spriteShader, &mBall);

		mRenderManager.End(glfwGetTime());
	}
}

void Game::CheckCollisions()
{
	if (!mBall.IsActive())
	{
		return;
	}

	GameLevel* currentLevel = mResourceManager.GetLevel(mLevelIndex);
	BrickGameObject* bricks = currentLevel->GetBricks();
	for (unsigned int i = 0; i < currentLevel->GetNumBricks(); i++)
	{
		if (bricks[i].IsDestroyed())
		{
			continue;
		}

		BallGameObject::BallHitResult hitResult = mBall.Collides(bricks + i);

		if (hitResult.Collided)
		{
			bricks[i].Destroy();

			glm::vec2 penetrationDistance = mBall.GetRadius() - glm::abs(hitResult.HitPoint - mBall.GetCenter());
			glm::vec2 ballPosition = mBall.GetPosition();
			glm::vec2 ballVelosity = mBall.GetVelocity();

			switch (hitResult.BrickSideDirection)
			{
			case VectorDirection::Up:
				ballVelosity.y = -ballVelosity.y;
				ballPosition.y -= penetrationDistance.y;
				break;
			case VectorDirection::Right:
				ballVelosity.x = -ballVelosity.x;
				ballPosition.x -= penetrationDistance.x;
				break;
			case VectorDirection::Down:
				ballVelosity.y = -ballVelosity.y;
				ballPosition.y += penetrationDistance.y;
				break;
			case VectorDirection::Left:
				ballVelosity.x = -ballVelosity.x;
				ballPosition.x += penetrationDistance.x;
				break;
			}

			mBall.SetPosition(ballPosition);
			mBall.SetVelocity(ballVelosity);

			if (bricks[i].IsSolid())
			{
				mShakeTime = 0.075f;
				mRenderManager.SetShake(true);
			}
		}
	}

	BallGameObject::BallHitResult hitWithPaddle = mBall.Collides(&mPlayer);
	if (hitWithPaddle.Collided)
	{
		glm::vec2 ballCenter = mBall.GetCenter();
		glm::vec2 paddleCenter = mPlayer.GetPosition() + mPlayer.GetSize() / 2.0f;
		glm::vec2 paddleToBall = ballCenter - paddleCenter;

		float percentage = paddleToBall.x / (mPlayer.GetSize().x / 2.0f);

		glm::vec2 newBallVelocity = mBall.GetVelocity();
		newBallVelocity.x = mBall.GetInitialVelocity().x * percentage * 2.0f;
		newBallVelocity.y = -std::abs(newBallVelocity.y);
		newBallVelocity = glm::normalize(newBallVelocity) * glm::length(mBall.GetVelocity());

		mBall.SetVelocity(newBallVelocity);
	}
}

void Game::InitResources()
{
	ResourceManager::LoadShaderOptions loadShaderOptions;
	loadShaderOptions.VertexShaderPath = "resources/shaders/sprite.vert";
	loadShaderOptions.FragmentShaderPath = "resources/shaders/sprite.frag";
	SPRITE_SHADER_INDEX = mResourceManager.LoadShader(loadShaderOptions);

	loadShaderOptions.VertexShaderPath = "resources/shaders/postprocessing.vert";
	loadShaderOptions.FragmentShaderPath = "resources/shaders/postprocessing.frag";
	POST_PROCESSING_SHADER_INDEX = mResourceManager.LoadShader(loadShaderOptions);

	ResourceManager::LoadTextureOptions loadTextureOptions;
	loadTextureOptions.Path = "resources/textures/awesomeface.png";
	TEXTURE_AWESOMEFACE_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/block.png";
	TEXTURE_BLOCK_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/block_solid.png";
	TEXTURE_BLOCK_SOLID_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/background.jpg";
	TEXTURE_BACKGROUND_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/paddle.png";
	TEXTURE_PADDLE_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/particle.png";
	TEXTURE_PARTICLE_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);

	auto projectionMat = glm::ortho(0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight), 0.0f, -1.0f, 1.0f);
	ShaderProgram* spriteShader = mResourceManager.GetShader(SPRITE_SHADER_INDEX);
	spriteShader->Bind();
	spriteShader->SetInt("uImage", 0);
	spriteShader->SetMat4("uProjection", glm::value_ptr(projectionMat));

	ResourceManager::LoadLevelOptions loadLevelOptions;
	loadLevelOptions.LevelWidth = mWidth;
	loadLevelOptions.LevelHeight = mHeight;
	loadLevelOptions.blockTexture = mResourceManager.GetTexture2D(TEXTURE_BLOCK_INDEX);
	loadLevelOptions.solidBlockTexture = mResourceManager.GetTexture2D(TEXTURE_BLOCK_SOLID_INDEX);
	loadLevelOptions.Path = "resources/levels/beginner.lvl";
	mResourceManager.LoadLevel(loadLevelOptions);
	loadLevelOptions.Path = "resources/levels/standard.lvl";
	mResourceManager.LoadLevel(loadLevelOptions);
	loadLevelOptions.Path = "resources/levels/few_small_gaps.lvl";
	mResourceManager.LoadLevel(loadLevelOptions);
	loadLevelOptions.Path = "resources/levels/space_invader.lvl";
	mResourceManager.LoadLevel(loadLevelOptions);
	loadLevelOptions.Path = "resources/levels/bounce_galore.lvl";
	mResourceManager.LoadLevel(loadLevelOptions);
}

void Game::InitPlayer()
{
	const glm::vec2 playerSize(100.0f, 20.0f);
	const glm::vec2 playerPos(
		static_cast<float>(mWidth) / 2.0f - playerSize.x / 2.0f,
		static_cast<float>(mHeight) - playerSize.y - 10.0f
	);

	GameObject::GameObjectInitOptions initOptions;
	initOptions.Position = playerPos;
	initOptions.Size = playerSize;
	initOptions.Sprite = mResourceManager.GetTexture2D(TEXTURE_PADDLE_INDEX);
	initOptions.Velocity = glm::vec2(500.0f, 0.0f);
	mPlayer.Init(initOptions);
}

void Game::InitBall()
{
	const float radius = 12.5f;

	glm::vec2 playerPos = mPlayer.GetPosition();
	glm::vec2 playerSize = mPlayer.GetSize();

	BallGameObject::BallInitOptions options;
	options.Position = playerPos + glm::vec2(playerSize.x / 2.0f - radius, -radius * 2.0f);
	options.Size = glm::vec2(radius * 2.0f, radius * 2.0f);
	options.Sprite = mResourceManager.GetTexture2D(TEXTURE_AWESOMEFACE_INDEX);
	options.Velocity = glm::vec2(100.0f, -350.0f);
	options.Radius = radius;
	options.ParticleTexture = mResourceManager.GetTexture2D(TEXTURE_PARTICLE_INDEX);

	mBall.Init(options);
}

void Game::ResetCurrentLevel()
{
	InitPlayer();
	InitBall();

	mResourceManager.GetLevel(mLevelIndex)->Reset();
}
