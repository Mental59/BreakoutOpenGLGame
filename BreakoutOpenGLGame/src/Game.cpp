#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Game.h"
#include "Manager/ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"

static unsigned int SPRITE_SHADER_INDEX;
static unsigned int TEXTURE_AWESOMEFACE_INDEX;

Game::Game(int width, int height) :
	mWidth(width),
	mHeight(height),
	mKeys(),
	mState(GAME_ACTIVE),
	mResourceManager(),
	mSpriteRenderer()
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

	ResourceManager::InitializationOptions resourceManagerInitOptions;
	resourceManagerInitOptions.numShaders = 1;
	resourceManagerInitOptions.numTextures = 1;
	mResourceManager.Init(resourceManagerInitOptions);

	ResourceManager::LoadShaderOptions loadShaderOptions;
	loadShaderOptions.vertexShaderPath = "resources/shaders/sprite.vert";
	loadShaderOptions.fragmentShaderPath = "resources/shaders/sprite.frag";
	SPRITE_SHADER_INDEX = mResourceManager.LoadShader(loadShaderOptions);

	ResourceManager::LoadTextureOptions loadTextureOptions;
	loadTextureOptions.path = "resources/textures/awesomeface.png";
	TEXTURE_AWESOMEFACE_INDEX = mResourceManager.LoadTexture2D(loadTextureOptions);

	auto projectionMat = glm::ortho(0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight), 0.0f, -1.0f, 1.0f);
	ShaderProgram* spriteShader = mResourceManager.GetShader(SPRITE_SHADER_INDEX);
	spriteShader->Bind();
	spriteShader->SetInt("uImage", 0);
	spriteShader->SetMat4("uProjection", glm::value_ptr(projectionMat));
}

void Game::ProcessInput(float dt)
{

}

void Game::Update(float dt)
{

}

void Game::Render()
{
	ShaderProgram* spriteShader = mResourceManager.GetShader(SPRITE_SHADER_INDEX);
	Texture2D* awesomeFaceTexture = mResourceManager.GetTexture2D(TEXTURE_AWESOMEFACE_INDEX);

	mSpriteRenderer.Draw(
		spriteShader,
		awesomeFaceTexture,
		glm::vec2(200.0f, 200.0f),
		glm::vec2(300.0f, 300.0f),
		45.0f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
}

Game::~Game()
{
#ifdef _DEBUG
	std::cout << "Game is destroyed" << std::endl;
#endif // _DEBUG
}
