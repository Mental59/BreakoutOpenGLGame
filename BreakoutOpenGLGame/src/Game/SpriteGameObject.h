#pragma once

#include <glm/glm.hpp>

class Texture2D;
class SpriteRenderer;

class SpriteGameObject
{
public:
	struct InitOptions
	{
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Size = glm::vec2(1.0f);
		glm::vec2 Velocity = glm::vec2(0.0f);
		glm::vec3 Color = glm::vec3(1.0f);
		float Rotation = 0.0f;
		Texture2D* Sprite = nullptr;
		bool IsSolid = false;
	};

	SpriteGameObject() = default;
	void Init(const InitOptions& options);

	const inline glm::vec2& GetPosition() const { return mPosition; }
	const inline glm::vec2& GetSize() const { return mSize; }
	const inline glm::vec2& GetVelocity() const { return mVelocity; }
	const inline glm::vec3& GetColor() const { return mColor; }
	const inline float GetRotation() const { return mRotation; }
	const inline bool IsSolid() const { return mIsSolid; }
	const inline bool isDestroyed() const { return mIsDestroyed; }
	const inline Texture2D* GetSprite() const { return mSprite; }

private:
	glm::vec2 mPosition, mSize, mVelocity;
	glm::vec3 mColor;
	float mRotation;
	Texture2D* mSprite;
	bool mIsSolid;
	bool mIsDestroyed = false;
};
