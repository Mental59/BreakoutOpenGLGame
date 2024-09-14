#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <memory>
#include "ResourceManager.h"
#include "Shader/ShaderProgram.h";
#include "Shader/Shader.h";
#include "Texture/Texture2D.h"
#include "External/stb_image.h"

void ResourceManager::Init(const InitializationOptions& options)
{
	mShaders = std::make_unique<ShaderProgram[]>(options.numShaders);
	mTextures = std::make_unique<Texture2D[]>(options.numTextures);
}

ShaderProgram* ResourceManager::GetShader(unsigned int index)
{
	return &mShaders[index];
}

Texture2D* ResourceManager::GetTexture2D(unsigned int index)
{
	return &mTextures[index];
}

unsigned int ResourceManager::LoadShader(LoadShaderOptions& options)
{
	unsigned int index = numLoadedShaders++;
	InitShaderProgram(&mShaders[index], options);
	options.index = index;
	return index;
}

unsigned int ResourceManager::LoadTexture2D(LoadTextureOptions& options)
{
	unsigned int index = numLoadedTextures++;
	InitTexture2D(&mTextures[index], options);
	options.index = index;
	return index;
}

void ResourceManager::InitShaderProgram(ShaderProgram* shaderProgram, const LoadShaderOptions& options)
{
	Shader vertexShader(GL_VERTEX_SHADER);
	Shader fragmentShader(GL_FRAGMENT_SHADER);
	Shader geometryShader(GL_GEOMETRY_SHADER);

	CompileShaderFromFile(vertexShader, options.vertexShaderPath);
	CompileShaderFromFile(fragmentShader, options.fragmentShaderPath);
	CompileShaderFromFile(geometryShader, options.geometryShaderPath);

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
	unsigned char* data = stbi_load(options.path, &width, &height, &numChannels, 0);

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
