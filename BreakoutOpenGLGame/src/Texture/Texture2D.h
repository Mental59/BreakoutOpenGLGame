#pragma once

class Texture2D
{
public:
	Texture2D();
	Texture2D(Texture2D&& other) noexcept;
	~Texture2D();

	void Init(int width, int height, int numChannels, unsigned char* data);
	void Bind() const;
	void Unbind() const;

	inline void SetImageFormat(int imageFormat) { mImageFormat = imageFormat; }
	inline void SetInternalFormat(unsigned int internalFormat) { mInternalFormat = internalFormat; }
	inline void SetWrap(int wrapS, int wrapT) { mWrapS = wrapS; mWrapT = wrapT; }
	inline void SetFilter(int minFilter, int magFilter) { mMinFilter = minFilter; mMagFilter = magFilter; }

	inline unsigned int GetId() const { return mID; }
private:
	int mWidth, mHeight, mNumChannels;

	unsigned int mID;

	unsigned int mImageFormat;
	int mInternalFormat;

	int mWrapS, mWrapT, mMinFilter, mMagFilter;
};
