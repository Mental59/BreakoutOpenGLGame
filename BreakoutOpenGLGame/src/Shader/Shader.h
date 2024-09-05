#pragma once

#include <string>

class Shader
{
public:
	enum ShaderType
	{
		Vertex, Fragment, Geometry
	};

	Shader(const char* sourcePath, ShaderType type);
	virtual ~Shader();

	inline const char* GetSource() const { return mSource.c_str(); }
	inline unsigned int GetID() const { return mID; }
	inline unsigned int GetType() const { return mType; }

	void Compile();

private:
	std::string ReadSource(const char* sourcePath);
	unsigned int Compile(const char* source, unsigned int type);

	std::string mSource;
	unsigned int mType;
	unsigned int mID;
};
