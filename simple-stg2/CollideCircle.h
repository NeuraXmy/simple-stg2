#pragma once

#include "Collider.h"

class PhysicObject;


class CollideCircle : public Collider
{
public:

	CollideCircle(CollideGroup group, PhysicObject* obj);

	virtual ~CollideCircle() = default;

	AABB get_aabb() const override;

	
	double get_collide_r();

	void set_collider_r(double r);


protected:

	virtual void on_collide(PhysicObject* obj) override;

private:

	bool on_test(Collider* c) override;

	double r;

};

