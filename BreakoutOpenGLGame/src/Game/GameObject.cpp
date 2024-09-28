#include "GameObject.h"
#include <type_traits>

GameObject::GameObject(GameObject&& other) noexcept
{
	mPosition = std::move(other.mPosition);
	mSize = std::move(other.mSize);
	mVelocity = std::move(other.mVelocity);
	mColor = std::move(other.mColor);
	mSprite = other.mSprite;
	mRotation = other.mRotation;
}

void GameObject::Init(const GameObjectInitOptions& options)
{
	mPosition = options.Position;
	mSize = options.Size;
	mVelocity = options.Velocity;
	mColor = options.Color;
	mRotation = options.Rotation;
	mSprite = options.Sprite;
}

bool GameObject::Collides(const GameObject& gameObject) const
{
	bool collisionX = mPosition.x + mSize.x >= gameObject.mPosition.x &&
		gameObject.mPosition.x + gameObject.mSize.x < mPosition.x;

	bool collisionY = mPosition.y + mSize.y >= gameObject.mPosition.y &&
		gameObject.mPosition.y + gameObject.mSize.y > mPosition.y;

	return collisionX && collisionY;
}
