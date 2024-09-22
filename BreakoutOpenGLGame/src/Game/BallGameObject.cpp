#include "BallGameObject.h"
#include "GameObject.h"
#include "BrickGameObject.h"

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

bool BallGameObject::Collides(const BrickGameObject* brick) const
{
	const glm::vec2 brickHalfSize = brick->GetSize() / 2.0f;
	const glm::vec2 brickCenter = brick->GetPosition() + brickHalfSize;

	const glm::vec2 ballCenter = mPosition + mRadius;

	const glm::vec2 closest = brickCenter + glm::clamp(ballCenter - brickCenter, -brickHalfSize, brickHalfSize);

	return glm::length(closest - ballCenter) < mRadius;
}
