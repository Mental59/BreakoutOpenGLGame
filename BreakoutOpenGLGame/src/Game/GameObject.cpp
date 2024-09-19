#include "GameObject.h"

void GameObject::Init(const GameObjectInitOptions& options)
{
	mPosition = options.Position;
	mSize = options.Size;
	mVelocity = options.Velocity;
	mColor = options.Color;
	mRotation = options.Rotation;
	mSprite = options.Sprite;
}
