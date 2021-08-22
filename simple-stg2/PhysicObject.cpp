#include <cmath>

#include "PhysicObject.h"


double PhysicObject::x() const
{
	return need_follow() 
		? d.x * std::cos(d.bound_obj->rot()) - d.y * std::sin(d.bound_obj->rot()) + d.bound_obj->x()
		: d.x;
}

double PhysicObject::y() const
{
	return need_follow() 
		? d.y * std::cos(d.bound_obj->rot()) + d.x * std::sin(d.bound_obj->rot()) + d.bound_obj->y()
		: d.y;
}

double PhysicObject::x_relative() const
{
	return d.x;
}

double PhysicObject::y_relative() const
{
	return d.y;
}

double PhysicObject::vx() const
{
	return d.vx;
}

double PhysicObject::vy() const
{
	return d.vy;
}

double PhysicObject::ax() const
{
	return d.ax;
}

double PhysicObject::v() const
{
	return std::sqrt(vx() * vx() + vy() * vy());
}

double PhysicObject::ay() const
{
	return d.ay;
}

double PhysicObject::rot() const
{
	return need_follow() ? d.rot + d.bound_obj->rot() : d.rot;
}

double PhysicObject::rot_relative() const
{
	return d.rot;
}

double PhysicObject::omega() const
{
	return d.omega;
}


void PhysicObject::set_x(double x)
{
	d.x = x;
}

void PhysicObject::set_y(double y)
{
	d.y = y;
}

void PhysicObject::set_pos(double x, double y)
{
	d.x = x;
	d.y = y;
}

void PhysicObject::set_vx(double vx)
{
	d.vx = vx;
}

void PhysicObject::set_pos(const PhysicObject* obj)
{
	set_pos(obj->x(), obj->y());
}

void PhysicObject::set_vy(double vy)
{
	d.vy = vy;
}

void PhysicObject::set_v(double vx, double vy)
{
	d.vx = vx;
	d.vy = vy;
}

void PhysicObject::set_ax(double ax)
{
	d.ax = ax;
}

void PhysicObject::set_ay(double ay)
{
	d.ay = ay;
}

void PhysicObject::set_a(double ax, double ay)
{
	d.ax = ax;
	d.ay = ay;
}

void PhysicObject::set_rot(double rot)
{
	d.rot = rot;
}

void PhysicObject::set_omega(double omega)
{
	d.omega = omega;
}


bool PhysicObject::pos_fixed() const
{
	return d.pos_fixed;
}

bool PhysicObject::rot_fixed() const
{
	return d.rot_fixed;
}

bool PhysicObject::anchored() const
{
	return d.anchored;
}

void PhysicObject::set_pos_fixed(bool flag)
{
	d.pos_fixed = flag;
}

void PhysicObject::set_rot_fixed(bool flag)
{
	d.rot_fixed = flag;
}

void PhysicObject::set_anchored(bool flag)
{
	d.anchored = flag;

	if (d.anchored)
	{
		point_to_v();
	}
}


PhysicObject* PhysicObject::bound_obj()
{
	return d.bound_obj;
}

const PhysicObject* PhysicObject::bound_obj() const
{
	return d.bound_obj;
}

const std::list<PhysicObject*>& PhysicObject::child_list() const
{
	return d.child_list;
}

bool PhysicObject::follow_bound() const
{
	return d.follow_bound;
}

void PhysicObject::bind(PhysicObject* obj)
{
	if (d.bound_obj)
	{
		set_pos(x(), y());
		set_rot(rot());
		d.bound_obj->d.child_list.remove(this);
		d.bound_obj = nullptr;
	}
	if (obj)
	{
		obj->d.child_list.push_back(this);
		d.bound_obj = obj;
	}
}

void PhysicObject::set_follow_bound(bool flag)
{
	d.follow_bound = flag;
}


void PhysicObject::point_to(double x, double y)
{
	d.rot = atan2(y - this->y(), x - this->x());
}

void PhysicObject::point_to(const PhysicObject* object)
{
	d.rot = atan2(object->y() - y(), object->x() - x());
}

