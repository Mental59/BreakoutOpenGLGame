#pragma once

class Shader
{
public:
	Shader(unsigned int type);
	virtual ~Shader();

	void Compile(const char* source);

	inline unsigned int GetID() const { return mID; }
	inline unsigned int GetType() const { return mType; }

private:
	unsigned int mType;
	unsigned int mID;
};
