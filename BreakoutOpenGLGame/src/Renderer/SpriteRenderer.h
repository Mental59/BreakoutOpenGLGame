#pragma once

#include <glm/glm.hpp>

class ShaderProgram;
class Texture2D;

class SpriteRenderer
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void Init();

	void Draw(
		ShaderProgram* shader,
		Texture2D* texture,
		glm::vec2 position,
		glm::vec2 size,
		float rotate,
		glm::vec3 color
	);

private:
	void InitRenderData();

	unsigned int mVAO, mVBO;
};

