#pragma once

#include "Texture/Texture2D.h"

class ShaderProgram;

class RenderManager
{
public:
	RenderManager() = default;
	~RenderManager();

	void Init(ShaderProgram* postProcessingShader, int width, int height);
	void Begin();
	void End(float time);

	inline void SetConfuse(bool value) { mConfuse = value; }
	inline void SetShake(bool value) { mShake = value; }
	inline void SetChaos(bool value) { mChaos = value; }

private:
	void InitRenderData();
	void InitFramebuffers(int width, int height);
	void InitShaderData();

	void DoPostProcessing(float time);
	void Draw();

	unsigned int mVAO, mVBO;
	int mWidth, mHeight;

	ShaderProgram* mPostProcessingShader;
	Texture2D mTexture;

	unsigned int mMSFBO, mFBO, mRBO;
	bool mConfuse, mShake, mChaos;
};
