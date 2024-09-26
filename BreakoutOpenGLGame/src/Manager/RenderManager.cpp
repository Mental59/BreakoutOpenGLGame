#include "RenderManager.h"
#include "Shader/ShaderProgram.h"
#include <glad/glad.h>
#include <iostream>

RenderManager::~RenderManager()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);

	glDeleteFramebuffers(1, &mMSFBO);
	glDeleteFramebuffers(1, &mFBO);
	glDeleteRenderbuffers(1, &mRBO);
}

void RenderManager::Init(ShaderProgram* postProcessingShader, int width, int height)
{
	mPostProcessingShader = postProcessingShader;
	mWidth = width;
	mHeight = height;

	mTexture.Init(width, height, 3, nullptr);
	InitRenderData();
	InitFramebuffers(width, height);
	InitShaderData();
}

void RenderManager::Begin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mMSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderManager::End(float time)
{
	// now resolve multisampled color-buffer into intermediate FBO to store to texture
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mMSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
	glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer

	DoPostProcessing(time);
}

void RenderManager::InitRenderData()
{
	float vertices[] = {
		// pos        // tex
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(mVAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RenderManager::InitFramebuffers(int width, int height)
{
	glGenFramebuffers(1, &mMSFBO);
	glGenFramebuffers(1, &mFBO);
	glGenRenderbuffers(1, &mRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, mMSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBO);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "RENDER_MANAGER::ERROR Multisampled framebuffer is not complete" << std::endl;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture.GetId(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "RENDER_MANAGER::ERROR Framebuffer is not complete" << std::endl;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderManager::InitShaderData()
{
	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right    
	};
	int edge_kernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	float blur_kernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};

	mPostProcessingShader->Bind();
	glUniform1i(glGetUniformLocation(mPostProcessingShader->GetID(), "uScene"), 0);
	glUniform2fv(glGetUniformLocation(mPostProcessingShader->GetID(), "uOffsets"), 9, (float*)offsets);
	glUniform1iv(glGetUniformLocation(mPostProcessingShader->GetID(), "uEdgeKernel"), 9, edge_kernel);
	glUniform1fv(glGetUniformLocation(mPostProcessingShader->GetID(), "uBlurKernel"), 9, blur_kernel);
}

void RenderManager::DoPostProcessing(float time)
{
	mPostProcessingShader->Bind();
	mPostProcessingShader->SetFloat("uTime", time);
	mPostProcessingShader->SetInt("uChaos", mChaos);
	mPostProcessingShader->SetInt("uConfuse", mConfuse);
	mPostProcessingShader->SetInt("uShake", mShake);

	glActiveTexture(GL_TEXTURE0);
	mTexture.Bind();

	Draw();
}

void RenderManager::Draw()
{
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
