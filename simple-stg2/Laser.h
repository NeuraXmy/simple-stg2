#pragma once

#include "SpriteObject.h"
#include "CollidePointSet.h"
#include "Particle.h"

namespace Sound { class Se;  }


class Laser : public SpriteObject, public CollidePointSet
{
public:

	enum class State
	{
		off,
		on,
		half_on
	};


	Laser();

	virtual ~Laser() = default;


	bool is_head_light() const;

	void set_head_light(bool flag);

	bool is_tail_light() const;

	void set_tail_light(bool flag);

	double get_light_offset() const;

	void set_light_offset(double offset);

	double get_light_omega() const;

	void set_light_omega(double omega);

	Sprite& light_sprite();

	const Sprite& light_sprite() const;


	Sprite& create_sprite();

	const Sprite& create_sprite() const;

	
	double get_w() const;

	void set_w(double w);

	double get_len() const;

	void set_len(double len);

	double get_full_len() const;

	void set_full_len(double len);

	bool is_grow_len() const;

	void set_grow_len(bool flag);

	void set_two_point(double head_x, double head_y, double tail_x, double tail_y, double offset = 0.0);

	double center_x() const;

	double center_y() const;

	double center_x_relative() const;

	double center_y_relative() const;


	void set_tail_physic_enabled(bool flag);

	bool is_tail_physic_enabled(bool flag);

	PhysicObject* get_tail_physic() const;


	void turn_half_on();

	void turn_on(int time);

	void turn_off(int time);

	bool is_half_on() const;

	bool is_on() const;

	bool is_off() const;

	State get_state() const;


	double get_half_on_w() const;

	void set_half_on_w(double w);

	
	void set_collider_interval(double interval);

	void set_head_tail_no_collider_count(int head, int tail);

	double get_collider_w() const;

	void set_collider_w(double w);


	bool is_grazed() const;

	void graze();

	int get_graze_interval() const;

	void set_graze_interval(int interval);


	bool is_can_be_killed_by_player() const;

	void set_can_be_killed_by_player(bool flag);


	bool is_boundary_test_enabled() const;

	void set_boundary_test(bool flag);

	void set_boundary(double l, double r, double t, double b);


	void shorten_from_head(double len, bool drop_item = true);

	void shorten_from_tail(double len, bool drop_item = true);

	void set_shorten_from(bool is_from_head);

	bool is_shorten_from_head() const;

	bool is_shorten_from_tail() const;

	void kill(bool drop_item = true);

	void kill_raw();

	bool is_killed() const;

	bool is_kill_when_bound_enemy_killed() const;

	void set_kill_when_bound_enemy_killed(bool flag);

	bool is_destroy_when_off() const;

	void set_destroy_when_off(bool flag);


	Particle::DataSet& kill_particle();

	const Particle::DataSet& kill_particle() const;

	
	double get_drop_interval() const;

	void set_drop_interval(double interval);

	template <class Func>
	void foreach_drop_point(Func&& func)
	{
		double now = drop_interval * 0.5;
		do
		{
			func(x() - now * cos(rot()), y() - now * sin(rot()));
			now += drop_interval;
		} while (now <= len + 1e-8);
	}

	template <class Func>
	void foreach_drop_point(double l, double r, Func&& func)
	{
		double now = l + drop_interval * 0.5;
		do
		{
			func(x() - now * cos(rot()), y() - now * sin(rot()));
			now += drop_interval;
		} while (now <= r);
	}


	Sound::Se* get_on_se() const;

	void set_on_se(Sound::Se* se);

	Sound::Se* get_kill_se() const;

	void set_kill_se(Sound::Se* se);


protected:
	
	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_collide(PhysicObject* obj) override;

	virtual void on_bound_obj_destroyed(PhysicObject* bound_obj) override;


	virtual void on_drop_item(double x, double y);

	virtual void on_kill();

	virtual void on_shorten_from_head(double len);

	virtual void on_shorten_from_tail(double len);


private:

	State state = State::off;

	double w = 3.0;
	double len = 100.0;
	double full_len = 100.0;
	bool grow_len = true;

	double half_on_w = 0.3;

	double w_scale = 1.0;
	double h_scale = 1.0;

	double collider_w = 1.0;
	double collider_interval = 1.0;
	int head_no_collider_count = 2;
	int tail_no_collider_count = 2;

	int turn_on_ani_time = 30;
	int turn_on_end_time = -1000;
	int turn_off_ani_time = 30;
	int turn_off_end_time = -1000;

	bool head_light = false;
	bool tail_light = false;
	double light_offset = 0.0;
	double light_rot = 0.0;
	double light_omega = 1.0;
	Sprite light_sprite_d;

	bool grazed = false;
	int last_graze_time = -10000;
	int graze_interval = 5;

	bool can_be_killed_by_player = false;

	bool boundary_test = true;
	double boundary_t = 20.0;
	double boundary_b = 20.0;
	double boundary_l = 20.0;
	double boundary_r = 20.0;

	bool killed = false;
	bool kill_when_bound_enemy_killed = true;
	bool destroy_when_off = true;
	bool default_shorten_from_head = true;

	Particle::DataSet kill_particle_data;

	double drop_interval = 15.0;

	PhysicObject* tail_physic = nullptr;

	Sound::Se* kill_se = nullptr;
	Sound::Se* on_se = nullptr;

	Sprite create_sprite_d;

	
	void update_collider(bool clear = false);

};

