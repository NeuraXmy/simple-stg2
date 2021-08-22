#pragma once

#include <list>

struct AABB
{
	double x1, y1, x2, y2;
	double w() const { return x2 - x1; }
	double h() const { return y2 - y1; }
	double cx() const { return (x1 + x2) * 0.5; }
	double cy() const { return (y1 + y2) * 0.5; }
	void unite(const AABB& r) 
	{
		x1 = std::min(x1, r.x1);
		y1 = std::min(y1, r.y1);
		x2 = std::max(x2, r.x2);
		y2 = std::max(y2, r.y2);
	}
	void intersect(const AABB& r) 
	{
		x1 = std::max(x1, r.x1);
		y1 = std::max(y1, r.y1);
		x2 = std::min(x2, r.x2);
		y2 = std::min(y2, r.y2);
	}
	bool test(const AABB& r) const {
		static constexpr double eps = 1e-7;
		return std::min(x2, r.x2) >= std::max(x1, r.x1) - eps 
			&& std::min(y2, r.y2) >= std::max(y1, r.y1) - eps;
	}
};

enum class CollideGroup
{
	all,
	player,
	player_bullet,
	player_bullet_bomb,
	enemy,
	enemy_bullet,
	item, 
	collide_group_num,
	none
};

enum class ColliderType
{
	collider,
	circle,
	point_set
};

class PhysicObject;


class Collider
{
	friend class CollideSystem;
	friend class CollideAdt;

public:

	const ColliderType type;


	Collider(ColliderType type, CollideGroup group, PhysicObject* obj);

	virtual ~Collider();

	virtual AABB get_aabb() const;
	

	void set_collide_group(CollideGroup group);

	CollideGroup collide_group() const;

	bool is_collide_enabled() const;

	void set_collide(bool enable);

	bool test(Collider* c);

	PhysicObject* get_obj() const;


protected:

	virtual bool on_test(Collider* c);

	virtual void on_collide(PhysicObject* obj);

private:

	PhysicObject* obj = nullptr;

	CollideGroup group = CollideGroup::none;

	bool enable = true;

	std::list<Collider*>::iterator iter_to_list;

};

