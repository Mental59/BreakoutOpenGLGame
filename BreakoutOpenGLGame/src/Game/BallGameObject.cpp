#include "BallGameObject.h"
#include "GameObject.h"

void BallGameObject::Init(const BallInitOptions& options)
{
	GameObject::Init(options);
	mRadius = options.Radius;
}

void BallGameObject::Move(float dt, float windowWidth)
{
	if (!mIsActive)
	{
		return;
	}

	mPosition += mVelocity * dt;

	if (mPosition.x <= 0.0f)
	{
		mVelocity.x = -mVelocity.x;
		mPosition.x = 0.0f;
	}
	else if (mPosition.x + mSize.x >= windowWidth)
	{
		mVelocity.x = -mVelocity.x;
		mPosition.x = windowWidth - mSize.x;
	}
	else if (mPosition.y <= 0.0f)
	{
		mVelocity.y = -mVelocity.y;
		mPosition.y = 0.0f;
	}
}

void BallGameObject::FollowPaddle(const float playerDisplacementX)
{
	if (mIsActive)
	{
		return;
	}

	mPosition.x += playerDisplacementX;
}
