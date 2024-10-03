#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glfw3.h>
#include <irrklang/irrKlang.h>
#include <iostream>
#include <format>
#include "Game.h"
#include "Manager/ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Game/GameLevel.h"
#include "Game/GameObject.h"
#include "Game/BallGameObject.h"
#include "Game/PowerUpSpawner.h"
#include "Game/PowerUp.h"
#include "Renderer/TextRenderer.h"

static unsigned int SHADER_SPRITE_INDEX;
static unsigned int SHADER_POST_PROCESSING_INDEX;
static unsigned int SHADER_TEXT_INDEX;
static unsigned int TEXTURE_AWESOMEFACE_INDEX;
static unsigned int TEXTURE_BLOCK_INDEX;
static unsigned int TEXTURE_BLOCK_SOLID_INDEX;
static unsigned int TEXTURE_BACKGROUND_INDEX;
static unsigned int TEXTURE_PADDLE_INDEX;
static unsigned int TEXTURE_PARTICLE_INDEX;
static unsigned int TEXTURE_SPEED_POWERUP_INDEX;
static unsigned int TEXTURE_STICKY_POWERUP_INDEX;
static unsigned int TEXTURE_PASSTHROUGH_POWERUP_INDEX;
static unsigned int TEXTURE_PADSIZEINCREASE_POWERUP_INDEX;
static unsigned int TEXTURE_CONFUSE_POWERUP_INDEX;
static unsigned int TEXTURE_CHAOS_POWERUP_INDEX;

static glm::vec2 BALL_INITIAL_VELOCITY;
static glm::vec2 PADDLE_INITIAL_SIZE;

static unsigned int MAX_LIVES = 3;

Game::Game(int width, int height) :
	mWidth(width),
	mHeight(height),
	mKeys(),
	mState(GAME_MENU),
	mResourceManager(),
	mSpriteRenderer(),
	mLevelIndex(0),
	mShakeTime(0.0f),
	mPaddle(),
	mPowerUpSpawner(),
	mSoundEngine(nullptr),
	mLives(MAX_LIVES)
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
	InitPaddle(glm::vec2(0.0f), true);
	InitBall();
	mRenderManager.Init(mResourceManager.GetShader(SHADER_POST_PROCESSING_INDEX), mWidth, mHeight);
	InitPowerUpSpawner();
	InitAudio();
	mTextRenderer.Init(mResourceManager.GetShader(SHADER_TEXT_INDEX), "resources/fonts/OCRAEXT.TTF");
}

void Game::ProcessInput(float dt)
{
	static bool keyAPrevious = false;
	static bool keyDPrevious = false;
	static bool keySpacePrevious = false;

	if (mState == GAME_ACTIVE)
	{
		glm::vec2 paddleVelocity = mPaddle.GetVelocity();
		glm::vec2 paddlePos = mPaddle.GetPosition();
		glm::vec2 paddleSize = mPaddle.GetSize();

		if (mKeys[GLFW_KEY_A] && paddlePos.x > 0.0f)
		{
			float displacementX = -paddleVelocity.x * dt;

			paddlePos.x += displacementX;
			mPaddle.SetPosition(paddlePos);

			mBall.FollowPaddle(displacementX);
		}
		if (mKeys[GLFW_KEY_D] && paddlePos.x < mWidth - paddleSize.x)
		{
			float displacementX = paddleVelocity.x * dt;

			paddlePos.x += displacementX;
			mPaddle.SetPosition(paddlePos);

			mBall.FollowPaddle(displacementX);
		}
		if (mKeys[GLFW_KEY_SPACE] && !mBall.IsActive())
		{
			mBall.Activate();
			PlaySoundWithVolume("resources/audio/bleep.wav", 0.6f, false);
		}
	}
	else if (mState == GAME_MENU)
	{
		if (!mKeys[GLFW_KEY_A] && keyAPrevious)
		{
			mLevelIndex = (mLevelIndex + 1) % mResourceManager.GetNumLevels();
		}
		if (!mKeys[GLFW_KEY_D] && keyDPrevious)
		{
			mLevelIndex--;
			if (mLevelIndex < 0)
			{
				mLevelIndex = mResourceManager.GetNumLevels() - 1;
			}
		}
		if (mKeys[GLFW_KEY_SPACE])
		{
			mState = GAME_ACTIVE;
		}
	}
	else
	{
		if (!mKeys[GLFW_KEY_SPACE] && keySpacePrevious)
		{
			ResetCurrentLevel(true);
			mLevelIndex = 0;
		}
	}

	keyAPrevious = mKeys[GLFW_KEY_A];
	keyDPrevious = mKeys[GLFW_KEY_D];
	keySpacePrevious = mKeys[GLFW_KEY_SPACE];
}

