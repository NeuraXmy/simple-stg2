#include "CollideCircle.h"
#include "CollidePointSet.h"
#include "PhysicObject.h"
#include <iostream>


CollideCircle::CollideCircle(CollideGroup group, PhysicObject* obj) : 
    Collider(ColliderType::circle, group, obj), r(0.0)
{
}


AABB CollideCircle::get_aabb() const
{
    double ox = get_obj()->x();
    double oy = get_obj()->y();
    return AABB{ ox - r, oy - r, ox + r, oy + r };
}

double CollideCircle::get_collide_r()
{
    return r;
}

void CollideCircle::set_collider_r(double r)
{
    this->r = r;
}

void CollideCircle::on_collide(PhysicObject* obj)
{

}

bool CollideCircle::on_test(Collider* c)
{
    if (c->type == ColliderType::circle)
    {
        auto p = reinterpret_cast<CollideCircle*>(c);
        double dx = p->get_obj()->x() - get_obj()->x();
        double dy = p->get_obj()->y() - get_obj()->y();
        double rs = p->get_collide_r() + get_collide_r();
        return dx * dx + dy * dy <= rs * rs + 1e-8;
    }

    if (c->type == ColliderType::point_set)
    {
        return c->test(this);
    }

    return false;
}
