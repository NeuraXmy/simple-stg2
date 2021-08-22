#pragma once

#include <vector>

#include "SpriteObject.h"
#include "CollideCircle.h"
#include "Particle.h"

namespace Sound { class Se; }



class Bullet : public SpriteObject, public CollideCircle
{
public:

	Bullet(CollideGroup group);

	virtual ~Bullet() = default;


	bool is_boundary_test_enabled() const;

	void set_boundary_test(bool flag);

	void set_boundary(double l, double r, double t, double b);


	void kill(bool drop_item = true);

	void kill_raw();

	bool is_killed() const;


	Particle::DataSet& kill_particle();
	
	const Particle::DataSet& kill_particle() const;

	Particle::DataSet& create_particle();

	const Particle::DataSet& create_particle() const;


	Sound::Se* get_create_se() const;

	void set_create_se(Sound::Se* se);

	Sound::Se* get_kill_se() const;

	void set_kill_se(Sound::Se* se);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_collide(PhysicObject* obj) override;


	virtual void on_kill();

	virtual void on_drop_item();

private:

	bool boundary_test = true;
	double boundary_t = 20.0;
	double boundary_b = 20.0;
	double boundary_l = 20.0;
	double boundary_r = 20.0;

	bool killed = false;

	Particle::DataSet kill_particle_data;
	Particle::DataSet create_particle_data;

	Sound::Se* kill_se = nullptr;
	Sound::Se* create_se = nullptr;

};