void Game::Update(float dt)
{
	if (mState == GAME_ACTIVE)
	{
		mBall.Move(dt, static_cast<float>(mWidth));
		mBall.Update(dt);

		CheckCollisions();

		if (mBall.GetPosition().y >= mHeight)
		{
			if (--mLives == 0)
			{
				ResetCurrentLevel(true);
			}
			else
			{
				ResetCurrentLevel(false);
			}
		}

		if (mShakeTime <= 0.0f)
		{
			mRenderManager.SetShake(false);
		}
		else
		{
			mShakeTime -= dt;
		}

		UpdatePowerUps(dt);

		if (mResourceManager.GetLevel(mLevelIndex)->IsCompleted())
		{
			mState = GAME_WIN;
		}
	}
}

void Game::Render()
{
	ShaderProgram* spriteShader = mResourceManager.GetShader(SHADER_SPRITE_INDEX);
	Texture2D* backgroundTexture = mResourceManager.GetTexture2D(TEXTURE_BACKGROUND_INDEX);
	GameLevel* currentLevel = mResourceManager.GetLevel(mLevelIndex);
	Texture2D* awesomeFaceTexture = mResourceManager.GetTexture2D(TEXTURE_AWESOMEFACE_INDEX);

	mRenderManager.Begin();

	mSpriteRenderer.Draw(spriteShader, backgroundTexture, glm::vec2(0.0f), glm::vec2(mWidth, mHeight), 0.0f, glm::vec4(1.0f));
	mSpriteRenderer.DrawGameLevel(spriteShader, currentLevel);
	mSpriteRenderer.DrawGameObject(spriteShader, &mPaddle);

	for (PowerUp& powerUp : mPowerUpSpawner.GetSpawnedPowerUps())
	{
		if (!powerUp.IsDestroyed())
		{
			mSpriteRenderer.DrawGameObject(spriteShader, &powerUp);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	mSpriteRenderer.DrawParticles(spriteShader, mBall.GetParticleEmitter());
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mSpriteRenderer.DrawGameObject(spriteShader, &mBall);

	mRenderManager.End(glfwGetTime());

	mTextRenderer.Draw(std::format("Lives: {}", mLives), glm::vec2(5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.5f);

	if (mState == GAME_MENU)
	{
		mTextRenderer.Draw("Press SPACE to start", glm::vec2(350.0f, mHeight / 2 + 10.0f), glm::vec3(1.0f), 1.0f);
		mTextRenderer.Draw("Press A or D to select level", glm::vec2(340.0f, mHeight / 2 + 50.0f), glm::vec3(1.0f), 0.75f);
	}

	if (mState == GAME_WIN)
	{
		mTextRenderer.Draw("You won!!!", glm::vec2(500.0f, mHeight / 2 + 10.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
		mTextRenderer.Draw("Press SPACE to retry or ESC to quit", glm::vec2(280.0f, mHeight / 2 + 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.75f);
	}
}

void Game::Resize(int width, int height)
{
	mRenderManager.Resize(width, height);
}

void Game::InitResources()
{
	ResourceManager::LoadShaderOptions loadShaderOptions;
	loadShaderOptions.VertexShaderPath = "resources/shaders/sprite.vert";
	loadShaderOptions.FragmentShaderPath = "resources/shaders/sprite.frag";
	SHADER_SPRITE_INDEX = mResourceManager.LoadShader(loadShaderOptions);

	loadShaderOptions.VertexShaderPath = "resources/shaders/postprocessing.vert";
	loadShaderOptions.FragmentShaderPath = "resources/shaders/postprocessing.frag";
	SHADER_POST_PROCESSING_INDEX = mResourceManager.LoadShader(loadShaderOptions);

	loadShaderOptions.VertexShaderPath = "resources/shaders/text.vert";
	loadShaderOptions.FragmentShaderPath = "resources/shaders/text.frag";
	SHADER_TEXT_INDEX = mResourceManager.LoadShader(loadShaderOptions);

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
	loadTextureOptions.Path = "resources/textures/powerup_chaos.png";
	TEXTURE_CHAOS_POWERUP_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/powerup_increase.png";
	TEXTURE_PADSIZEINCREASE_POWERUP_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/powerup_confuse.png";
	TEXTURE_CONFUSE_POWERUP_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/powerup_passthrough.png";
	TEXTURE_PASSTHROUGH_POWERUP_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/powerup_speed.png";
	TEXTURE_SPEED_POWERUP_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);
	loadTextureOptions.Path = "resources/textures/powerup_sticky.png";
	TEXTURE_STICKY_POWERUP_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);

	auto projectionMat = glm::ortho(0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight), 0.0f, -1.0f, 1.0f);
	ShaderProgram* spriteShader = mResourceManager.GetShader(SHADER_SPRITE_INDEX);
	spriteShader->Bind();
	spriteShader->SetInt("uImage", 0);
	spriteShader->SetMat4("uProjection", glm::value_ptr(projectionMat));

	ShaderProgram* textShader = mResourceManager.GetShader(SHADER_TEXT_INDEX);
	textShader->Bind();
	textShader->SetInt("uTextImage", 0);
	textShader->SetMat4("uProjection", glm::value_ptr(projectionMat));

	ResourceManager::LoadLevelOptions loadLevelOptions;
	loadLevelOptions.LevelWidth = mWidth;
	loadLevelOptions.LevelHeight = mHeight / 2.0f;
	loadLevelOptions.blockTexture = mResourceManager.GetTexture2D(TEXTURE_BLOCK_INDEX);
	loadLevelOptions.solidBlockTexture = mResourceManager.GetTexture2D(TEXTURE_BLOCK_SOLID_INDEX);
	loadLevelOptions.Path = "resources/levels/standard.lvl";
	mResourceManager.LoadLevel(loadLevelOptions);
	loadLevelOptions.Path = "resources/levels/few_small_gaps.lvl";
	mResourceManager.LoadLevel(loadLevelOptions);
	loadLevelOptions.Path = "resources/levels/space_invader.lvl";
	mResourceManager.LoadLevel(loadLevelOptions);
	loadLevelOptions.Path = "resources/levels/bounce_galore.lvl";
	mResourceManager.LoadLevel(loadLevelOptions);
}

void Game::InitPaddle(glm::vec2 pos, bool setInitPos)
{
	GameObject::GameObjectInitOptions initOptions;

	const glm::vec2 paddleSize(100.0f, 20.0f);
	if (setInitPos)
	{
		const glm::vec2 paddlePos(
			static_cast<float>(mWidth) / 2.0f - paddleSize.x / 2.0f,
			static_cast<float>(mHeight) - paddleSize.y - 10.0f
		);
		initOptions.Position = paddlePos;
	}
	else
	{
		initOptions.Position = pos;
	}

	initOptions.Size = paddleSize;
	initOptions.Sprite = mResourceManager.GetTexture2D(TEXTURE_PADDLE_INDEX);
	initOptions.Velocity = glm::vec2(500.0f, 0.0f);
	mPaddle.Init(initOptions);

	PADDLE_INITIAL_SIZE = mPaddle.GetSize();
}

void Game::InitBall()
{
	const float radius = 12.5f;

	glm::vec2 paddlePos = mPaddle.GetPosition();
	glm::vec2 paddleSize = mPaddle.GetSize();

	BallGameObject::BallInitOptions options;
	options.Position = paddlePos + glm::vec2(paddleSize.x / 2.0f - radius, -radius * 2.0f);
	options.Size = glm::vec2(radius * 2.0f, radius * 2.0f);
	options.Sprite = mResourceManager.GetTexture2D(TEXTURE_AWESOMEFACE_INDEX);
	options.Velocity = glm::vec2(100.0f, -350.0f);
	options.Radius = radius;
	options.ParticleTexture = mResourceManager.GetTexture2D(TEXTURE_PARTICLE_INDEX);

	mBall.Init(options);

	BALL_INITIAL_VELOCITY = mBall.GetVelocity();
}

void Game::InitPowerUpSpawner()
{
	PowerUpSpawner::PowerUpSwawnerOptions initOptions;
	initOptions.SpeedTexture = mResourceManager.GetTexture2D(TEXTURE_SPEED_POWERUP_INDEX);
	initOptions.StickyTexture = mResourceManager.GetTexture2D(TEXTURE_STICKY_POWERUP_INDEX);
	initOptions.PassThroughTexture = mResourceManager.GetTexture2D(TEXTURE_PASSTHROUGH_POWERUP_INDEX);
	initOptions.PadSizeIncreaseTexture = mResourceManager.GetTexture2D(TEXTURE_PADSIZEINCREASE_POWERUP_INDEX);
	initOptions.ConfuseTexture = mResourceManager.GetTexture2D(TEXTURE_CONFUSE_POWERUP_INDEX);
	initOptions.ChaosTexture = mResourceManager.GetTexture2D(TEXTURE_CHAOS_POWERUP_INDEX);
	mPowerUpSpawner.Init(initOptions);
}

void Game::InitAudio()
{
	mSoundEngine = irrklang::createIrrKlangDevice();
	if (!mSoundEngine)
	{
		std::cout << "GAME::ERROR Failed to initialize sound engine" << std::endl;
	}
	PlaySoundWithVolume("resources/audio/breakout.mp3", 0.5f, true);
}

void Game::PlaySoundWithVolume(const char* path, float volume, bool loop)
{
	if (!mSoundEngine)
	{
		return;
	}

	if (irrklang::ISound* sound = mSoundEngine->play2D(path, loop, false, true))
	{
		sound->setVolume(volume);
		sound->drop();
		sound = nullptr;
	}
}

void Game::CheckCollisions()
{
	if (!mBall.IsActive())
	{
		return;
	}

	CheckCollisionsWithBricks();
	CheckCollisionWithPaddle();
	CheckCollisionsWithPowerUps();
}

void Game::CheckCollisionsWithBricks()
{
	GameLevel* currentLevel = mResourceManager.GetLevel(mLevelIndex);
	BrickGameObject* bricks = currentLevel->GetBricks();
	for (unsigned int i = 0; i < currentLevel->GetNumBricks(); i++)
	{
		if (bricks[i].IsDestroyed())
		{
			continue;
		}

		BallGameObject::BallHitResult hitResult = mBall.Collides(bricks[i]);

		if (hitResult.Collided)
		{
			bricks[i].Destroy();

			if (!mBall.IsPassingThrough() || bricks[i].IsSolid())
			{
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
			}

			if (bricks[i].IsSolid())
			{
				mShakeTime = 0.075f;
				mRenderManager.SetShake(true);
				PlaySoundWithVolume("resources/audio/solid.wav", 0.6f, false);
			}
			else
			{
				mPowerUpSpawner.SpawnAt(bricks[i].GetPosition());
				PlaySoundWithVolume("resources/audio/bleep.mp3", 0.6f, false);
			}
		}
	}
}

void Game::CheckCollisionWithPaddle()
{
	BallGameObject::BallHitResult hitWithPaddle = mBall.Collides(mPaddle);
	if (hitWithPaddle.Collided)
	{
		glm::vec2 ballCenter = mBall.GetCenter();
		glm::vec2 paddleCenter = mPaddle.GetPosition() + mPaddle.GetSize() / 2.0f;
		glm::vec2 paddleToBall = ballCenter - paddleCenter;

		float percentage = paddleToBall.x / (mPaddle.GetSize().x / 2.0f);

		glm::vec2 newBallVelocity = mBall.GetVelocity();
		newBallVelocity.x = mBall.GetInitialVelocity().x * percentage * 2.0f;
		newBallVelocity.y = -std::abs(newBallVelocity.y);
		newBallVelocity = glm::normalize(newBallVelocity) * glm::length(mBall.GetVelocity());

		mBall.SetVelocity(newBallVelocity);

		if (mBall.IsSticky())
		{
			mBall.Deactivate();
		}
		else
		{
			PlaySoundWithVolume("resources/audio/bleep.wav", 0.6f, false);
		}
	}
}

void Game::CheckCollisionsWithPowerUps()
{
	for (PowerUp& powerUp : mPowerUpSpawner.GetSpawnedPowerUps())
	{
		if (powerUp.IsDestroyed())
		{
			continue;
		}

		if (powerUp.GetPosition().y >= mHeight)
		{
			powerUp.Destroy();
		}
		else if (mPaddle.Collides(powerUp))
		{
			ActivatePowerUp(powerUp);
			powerUp.Activate();
			powerUp.Destroy();
			PlaySoundWithVolume("resources/audio/powerup.wav", 0.6f, false);
		}
	}
}

void Game::ResetCurrentLevel(bool resetLevel)
{
	if (resetLevel)
	{
		InitPaddle(mPaddle.GetPosition(), true);
		mResourceManager.GetLevel(mLevelIndex)->Reset();
		mState = GAME_MENU;
		mLives = MAX_LIVES;
	}
	else
	{
		InitPaddle(mPaddle.GetPosition(), false);
	}

	InitBall();
	mPowerUpSpawner.Clear();

	DisableStickyBallPowerUp();
	DisablePassThroughPowerUp();
	DisableConfusePowerUp();
	DisableChaosPowerUp();

	mPaddle.SetSize(PADDLE_INITIAL_SIZE);
	mBall.SetVelocity(BALL_INITIAL_VELOCITY);

	if (resetLevel)
	{
		mResourceManager.GetLevel(mLevelIndex)->Reset();
		mState = GAME_MENU;
		mLives = MAX_LIVES;
	}
}

void Game::ActivatePowerUp(const PowerUp& powerUp)
{
	switch (powerUp.GetType())
	{
	case PowerUp::PowerUpType::Speed:
		ActivateSpeedPowerUp();
		break;
	case PowerUp::PowerUpType::Sticky:
		ActivateStickyBallPowerUp();
		break;
	case PowerUp::PowerUpType::PassThrough:
		ActivatePassThroughPowerUp();
		break;
	case PowerUp::PowerUpType::PadSizeIncrease:
		ActiatePadSizeIncreasePowerUp();
		break;
	case PowerUp::PowerUpType::Confuse:
		ActivateConfusePowerUp();
		break;
	case PowerUp::PowerUpType::Chaos:
		ActivateChaosPowerUp();
		break;
	}
}

void Game::UpdatePowerUps(float dt)
{
	mPowerUpSpawner.RemoveUnusedPowerups();

	bool shouldDeactivateSticky = false;
	bool shouldDeactivatePassThrough = false;
	bool shouldDeactivateConfuse = false;
	bool shouldDeactivateChaos = false;

	bool hasActivatedSticky = false;
	bool hasActivatedPassThrough = false;
	bool hasActivatedConfuse = false;
	bool hasActivatedChaos = false;

	std::vector<PowerUp>& powerUps = mPowerUpSpawner.GetSpawnedPowerUps();
	for (PowerUp& powerUp : powerUps)
	{
		if (powerUp.IsDestroyed() && !powerUp.IsActivated())
		{
			continue;
		}

		powerUp.SetPosition(powerUp.GetPosition() + powerUp.GetVelocity() * dt);
		if (powerUp.IsActivated())
		{
			bool isActivated = true;
			if (powerUp.GetDuration() <= 0.0f)
			{
				powerUp.Deactivate();
				isActivated = false;
			}

			switch (powerUp.GetType())
			{
			case PowerUp::PowerUpType::Sticky:
				if (isActivated)
				{
					hasActivatedSticky = true;
				}
				else
				{
					shouldDeactivateSticky = true;
				}
				break;
			case PowerUp::PowerUpType::PassThrough:
				if (isActivated)
				{
					hasActivatedPassThrough = true;
				}
				else
				{
					shouldDeactivatePassThrough = true;
				}
				break;
			case PowerUp::PowerUpType::Confuse:
				if (isActivated)
				{
					hasActivatedConfuse = true;
				}
				else
				{
					shouldDeactivateConfuse = true;
				}
				break;
			case PowerUp::PowerUpType::Chaos:
				if (isActivated)
				{
					hasActivatedChaos = true;
				}
				else
				{
					shouldDeactivateChaos = true;
				}
				break;
			}

			powerUp.DecreaseDuration(dt);
		}
	}

	if (shouldDeactivateSticky && !hasActivatedSticky)
	{
		DisableStickyBallPowerUp();
	}
	if (shouldDeactivatePassThrough && !hasActivatedPassThrough)
	{
		DisablePassThroughPowerUp();
	}
	if (shouldDeactivateConfuse && !hasActivatedConfuse)
	{
		DisableConfusePowerUp();
	}
	if (shouldDeactivateChaos && !hasActivatedChaos)
	{
		DisableChaosPowerUp();
	}
}

void Game::ActivateSpeedPowerUp()
{
	mBall.SetVelocity(mBall.GetVelocity() * 1.2f);
}

void Game::ActivateStickyBallPowerUp()
{
	mBall.SetSticky(true);
	mPaddle.SetColor(glm::vec4(1.0f, 0.5f, 1.0f, 1.0f));
}

void Game::ActivatePassThroughPowerUp()
{
	mBall.SetPassThrough(true);
	mBall.SetColor(glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));
}

void Game::ActiatePadSizeIncreasePowerUp()
{
	mPaddle.SetSize(mPaddle.GetSize() + glm::vec2(50.f, 0.0f));
}

void Game::ActivateConfusePowerUp()
{
	mRenderManager.SetConfuse(true);
}

void Game::ActivateChaosPowerUp()
{
	mRenderManager.SetChaos(true);
}

void Game::DisableStickyBallPowerUp()
{
	mBall.SetSticky(false);
	mPaddle.SetColor(glm::vec4(1.0f));
}

void Game::DisablePassThroughPowerUp()
{
	mBall.SetPassThrough(false);
	mBall.SetColor(glm::vec4(1.0f));
}

void Game::DisableConfusePowerUp()
{
	mRenderManager.SetConfuse(false);
}

void Game::DisableChaosPowerUp()
{
	mRenderManager.SetChaos(false);
}
