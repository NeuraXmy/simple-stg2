#pragma once

#include <vector>
#include "Collider.h"

class PhysicObject;


class CollidePointSet : public Collider
{
public:

	struct Point
	{
		double x = 0.0;
		double y = 0.0;
		double r = 0.0;
		bool enable = true;
	};

	CollidePointSet(CollideGroup group, PhysicObject* obj);

	virtual ~CollidePointSet() = default;

	AABB get_aabb() const override;


	std::vector<Point>& get_collide_points();

	const std::vector<Point>& get_collide_points() const;

	Point get_collided_point() const;


protected:

	virtual void on_collide(PhysicObject* obj) override;


private:

	std::vector<Point> points;

	Point collided_point;


	bool on_test(Collider* c) override;

};

