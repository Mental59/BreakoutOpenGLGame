#include "BallGameObject.h"
#include "GameObject.h"
#include "Utils/Vector.h"
#include "Particle/ParticleEmitter.h"

void BallGameObject::Init(const BallInitOptions& options)
{
	GameObject::Init(options);
	mRadius = options.Radius;
	mInitialVelocity = options.Velocity;
	mIsActive = false;

	ParticleEmitter::ParticleEmitterInitOptions particleEmitterInitOptions;
	particleEmitterInitOptions.Position = options.Position;
	particleEmitterInitOptions.Size = options.Size / 2.0f;
	particleEmitterInitOptions.MinParticleLifetimeSeconds = 0.1f;
	particleEmitterInitOptions.MaxParticleLifetimeSeconds = 0.9f;
	particleEmitterInitOptions.NumNewParticlesEachUpdate = 2;
	particleEmitterInitOptions.NumParticles = 100;
	particleEmitterInitOptions.Sprite = options.ParticleTexture;
	mParticleEmitter.Init(particleEmitterInitOptions);
}

void BallGameObject::Move(float dt, float windowWidth)
{
	if (mIsActive)
	{
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

	mParticleEmitter.SetPosition(mPosition);
	mParticleEmitter.SetVelocity(mVelocity);
}

void BallGameObject::Update(float dt)
{
	mParticleEmitter.Update(dt, glm::vec2(mRadius / 2.0f));
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
	// TODO: It doesn't check if the ball is inside of the brick
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
