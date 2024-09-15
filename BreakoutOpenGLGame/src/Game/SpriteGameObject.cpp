#include "SpriteGameObject.h"

void SpriteGameObject::Init(const InitOptions& options)
{
	mPosition = options.Position;
	mSize = options.Size;
	mVelocity = options.Velocity;
	mColor = options.Color;
	mRotation = options.Rotation;
	mSprite = options.Sprite;
	mIsSolid = options.IsSolid;
	mIsDestroyed = false;
}
