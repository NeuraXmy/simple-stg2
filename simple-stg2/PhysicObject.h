#pragma once

#include <list>

#include "GameObject.h"


class PhysicObject : public GameObject
{
public:

	virtual ~PhysicObject() = default;


	double x() const;

	double y() const;

	double x_relative() const;

	double y_relative() const;

	double vx() const;

	double vy() const;

	double v() const;

	double ax() const;

	double ay() const;

	double rot() const;

	double rot_relative() const;

	double omega() const;


	void set_x(double x);

	void set_y(double y);

	void set_pos(double x, double y);

	void set_pos(const PhysicObject* obj);

	void set_vx(double vx);

	void set_vy(double vy);

	void set_v(double vx, double vy);

	void set_ax(double ax);

	void set_ay(double ay);

	void set_a(double ax, double ay);

	void set_rot(double rot);

	void set_omega(double omega);


	bool pos_fixed() const;

	bool rot_fixed() const;

	bool anchored() const;

	void set_pos_fixed(bool flag);

	void set_rot_fixed(bool flag);

	void set_anchored(bool flag);



	PhysicObject* bound_obj();

	const PhysicObject* bound_obj() const;

	const std::list<PhysicObject*>& child_list() const;

	bool follow_bound() const;

	void bind(PhysicObject* obj);

	void set_follow_bound(bool flag);


	void point_to(double x, double y);

	void point_to(const PhysicObject* object);

	double get_point_to_rot(double x, double y) const;

	double get_point_to_rot(const PhysicObject* object) const;

	void point_to_v();

	void set_v_to_rot();

	void set_a_to_rot();

	void point_v_to(double x, double y);

	void point_a_to(double x, double y);

	void point_v_to(const PhysicObject * obj);

	void point_a_to(const PhysicObject * obj);

	void point_v_to(double rot);
	
	void point_a_to(double rot);

	double get_point_to_v_rot() const;

	double get_dist(double x, double y) const;

	double get_dist(const PhysicObject* object) const;

	double get_v_rot() const;

	double get_a_rot() const;

	
	double get_min_v() const;

	void set_min_v(double v);

	double get_max_v() const;

	void set_max_v(double v);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_draw_ui() override;


	virtual void on_bound_obj_destroyed(PhysicObject* bound_obj);


private:

	struct Data {

		double x = 0.0;
		double y = 0.0;
		double vx = 0.0;
		double vy = 0.0;
		double ax = 0.0;
		double ay = 0.0;

		double min_v = -1e9;
		double max_v =  1e9;

		double rot = 0.0;
		double omega = 0.0;

		bool pos_fixed = false;
		bool rot_fixed = false;
		bool anchored = false;

		bool follow_bound = true;
		PhysicObject* bound_obj = nullptr;
		std::list<PhysicObject*> child_list;
	};

	Data d;

	bool need_follow() const;

};