double PhysicObject::get_point_to_rot(double x, double y) const
{
	return atan2(y - d.y, x - d.x);
}

double PhysicObject::get_point_to_rot(const PhysicObject* object) const
{
	return atan2(object->y() - y(), object->x() - x());
}

void PhysicObject::point_to_v()
{
	d.rot = atan2(d.vy, d.vx);
}

void PhysicObject::set_v_to_rot()
{
	double v = sqrt(d.vx * d.vx + d.vy * d.vy);
	d.vx = v * cos(d.rot);
	d.vy = v * sin(d.rot);
}

void PhysicObject::set_a_to_rot()
{
	double a = sqrt(d.ax * d.ax + d.ay * d.ay);
	d.ax = a * cos(d.rot);
	d.ay = a * sin(d.rot);
}

void PhysicObject::point_v_to(double x, double y)
{
	double v = this->v();
	double r = get_point_to_rot(x, y);
	set_v(v * cos(r), v * sin(r));
}

void PhysicObject::point_a_to(double x, double y)
{
	double a = sqrt(d.ax * d.ax + d.ay * d.ay);
	double r = get_point_to_rot(x, y);
	set_a(a * cos(r), a * sin(r));
}

void PhysicObject::point_v_to(const PhysicObject* obj)
{
	point_v_to(obj->x(), obj->y());
}

void PhysicObject::point_a_to(const PhysicObject* obj)
{
	point_a_to(obj->x(), obj->y());
}

void PhysicObject::point_v_to(double rot)
{
	double v = this->v();
	set_v(v * cos(rot), v * sin(rot));
}

void PhysicObject::point_a_to(double rot)
{
	double a = sqrt(d.ax * d.ax + d.ay * d.ay);
	set_a(a * cos(rot), a * sin(rot));
}

double PhysicObject::get_point_to_v_rot() const
{
	return atan2(d.vy, d.vx);
}

double PhysicObject::get_dist(double x, double y) const
{
	double dx = this->x() - x;
	double dy = this->y() - y;
	return sqrt(dx * dx + dy * dy);
}

double PhysicObject::get_dist(const PhysicObject* object) const
{
	return get_dist(object->x(), object->y());
}

double PhysicObject::get_v_rot() const
{
	return atan2(vy(), vx());
}

double PhysicObject::get_a_rot() const
{
	return atan2(ay(), ax());
}

double PhysicObject::get_min_v() const
{
	return d.min_v;
}

void PhysicObject::set_min_v(double v)
{
	d.min_v = v;
}

double PhysicObject::get_max_v() const
{
	return d.max_v;
}

void PhysicObject::set_max_v(double v)
{
	d.max_v = v;
}


void PhysicObject::on_create()
{
	GameObject::on_create();


}

void PhysicObject::on_update()
{
	GameObject::on_update();

	if (!d.pos_fixed)
	{
		d.vx += d.ax;
		d.vy += d.ay;

		if (d.max_v < 1e8 && v() >= d.max_v)
		{
			d.ax = 0.0;
			d.ay = 0.0;
			d.max_v =  1e9;
		}
		if (d.min_v > 0.0 && v() <= d.min_v)
		{
			d.ax = 0.0;
			d.ay = 0.0;
			d.min_v = -1e9;
		}

		d.x += d.vx;
		d.y += d.vy;
	}

	if (!d.rot_fixed)
	{
		d.rot += d.omega;

		if (d.anchored)
		{
			point_to_v();
		}
	}
}

void PhysicObject::on_destroy()
{
	this->bind(nullptr);
	auto tmp = child_list();
	for (auto child : tmp)
	{
		child->on_bound_obj_destroyed(this);
		child->bind(nullptr);
	}

	GameObject::on_destroy();
}

void PhysicObject::on_draw()
{
	GameObject::on_draw();

}

void PhysicObject::on_draw_ui()
{
}

void PhysicObject::on_bound_obj_destroyed(PhysicObject* bound_obj) 
{

}


bool PhysicObject::need_follow() const
{
	return d.bound_obj && d.follow_bound;
}