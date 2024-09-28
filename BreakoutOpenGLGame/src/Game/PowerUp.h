#pragma once

#include "GameObject.h"
#include <iostream>

class PowerUp : public GameObject
{
public:
	enum PowerUpType : unsigned char
	{
		Speed = 0,
		Sticky,
		PassThrough,
		PadSizeIncrease,
		Confuse,
		Chaos
	};

	struct PowerUpInitOptions : public GameObjectInitOptions
	{
		PowerUpType Type;
		float Duration;
	};

	PowerUp() = default;
	PowerUp(PowerUp&& other) noexcept;

	void Init(const PowerUpInitOptions& options);

	inline bool IsDestroyed() const { return mDestroyed; }
	inline bool IsActivated() const { return mActivated; }
	inline PowerUpType GetType() const { return mType; }
	inline float GetDuration() const { return mDuration; }

	inline void Activate() { mActivated = true; }
	inline void Deactivate() { mActivated = false; }
	inline void Destroy() { mDestroyed = true; }
	inline void DecreaseDuration(float value) { mDuration -= value; }

private:
	float mDuration;
	PowerUpType mType;
	bool mActivated, mDestroyed;
};
