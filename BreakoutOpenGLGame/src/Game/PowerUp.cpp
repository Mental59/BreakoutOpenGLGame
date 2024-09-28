#include "PowerUp.h"
#include <type_traits>

PowerUp::PowerUp(PowerUp&& other) noexcept : GameObject(std::forward<GameObject>(other))
{
	mDuration = other.mDuration;
	mType = other.mType;
	mActivated = other.mActivated;
	mDestroyed = other.mDestroyed;
}

void PowerUp::Init(const PowerUpInitOptions& options)
{
	GameObject::Init(options);
	mActivated = false;
	mDestroyed = false;
	mDuration = options.Duration;
	mType = options.Type;
}
