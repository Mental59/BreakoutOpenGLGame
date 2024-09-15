#include <iostream>
#include <glad/glad.h>
#include "Shader.h"

Shader::Shader(unsigned int type) : mID(0), mType(type)
{

}

Shader::~Shader()
{
	glDeleteShader(mID);

#ifdef _DEBUG
	if (mID > 0)
	{
		std::cout << "Shader with id " << mID << " destroyed" << std::endl;
	}
#endif // _DEBUG
}

void Shader::Compile(const char* source)
{
	if (mID != 0 || !source)
	{
		return;
	}

	mID = glCreateShader(mType);

	glShaderSource(mID, 1, &source, nullptr);
	glCompileShader(mID);

	GLint compileStatus;
	glGetShaderiv(mID, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		GLint infoLength;
		glGetShaderiv(mID, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 0)
		{
			char message[1024];
			glGetShaderInfoLog(mID, 1024, nullptr, message);
			std::cout << "SHADER::ERROR Failed to compile shader" << std::endl;
			std::cout << message << std::endl;
		}

		glDeleteShader(mID);
		return;
	}
}
