#pragma once

#include "GameObject.h"

class BrickGameObject : public GameObject
{
public:
	struct BrickInitOptins : public GameObjectInitOptions
	{
		bool IsSolid = false;
	};

	void Init(const BrickInitOptins& options);

	const inline bool IsSolid() const { return mIsSolid; }
	const inline bool IsDestroyed() const { return mIsDestroyed; }

	const inline void SetDestroyed(const bool isDestroyed) { mIsDestroyed = isDestroyed; }

private:
	bool mIsSolid;
	bool mIsDestroyed = false;
};
