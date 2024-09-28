#pragma once

#include "GameObject.h"
#include <glm/glm.hpp>
#include "Utils/Vector.h"
#include "Particle/ParticleEmitter.h"


class BallGameObject : public GameObject
{
public:
	struct BallInitOptions : public GameObjectInitOptions
	{
		float Radius = 0.0f;
		Texture2D* ParticleTexture = nullptr;
	};

	struct BallHitResult
	{
		bool Collided = false;
		glm::vec2 HitPoint = glm::vec2(0.0f);
		VectorDirection BrickSideDirection = VectorDirection::Up;
	};

	inline float GetRadius() const { return mRadius; }
	inline const glm::vec2& GetCenter() const { return mPosition + mRadius; }
	inline bool IsActive() const { return mIsActive; }
	inline const glm::vec2& GetInitialVelocity() const { return mInitialVelocity; }
	inline const ParticleEmitter* GetParticleEmitter() const { return &mParticleEmitter; }
	inline bool IsSticky() const { return mSticky; }
	inline bool IsPassingThrough() const { return mPassThrough; }

	inline void SetSticky(bool value) { mSticky = value; }
	inline void SetPassThrough(bool value) { mPassThrough = value; }

	void Init(const BallInitOptions& options);
	void Move(float dt, float windowWidth);
	void Update(float dt);

	inline void Activate() { mIsActive = true; }
	inline void Deactivate() { mIsActive = false; }
	void FollowPaddle(const float playerDisplacementX);
	BallHitResult Collides(const GameObject& brick) const;

private:
	ParticleEmitter mParticleEmitter;
	glm::vec2 mInitialVelocity;

	float mRadius;
	bool mIsActive = false;
	bool mSticky = false;
	bool mPassThrough = false;
};
