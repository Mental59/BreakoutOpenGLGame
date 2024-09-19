#include "BrickGameObject.h"
#include "GameObject.h"

void BrickGameObject::Init(const BrickInitOptins& options)
{
	GameObject::Init(options);
	mIsSolid = options.IsSolid;
}
