#include "CollideAdt.h"

bool CollideAdt::test_collide(Collider* a, Collider* b) const
{
	if (a->test(b))
		return a->on_collide(b->get_obj()), b->on_collide(a->get_obj()), true;
	else
		return false;
}
