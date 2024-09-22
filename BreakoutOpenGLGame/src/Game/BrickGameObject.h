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
	void Reset();

	const inline bool IsSolid() const { return mIsSolid; }
	const inline bool IsDestroyed() const { return mIsDestroyed; }

	const inline void Destroy() { mIsDestroyed = !mIsSolid && true; }

private:
	bool mIsSolid;
	bool mIsDestroyed = false;
};
