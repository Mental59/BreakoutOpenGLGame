#include <iostream>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "Shader.h"

ShaderProgram::ShaderProgram() : mID(0)
{

}

ShaderProgram::~ShaderProgram()
{
#ifdef _DEBUG
	std::cout << "Shader program with id " << mID << " destroyed" << std::endl;
#endif // _DEBUG

	glDeleteProgram(mID);
}

void ShaderProgram::Init(const Shader& vertexShader, const Shader& fragmentShader, const Shader& geometryShader)
{
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	unsigned int vertexShaderId = vertexShader.GetID();
	unsigned int fragmentShaderId = fragmentShader.GetID();
	unsigned int geometryShaderId = geometryShader.GetID();

	if (vertexShaderId) glAttachShader(shaderProgram, vertexShaderId);
	if (fragmentShaderId) glAttachShader(shaderProgram, fragmentShaderId);
	if (geometryShaderId) glAttachShader(shaderProgram, geometryShaderId);

	glLinkProgram(shaderProgram);

	int programLinkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programLinkStatus);

	if (programLinkStatus == GL_FALSE)
	{
		int infoLen = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 0)
		{
			char message[1024];
			glGetProgramInfoLog(shaderProgram, 1024, NULL, message);
			std::cout << "ERROR::SHADER_PROGRAM Linking error" << std::endl << message << std::endl;
		}

		glDeleteProgram(shaderProgram);

		mID = 0;
		return;
	}

	mID = shaderProgram;
}

void ShaderProgram::Bind()
{
	glUseProgram(mID);
}

void ShaderProgram::Unbind()
{
	glUseProgram(0);
}

void ShaderProgram::SetInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::SetVec4(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void ShaderProgram::SetMat4(const std::string& name, const float* data)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, data);
}

void ShaderProgram::SetVec3(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void ShaderProgram::SetVec3(const std::string& name, const float* data)
{
	glUniform3fv(GetUniformLocation(name), 1, data);
}

void ShaderProgram::SetVec2(const std::string& name, const float* data)
{
	glUniform2fv(GetUniformLocation(name), 1, data);
}

void ShaderProgram::SetUniformBlockBinding(const char* uniformBlockName, unsigned int binding) const
{
	unsigned int blockIndex = glGetUniformBlockIndex(mID, uniformBlockName);
	glUniformBlockBinding(mID, blockIndex, binding);
}

int ShaderProgram::GetUniformLocation(const std::string& name)
{
	auto uniformLocation = mUniformLocationCache.find(name);
	if (uniformLocation != mUniformLocationCache.end())
	{
		return uniformLocation->second;
	}

	int location = glGetUniformLocation(mID, name.c_str());
	if (location == -1)
	{
		std::cout << "WARNING::SHADER_PROGRAM uniform '" << name << "' doesn't exist" << std::endl;
	}

	mUniformLocationCache[name] = location;
	return location;
}
