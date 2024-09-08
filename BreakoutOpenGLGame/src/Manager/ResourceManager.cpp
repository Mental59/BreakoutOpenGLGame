#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <memory>
#include "ResourceManager.h"
#include "Shader/ShaderProgram.h";
#include "Shader/Shader.h";
#include "Texture/Texture2D.h"
#include "External/stb_image.h"

ResourceManager::ResourceManager() : mShaders(), mTextures()
{

}

std::weak_ptr<ShaderProgram> ResourceManager::GetShader(unsigned int index)
{
	return std::weak_ptr<ShaderProgram>(mShaders[index]);
}

std::weak_ptr<Texture2D> ResourceManager::GetTexture2D(unsigned int index)
{
	return std::weak_ptr<Texture2D>(mTextures[index]);
}

unsigned int ResourceManager::LoadShader(LoadShaderOptions& options)
{
	std::shared_ptr<ShaderProgram> shaderPtr = std::make_shared<ShaderProgram>();
	InitShaderProgram(shaderPtr.get(), options);
	mShaders.push_back(std::move(shaderPtr));

	unsigned int index = mShaders.size() - 1;
	options.index = index;

	return index;
}

unsigned int ResourceManager::LoadTexture2D(LoadTextureOptions& options)
{
	std::shared_ptr<Texture2D> texturePtr = std::make_shared<Texture2D>();
	InitTexture2D(texturePtr.get(), options);
	mTextures.push_back(std::move(texturePtr));

	unsigned int index = mTextures.size() - 1;
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
