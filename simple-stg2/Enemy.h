#pragma once

#include "ActorObject.h"
#include "CollideCircle.h"
#include "ParticleSystem.h"

#include "Shooter.h"

namespace Sound { class Se; }


class Enemy : public ActorObject, public CollideCircle
{
public:

	Enemy();

	virtual ~Enemy() = default;


	int get_max_health() const;

	void set_max_health(int hp, bool recover = true);

	int get_health() const;

	void set_health(int hp);

	void damage_health(int damage);

	int get_last_be_attacked_time() const;

	bool is_in_low_health() const;


	bool is_boundary_test() const;

	void set_boundary_test(bool flag);

	void set_boundary(double l, double r, double t, double b);


	bool is_invincible() const;

	void set_invincible(bool flag, int len = -1);

	void set_invincible_cancel_time(int time);


	bool is_can_hit_player() const;

	void set_can_hit_player(bool flag);


	void kill(bool drop_item = true);

	void kill_raw();

	bool is_killed();

	void set_still_frame_index(int index);

	void set_still_from_left_frame_index(int index);

	void set_still_from_right_frame_index(int index);

	void set_left_move_frame_index(int index);

	void set_right_move_frame_index(int index);

	
	Particle::DataSet& kill_particle();

	const Particle::DataSet& kill_particle() const;

	
	template<class ShooterType, class ... Args>
	void append_shooter(Args ... args)
	{
		auto s = ObjectSystem::instance()->create_object<ShooterType>(args ...);
		shooters.push_back(s);
		s->bind(this);
	}

	void start_all_shooter();

	void stop_all_shooter();

	void clear_all_shooter();


	Sound::Se* get_kill_se() const;

	void set_kill_se(Sound::Se* se);

	Sound::Se* get_damage_se() const;

	void set_damage_se(Sound::Se* se);

	Sound::Se* get_damage_low_se() const;

	void set_damage_low_se(Sound::Se* se);
	

protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_change_act_state(int state) override;

	virtual void on_collide(PhysicObject* obj) override;


	virtual void on_kill();

	virtual void on_attacked();

	virtual void on_low_health();

	virtual void on_drop_item();


private:

	enum class ActState
	{
		still,
		still_from_left,
		still_from_right,
		left,
		right,
		act_state_num
	};

	double last_x = 0.0;
	double last_y = 0.0;

	bool invincible = false;
	int invicible_cancel_time = -10000;

	int health = 1;
	int max_health = 1;

	bool can_hit_player = true;

	bool boundary_test = true;
	double boundary_t = 50.0;
	double boundary_b = 50.0;
	double boundary_l = 50.0;
	double boundary_r = 50.0;

	int last_be_attacked_time = -100;

	bool killed = false;

	Particle::DataSet kill_particle_data;

	std::vector<Shooter*> shooters;

	Sound::Se* kill_se = nullptr;
	Sound::Se* damage_se = nullptr;
	Sound::Se* damage_low_se = nullptr;

};


