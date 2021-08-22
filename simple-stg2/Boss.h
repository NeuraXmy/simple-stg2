#pragma once

#include "ActorObject.h"
#include "ParticleSystem.h"
#include "CollideCircle.h"
#include "BossAttack.h"
#include "Movement.h"

namespace Sound { class Se;  }


class Boss : public ActorObject, public CollideCircle
{
public:

	Boss();

	virtual ~Boss() = default;


	void damage_health(int damage);

	int get_last_be_attacked_time() const;

	bool is_in_low_health() const;


	bool is_invincible() const;

	void set_invincible(bool flag, int len = -1);

	void set_invincible_cancel_time(int time);


	bool is_can_hit_player() const;

	void set_can_hit_player(bool flag);


	void kill();

	void kill_raw();

	bool is_killed();

	Particle::DataSet& kill_particle();

	const Particle::DataSet& kill_particle() const;

	Particle::DataSet& kill_ani_particle();

	const Particle::DataSet& kill_ani_particle() const;

	int get_kill_ani_time() const;

	void set_kill_ani_time(int time);

	
	void charge();

	Particle::DataSet& charge_particle();

	const Particle::DataSet& charge_particle() const;


	void set_casting(bool flag, int len = -1);

	bool is_casting() const;


	void set_still_frame_index(int index);

	void set_still_from_left_frame_index(int index);

	void set_still_from_right_frame_index(int index);

	void set_left_move_frame_index(int index);

	void set_right_move_frame_index(int index);

	void set_casting_frame_index(int index);
	

	template <class Attack, class ... Args>
	void append_attack(Args ... args)
	{
		auto a = ObjectSystem::instance()->create_object<Attack>(args...);
		a->bind(this);
		a->set_boss(this);
		attacks.push_back(a);
	}

	void clear_attacks();
	
	BossAttack* get_current_attack() const;

	std::vector<BossAttack*> get_attacks();

	const std::vector<BossAttack*> get_attacks() const;

	int get_attack_count() const;

	int get_sc_attack_count() const;

	void start_attacks(int index = 0);

	bool is_kill_when_attacks_end() const;

	void set_kill_when_attacks_end(bool flag);

	int get_attack_interval() const;

	void set_attack_interval(int interval);


	Sprite& magic_circle();

	const Sprite& magic_circle() const;

	void set_magic_circle(bool enable);


	double get_game_center_x() const;

	double get_game_center_y() const;

	void set_game_center(double x, double y);

	void move_to_center(int time, move::Factor factor = move::factor::linear);

	void move_randomly(
		int time, double probability_to_player,
		utils::Range dx, utils::Range dy,
		utils::Range x_limit, utils::Range y_limit,
		move::Factor factor = move::factor::linear
	);


	bool is_attack_ended() const;


	Sound::Se* get_kill_se() const;

	void set_kill_se(Sound::Se* se);

	Sound::Se* get_damage_se() const;

	void set_damage_se(Sound::Se* se);

	Sound::Se* get_damage_low_se() const;

	void set_damage_low_se(Sound::Se* se);

	Sound::Se* get_charge_se() const;

	void set_charge_se(Sound::Se* se);
	

protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_change_act_state(int state) override;

	virtual void on_collide(PhysicObject * obj) override;


	virtual void on_kill();

	virtual void on_attacked();

	virtual void on_low_health();

	virtual void on_attack_end();


private:

	enum class ActState
	{
		still,
		still_from_left,
		still_from_right,
		left,
		right,
		casting,
		act_state_num
	};

	double last_x = 0.0;
	double last_y = 0.0;

	bool invincible = false;
	int invicible_cancel_time = -10000;

	bool can_hit_player = true;

	int last_be_attacked_time = -100;

	bool killed = false;
	int killed_time = -1000;
	int kill_ani_time = 60;
	int kill_particle_interval = 2;

	bool kill_when_attacks_end = true;

	Particle::DataSet kill_particle_data;

	Particle::DataSet kill_ani_particle_data;

	Particle::DataSet charge_particle_data;

	bool casting = false;
	int cast_cancel_time = -1000;

	std::vector<BossAttack*> attacks = {};
	int current_attack = -1;
	int attack_start_interval = 30;
	int attack_start_time = -1000;

	Sprite magic_circle_d;
	bool magic_circle_enabled = false;
	int magic_circle_enabled_time = -1000;

	double center_x = 0.0;
	double center_y = 0.0;

	double move_s_x = 0.0;
	double move_s_y = 0.0;
	double move_t_x = 0.0;
	double move_t_y = 0.0;
	int move_start_time = 0;
	int move_end_time = -10000;
	move::Factor move_factor = move::factor::linear;

	Sound::Se* kill_se = nullptr;
	Sound::Se* damage_se = nullptr;
	Sound::Se* damage_low_se = nullptr;
	Sound::Se* charge_se = nullptr;

};

