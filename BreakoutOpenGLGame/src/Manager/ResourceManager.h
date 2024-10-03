#pragma once

#include <vector>
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Game/GameLevel.h"

class Shader;

class ResourceManager
{
public:
	struct LoadShaderOptions
	{
		const char* VertexShaderPath = nullptr;
		const char* FragmentShaderPath = nullptr;
		const char* GeometryShaderPath = nullptr;
	};

	struct LoadTextureOptions
	{
		const char* Path = nullptr;
	};

	struct LoadLevelOptions
	{
		const char* Path = nullptr;
		unsigned int LevelWidth = 0;
		unsigned int LevelHeight = 0;
		Texture2D* blockTexture = nullptr;
		Texture2D* solidBlockTexture = nullptr;
	};

	ResourceManager() = default;

	ShaderProgram* GetShader(unsigned int index);
	Texture2D* GetTexture2D(unsigned int index);
	GameLevel* GetLevel(unsigned int index);

	unsigned int LoadShader(const LoadShaderOptions& options);
	unsigned int LoadTexture2D(const LoadTextureOptions& options);
	unsigned int LoadLevel(const LoadLevelOptions& options);

	unsigned int GetNumLevels() const { return mLevels.size(); }
private:
	void InitShaderProgram(ShaderProgram* shaderProgram, const LoadShaderOptions& options);
	void CompileShaderFromFile(Shader& shader, const char* filePath);

	void InitTexture2D(Texture2D* texture, const LoadTextureOptions& options);
	void InitLevel(GameLevel* level, const LoadLevelOptions& options);

	std::vector<ShaderProgram> mShaders;
	std::vector<Texture2D> mTextures;
	std::vector<GameLevel> mLevels;
};
