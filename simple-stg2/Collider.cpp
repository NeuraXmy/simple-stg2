#include "Collider.h"
#include "CollideSystem.h"

Collider::Collider(ColliderType type, CollideGroup group, PhysicObject* obj) :
    group(group), obj(obj), type(type)
{
    if (group == CollideGroup::none)
    {
        return;
    }

    auto& l = CollideSystem::instance()->colliders[(int)group];
    l.push_back(this);
    iter_to_list = l.end();
    iter_to_list--;
}

Collider::~Collider()
{
    if (group == CollideGroup::none)
    {
        return;
    }

    auto& l = CollideSystem::instance()->colliders[(int)group];
    l.erase(iter_to_list);
}


void Collider::set_collide_group(CollideGroup group)
{
    if (this->group == group)
    {
        return;
    }

    if (this->group != CollideGroup::none)
    {
        auto& l = CollideSystem::instance()->colliders[(int)this->group];
        l.erase(iter_to_list);
        this->group = CollideGroup::none;
    }

    this->group = group;
    auto& l = CollideSystem::instance()->colliders[(int)group];
    l.push_back(this);
    iter_to_list = l.end();
    iter_to_list--;
}

CollideGroup Collider::collide_group() const
{
    return group;
}

bool Collider::is_collide_enabled() const
{
    return enable;
}

void Collider::set_collide(bool enable)
{
    this->enable = enable;
}


bool Collider::test(Collider* c)
{
    if (!enable || !c->enable)
    {
        return false;
    }
    return on_test(c);
}

PhysicObject* Collider::get_obj() const
{
    return obj;
}


bool Collider::on_test(Collider* c)
{
    return false;
}

void Collider::on_collide(PhysicObject* obj)
{

}

AABB Collider::get_aabb() const
{
    return AABB{};
}
