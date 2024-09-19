#pragma once

#include "GameObject.h"

class BallGameObject : public GameObject
{
public:
	struct BallInitOptions : public GameObjectInitOptions
	{
		float Radius;
	};

	void Init(const BallInitOptions& options);
	void Move(float dt, float windowWidth);

	inline void Activate() { mIsActive = true; }
	void FollowPaddle(const float playerDisplacementX);

private:
	float mRadius;
	bool mIsActive = false;
};
