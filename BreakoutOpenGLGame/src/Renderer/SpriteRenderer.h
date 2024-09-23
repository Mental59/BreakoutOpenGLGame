#pragma once

#include <glm/glm.hpp>

class ShaderProgram;
class Texture2D;
class GameObject;
class GameLevel;
class ParticleEmitter;

class SpriteRenderer
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void Init();

	void Draw(
		ShaderProgram* shader,
		const Texture2D* texture,
		const glm::vec2& position,
		const glm::vec2& size,
		const float rotate,
		const glm::vec4& color
	) const;

	void DrawGameObject(
		ShaderProgram* shader,
		const GameObject* gameObject
	) const;

	void DrawGameLevel(ShaderProgram* shader, const GameLevel* level) const;
	void DrawParticles(ShaderProgram* shader, const ParticleEmitter* emitter) const;

private:
	void InitRenderData();

	unsigned int mVAO, mVBO;
};

