#pragma once

#include <unordered_map>
#include <string>
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
		const char* shaderName;
	};

	struct LoadTextureOptions
	{
		const char* path;
		const char* textureName;
	};

	ResourceManager();

	std::weak_ptr<ShaderProgram> GetShader(const std::string& name);
	std::weak_ptr<Texture2D> GetTexture2D(const std::string& name);

	std::weak_ptr<ShaderProgram> LoadShader(const LoadShaderOptions& options);
	std::weak_ptr<Texture2D> LoadTexture2D(const LoadTextureOptions& options);
private:
	ShaderProgram* LoadShaderFromFile(const LoadShaderOptions& options);
	void CompileShaderFromFile(Shader& shader, const char* filePath);

	Texture2D* LoadTexture2DFromFile(const LoadTextureOptions& options);

	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> mShaders;
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> mTextures;
};
