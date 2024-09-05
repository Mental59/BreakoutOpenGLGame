#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>

Shader::Shader(const char* sourcePath, ShaderType type)
{
	switch (type)
	{
	case Shader::Vertex:
		mType = GL_VERTEX_SHADER;
		break;
	case Shader::Fragment:
		mType = GL_FRAGMENT_SHADER;
		break;
	case Shader::Geometry:
		mType = GL_GEOMETRY_SHADER;
		break;
	}

	mSource = ReadSource(sourcePath);
	Compile();
}

Shader::~Shader()
{
	glDeleteShader(mID);
}

void Shader::Compile()
{
	mID = Compile(mSource.c_str(), mType);
}

std::string Shader::ReadSource(const char* sourcePath)
{
	std::ifstream stream(sourcePath);
	std::stringstream sourceStream;
	sourceStream << stream.rdbuf();
	return sourceStream.str();
}

GLuint Shader::Compile(const char* source, unsigned int type)
{
	GLuint id = glCreateShader(type);

	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	GLint compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		GLint infoLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 0)
		{
			char message[1024];
			glGetShaderInfoLog(id, 1024, nullptr, message);
			std::cout << "SHADER::ERROR Failed to compile shader" << std::endl;
			std::cout << message << std::endl;
		}

		glDeleteShader(id);
		return 0;
	}

	return id;
}