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

std::weak_ptr<ShaderProgram> ResourceManager::GetShader(const std::string& name)
{
	return std::weak_ptr<ShaderProgram>(mShaders[name]);
}

std::weak_ptr<Texture2D> ResourceManager::GetTexture2D(const std::string& name)
{
	return std::weak_ptr<Texture2D>(mTextures[name]);
}

std::weak_ptr<ShaderProgram> ResourceManager::LoadShader(const LoadShaderOptions& options)
{
	ShaderProgram* shader = LoadShaderFromFile(options);

	std::shared_ptr<ShaderProgram> shaderPtr(shader);
	mShaders[options.shaderName] = std::move(shaderPtr);

	return std::weak_ptr<ShaderProgram>(mShaders[options.shaderName]);
}

std::weak_ptr<Texture2D> ResourceManager::LoadTexture2D(const LoadTextureOptions& options)
{
	Texture2D* texture = LoadTexture2DFromFile(options);

	std::shared_ptr<Texture2D> texturePtr(texture);
	mTextures[options.textureName] = std::move(texturePtr);

	return std::weak_ptr<Texture2D>(mTextures[options.textureName]);
}

ShaderProgram* ResourceManager::LoadShaderFromFile(const LoadShaderOptions& options)
{
	ShaderProgram* shaderProgram = new ShaderProgram();

	Shader vertexShader(GL_VERTEX_SHADER);
	Shader fragmentShader(GL_FRAGMENT_SHADER);
	Shader geometryShader(GL_GEOMETRY_SHADER);

	CompileShaderFromFile(vertexShader, options.vertexShaderPath);
	CompileShaderFromFile(fragmentShader, options.fragmentShaderPath);
	CompileShaderFromFile(geometryShader, options.geometryShaderPath);

	shaderProgram->Init(vertexShader, fragmentShader, geometryShader);

	return shaderProgram;
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

Texture2D* ResourceManager::LoadTexture2DFromFile(const LoadTextureOptions& options)
{
	Texture2D* texture = new Texture2D();

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

	return texture;
}
