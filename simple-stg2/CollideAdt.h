#pragma once

#include "Collider.h"

class CollideAdt 
{
public:

	virtual ~CollideAdt() = default;

	virtual void insert(Collider* c) = 0;

	virtual void build() = 0;

	virtual int test(Collider* c) = 0;

	virtual void clear() = 0;

protected:

	bool test_collide(Collider* a, Collider* b) const;

};