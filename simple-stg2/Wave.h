#pragma once

#include "PhysicObject.h"


class Wave : public PhysicObject
{
public:

	virtual ~Wave() = default;

	
	bool is_enabled() const;

	void set_enabled(bool flag);

	void start_expand();


	double get_radius() const;
	
	void set_radius(double r);

	double get_max_radius() const;

	void set_max_radius(double r);

	double get_expand_v() const;

	void set_expand_v(double v);

	double get_expand_a() const;

	void set_expand_a(double a);

	double get_initial_expand_v() const;

	void set_initial_expand_v(double v);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;



	virtual void on_cover(PhysicObject* obj);

	virtual void on_reach_max_radius();

	virtual void on_start_expand();

private:

	double radius = 0.0;
	double last_update_radius = 0.0;

	double max_radius = 0.0;

	double expand_v = 0.0;
	double expand_a = 0.0;

	double initial_v = 0.0;

	bool enabled = false;
};

