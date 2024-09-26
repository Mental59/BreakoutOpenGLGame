#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <memory>
#include "External/stb_image.h"
#include "ResourceManager.h"
#include "Shader/ShaderProgram.h";
#include "Shader/Shader.h";
#include "Texture/Texture2D.h"
#include "Game/GameLevel.h"


ShaderProgram* ResourceManager::GetShader(unsigned int index)
{
	return &mShaders[index];
}

Texture2D* ResourceManager::GetTexture2D(unsigned int index)
{
	return &mTextures[index];
}

GameLevel* ResourceManager::GetLevel(unsigned int index)
{
	return &mLevels[index];
}

unsigned int ResourceManager::LoadShader(const LoadShaderOptions& options)
{
	ShaderProgram shader;
	InitShaderProgram(&shader, options);

	unsigned int index = mShaders.size();
	mShaders.push_back(std::move(shader));

	return index;
}

unsigned int ResourceManager::LoadTexture2D(const LoadTextureOptions& options)
{
	Texture2D texture;
	InitTexture2D(&texture, options);

	unsigned int index = mTextures.size();
	mTextures.push_back(std::move(texture));

	return index;
}

unsigned int ResourceManager::LoadLevel(const LoadLevelOptions& options)
{
	GameLevel level;
	InitLevel(&level, options);

	unsigned int index = mLevels.size();
	mLevels.push_back(std::move(level));

	return index;
}

void ResourceManager::InitShaderProgram(ShaderProgram* shaderProgram, const LoadShaderOptions& options)
{
	Shader vertexShader(GL_VERTEX_SHADER);
	Shader fragmentShader(GL_FRAGMENT_SHADER);
	Shader geometryShader(GL_GEOMETRY_SHADER);

	CompileShaderFromFile(vertexShader, options.VertexShaderPath);
	CompileShaderFromFile(fragmentShader, options.FragmentShaderPath);
	CompileShaderFromFile(geometryShader, options.GeometryShaderPath);

	shaderProgram->Init(vertexShader, fragmentShader, geometryShader);
}

void ResourceManager::CompileShaderFromFile(Shader& shader, const char* filePath)
{
	if (!filePath)
	{
		return;
	}

	std::ifstream stream(filePath);
	std::stringstream sourceStream;
	sourceStream << stream.rdbuf();
	shader.Compile(sourceStream.str().c_str());
}

void ResourceManager::InitTexture2D(Texture2D* texture, const LoadTextureOptions& options)
{
	int width, height, numChannels;
	unsigned char* data = stbi_load(options.Path, &width, &height, &numChannels, 0);

	if (numChannels == 4)
	{
		texture->SetInternalFormat(GL_RGBA8);
		texture->SetImageFormat(GL_RGBA);
	}
	else
	{
		texture->SetInternalFormat(GL_RGB8);
		texture->SetImageFormat(GL_RGB);
	}

	texture->Init(width, height, numChannels, data);

	stbi_image_free(data);
}

void ResourceManager::InitLevel(GameLevel* level, const LoadLevelOptions& options)
{
	std::ifstream stream(options.Path);

	unsigned int nRows, nColumns;
	stream >> nRows >> nColumns;

	unsigned int numElemts = nRows * nColumns;
	auto tileData = std::make_unique<unsigned int[]>(numElemts);
	for (unsigned int i = 0; i < numElemts; i++)
	{
		stream >> tileData[i];
	}

	GameLevel::InitOptions gameLevelInitOptions;
	gameLevelInitOptions.NumRows = nRows;
	gameLevelInitOptions.NumColumns = nColumns;
	gameLevelInitOptions.LevelWidth = options.LevelWidth;
	gameLevelInitOptions.LevelHeight = options.LevelHeight;
	gameLevelInitOptions.TileData = tileData.get();
	gameLevelInitOptions.blockTexture = options.blockTexture;
	gameLevelInitOptions.solidBlockTexture = options.solidBlockTexture;
	level->Init(gameLevelInitOptions);
}
