#include "CollidePointSet.h"
#include "CollideCircle.h"
#include "PhysicObject.h"


CollidePointSet::CollidePointSet(CollideGroup group, PhysicObject* obj) : 
	Collider(ColliderType::point_set, group, obj)
{

}


AABB CollidePointSet::get_aabb() const
{
    AABB aabb = { 1e9, 1e9, -1e9, -1e9 };

    double rot = get_obj()->rot();

    for (const auto& p : points)
    {
        double x = p.x * cos(rot) - p.y * sin(rot);
        double y = p.y * cos(rot) + p.x * sin(rot);

        aabb.x1 = std::min(aabb.x1, x - p.r);
        aabb.y1 = std::min(aabb.y1, y - p.r);
        aabb.x2 = std::max(aabb.x2, x + p.r);
        aabb.y2 = std::max(aabb.y2, y + p.r);
    }

    double ox = get_obj()->x();
    double oy = get_obj()->y();
    aabb.x1 += ox, aabb.x2 += ox;
    aabb.y1 += oy, aabb.y2 += oy;
    return aabb;
}

std::vector<CollidePointSet::Point>& CollidePointSet::get_collide_points()
{
	return points;
}

const std::vector<CollidePointSet::Point>& CollidePointSet::get_collide_points() const
{
	return points;
}

CollidePointSet::Point CollidePointSet::get_collided_point() const
{
	return collided_point;
}


void CollidePointSet::on_collide(PhysicObject* obj)
{
	
}

bool CollidePointSet::on_test(Collider* c)
{
    if (c->type == ColliderType::circle)
    {
        auto p = reinterpret_cast<CollideCircle*>(c);
        double sx = get_obj()->x();
        double sy = get_obj()->y();
        double rot = get_obj()->rot();
        double px = p->get_obj()->x();
        double py = p->get_obj()->y();
        double pr = p->get_collide_r();
    
        for (const auto& p : get_collide_points())
        {
            if (!p.enable)
                continue;

            double dx = sx + p.x * cos(rot) - p.y * sin(rot) - px;
            double dy = sy + p.y * cos(rot) + p.x * sin(rot) - py;
            double rs = p.r + pr;
            if (dx * dx + dy * dy <= rs * rs + 1e-8)
            {
                collided_point = p;
                return true;
            }
        }
        return false;
    }
	
	return false;
}

