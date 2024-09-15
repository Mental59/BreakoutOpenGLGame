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

Texture2D::Texture2D(Texture2D&& other) noexcept
{
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mNumChannels = other.mNumChannels;
	mID = other.mID;
	mImageFormat = other.mImageFormat;
	mInternalFormat = other.mInternalFormat;
	mWrapS = other.mWrapS;
	mWrapT = other.mWrapT;
	mMinFilter = other.mMinFilter;
	mMagFilter = other.mMagFilter;

	other.mID = 0;
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &mID);

#ifdef _DEBUG
	if (mID > 0)
	{
		std::cout << "Texture with id " << mID << " destroyed" << std::endl;
	}
#endif // _DEBUG
}


void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, mID);
}

void Texture2D::Unbind() const
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
