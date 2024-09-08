#pragma once

#include <vector>
#include <memory>
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"

class Shader;

class ResourceManager
{
public:
	struct LoadShaderOptions
	{
		const char* vertexShaderPath = nullptr;
		const char* fragmentShaderPath = nullptr;
		const char* geometryShaderPath = nullptr;
		unsigned int index = 0;
	};

	struct LoadTextureOptions
	{
		const char* path = nullptr;
		unsigned int index = 0;
	};

	struct InitializationOptions
	{
		unsigned int numShaders = 0;
		unsigned int numTextures = 0;
	};

	ResourceManager(const InitializationOptions& options);

	ShaderProgram* GetShader(unsigned int index);
	Texture2D* GetTexture2D(unsigned int index);

	unsigned int LoadShader(LoadShaderOptions& options);

	unsigned int LoadTexture2D(LoadTextureOptions& options);
private:
	void InitShaderProgram(ShaderProgram* shaderProgram, const LoadShaderOptions& options);
	void CompileShaderFromFile(Shader& shader, const char* filePath);

	void InitTexture2D(Texture2D* texture, const LoadTextureOptions& options);

	std::unique_ptr<ShaderProgram[]> mShaders;
	std::unique_ptr<Texture2D[]> mTextures;

	unsigned int numLoadedShaders = 0;
	unsigned int numLoadedTextures = 0;
};
