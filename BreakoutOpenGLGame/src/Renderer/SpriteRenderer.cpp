#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SpriteRenderer.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Game/GameObject.h"
#include "Game/GameLevel.h"

SpriteRenderer::SpriteRenderer() : mVAO(0), mVBO(0)
{

}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

void SpriteRenderer::Init()
{
	InitRenderData();
}

void SpriteRenderer::Draw(
	ShaderProgram* shader,
	const Texture2D* texture,
	const glm::vec2& position,
	const glm::vec2& size,
	const float rotate,
	const glm::vec3& color
) const
{
	shader->Bind();

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(size.x / 2.0f, size.y / 2.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, -glm::vec3(size.x / 2.0f, size.y / 2.0f, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	shader->SetMat4("uModel", glm::value_ptr(model));
	shader->SetVec3("uSpriteColor", glm::value_ptr(color));

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawGameObject(
	ShaderProgram* shader,
	const GameObject* gameObject
) const
{
	Draw(
		shader,
		gameObject->GetSprite(),
		gameObject->GetPosition(),
		gameObject->GetSize(),
		gameObject->GetRotation(),
		gameObject->GetColor()
	);
}

void SpriteRenderer::DrawGameLevel(ShaderProgram* shader, const GameLevel* level) const
{
	unsigned int numBricks = level->GetNumBricks();
	GameObject* bricks = level->GetBricks();

	for (unsigned int i = 0; i < numBricks; i++)
	{
		if (!bricks[i].isDestroyed())
		{
			DrawGameObject(shader, &bricks[i]);
		}
	}
}

void SpriteRenderer::InitRenderData()
{
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f, // top left
		1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		0.0f, 0.0f, 0.0f, 0.0f, // bottom left

		0.0f, 1.0f, 0.0f, 1.0f, // top left
		1.0f, 1.0f, 1.0f, 1.0f, // top right
		1.0f, 0.0f, 1.0f, 0.0f // bottom right
	};

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(mVAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
