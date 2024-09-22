#include "BallGameObject.h"
#include "GameObject.h"
#include "Utils/Vector.h"

void BallGameObject::Init(const BallInitOptions& options)
{
	GameObject::Init(options);
	mRadius = options.Radius;
	mInitialVelocity = options.Velocity;
	mIsActive = false;
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

BallGameObject::BallHitResult BallGameObject::Collides(const GameObject* gameObject) const
{
	const glm::vec2 brickHalfSize = gameObject->GetSize() / 2.0f;
	const glm::vec2 brickCenter = gameObject->GetPosition() + brickHalfSize;

	const glm::vec2 ballCenter = mPosition + mRadius;

	const glm::vec2 closestPoint = brickCenter + glm::clamp(ballCenter - brickCenter, -brickHalfSize, brickHalfSize);
	const glm::vec2 ballCenterToClosestPoint = closestPoint - ballCenter;

	if (glm::length(ballCenterToClosestPoint) <= mRadius)
	{
		VectorDirection direction = GetVectorDirection(glm::normalize(-ballCenterToClosestPoint));
		return { true, closestPoint, direction };
	}

	return { false };
}
