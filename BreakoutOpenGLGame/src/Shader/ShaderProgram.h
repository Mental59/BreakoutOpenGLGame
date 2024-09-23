#pragma once

#include <string>
#include <unordered_map>

class Shader;

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(ShaderProgram&& other) noexcept;
	~ShaderProgram();

	void Init(const Shader& vertexShader, const Shader& fragmentShader, const Shader& geometryShader);
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetID() const { return mID; }

	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetVec4(const std::string& name, float v0, float v1, float v2, float v3);
	void SetVec4(const std::string& name, const float* data);
	void SetMat4(const std::string& name, const float* data);
	void SetVec3(const std::string& name, float v0, float v1, float v2);
	void SetVec3(const std::string& name, const float* data);
	void SetVec2(const std::string& name, const float* data);

	void SetUniformBlockBinding(const char* uniformBlockName, unsigned int binding) const;

private:
	int GetUniformLocation(const std::string& name);

	unsigned int mID;
	std::unordered_map<std::string, int> mUniformLocationCache;
};
