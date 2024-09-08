#pragma once

#include <vector>
#include <memory>

class ShaderProgram;
class Shader;
class Texture2D;

class ResourceManager
{
public:
	struct LoadShaderOptions
	{
		const char* vertexShaderPath;
		const char* fragmentShaderPath;
		const char* geometryShaderPath;
		unsigned int index;
	};

	struct LoadTextureOptions
	{
		const char* path;
		unsigned int index;
	};

	ResourceManager();

	std::weak_ptr<ShaderProgram> GetShader(unsigned int index);
	std::weak_ptr<Texture2D> GetTexture2D(unsigned int index);

	unsigned int LoadShader(LoadShaderOptions& options);

	unsigned int LoadTexture2D(LoadTextureOptions& options);
private:
	void InitShaderProgram(ShaderProgram* shaderProgram, const LoadShaderOptions& options);
	void CompileShaderFromFile(Shader& shader, const char* filePath);

	void InitTexture2D(Texture2D* texture, const LoadTextureOptions& options);

	std::vector<std::shared_ptr<ShaderProgram>> mShaders;
	std::vector<std::shared_ptr<Texture2D>> mTextures;
};
