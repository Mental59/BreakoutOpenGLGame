#include <glad/glad.h>
#include <iostream>
#include "Texture2D.h"

Texture2D::Texture2D() :
	mWidth(0),
	mHeight(0),
	mNumChannels(0),
	mID(0),
	mImageFormat(GL_RGB),
	mInternalFormat(GL_RGB8),
	mWrapS(GL_REPEAT), mWrapT(GL_REPEAT),
	mMinFilter(GL_LINEAR), mMagFilter(GL_LINEAR)
{

}

Texture2D::~Texture2D()
{
#ifdef _DEBUG
	std::cout << "Texture with id " << mID << " destroyed" << std::endl;
#endif // _DEBUG

	glDeleteTextures(1, &mID);
}


void Texture2D::Bind()
{
	glBindTexture(GL_TEXTURE_2D, mID);
}

void Texture2D::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Init(int width, int height, int numChannels, unsigned char* data)
{
	mWidth = width;
	mHeight = height;
	mNumChannels = numChannels;

	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, width, height, 0, mImageFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter);

	glBindTexture(GL_TEXTURE_2D, 0);
}
