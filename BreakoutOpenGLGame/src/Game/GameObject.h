#pragma once

#include <glm/glm.hpp>

class Texture2D;
class SpriteRenderer;

class GameObject
{
public:
	struct GameObjectInitOptions
	{
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Size = glm::vec2(1.0f);
		glm::vec2 Velocity = glm::vec2(0.0f);
		glm::vec4 Color = glm::vec4(1.0f);
		float Rotation = 0.0f;
		Texture2D* Sprite = nullptr;
	};

	GameObject() = default;
	GameObject(GameObject&& other) noexcept;

	GameObject& operator=(const GameObject& other) = default;

	void Init(const GameObjectInitOptions& options);

	const inline glm::vec2& GetPosition() const { return mPosition; }
	const inline glm::vec2& GetSize() const { return mSize; }
	const inline glm::vec2& GetVelocity() const { return mVelocity; }
	const inline glm::vec4& GetColor() const { return mColor; }
	const inline float GetRotation() const { return mRotation; }
	const inline Texture2D* GetSprite() const { return mSprite; }

	const inline void SetPosition(const glm::vec2& position) { mPosition = position; }
	const inline void SetSize(const glm::vec2& size) { mSize = size; }
	const inline void SetVelocity(const glm::vec2& velocity) { mVelocity = velocity; }
	const inline void SetColor(const glm::vec4& color) { mColor = color; }
	const inline void SetRotation(const float rotation) { mRotation = rotation; }

	bool Collides(const GameObject& gameObject) const;
protected:
	glm::vec2 mPosition, mSize, mVelocity;
	glm::vec4 mColor;
	Texture2D* mSprite;
	float mRotation;
};
