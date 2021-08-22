#pragma once

#include "ActorObject.h"
#include "InputListener.h"
#include "CollideCircle.h"
#include "DroneSet.h"
#include "Bomb.h"

class DeathWave;
class EnemyBullet;
class Laser;
namespace Sound { class Se; }

class Player : public ActorObject, public InputListener, public CollideCircle
{
public:

	Player();

	virtual ~Player() = default;


	class HitPoint : public SpriteObject
	{
		static inline const double rotating_speed = 0.04;
		static inline const int ani_time = 4;
		static inline const double scale_k = 1.0;
	public:
		void show();
		void hide();
	private:
		void on_create();
		void on_update();
		void on_draw();
		int start_show_time = 0;
	};

	class GrazeCircle : public CollideCircle, public PhysicObject
	{
		Player* p;
	public:
		GrazeCircle(Player* player);
	private:
		void on_collide(PhysicObject* obj) override;
	};


	void set_boundary(double l, double r, double t, double b);


	bool is_slowmode();

	void set_slowmode(bool flag);


	double get_speed() const;

	void set_speed(double speed);

	double get_slowmode_speed() const;

	void set_slowmode_speed(double speed);

	
	bool is_invincible() const;
	
	void set_invincible(bool flag, int len = -1);

	void set_invincible_cancel_time(int time);


	bool is_lock_move() const;

	void set_lock_move(bool flag);

	bool is_lock_attack() const;

	void set_lock_attack(bool flag);

	bool is_lock_bomb() const;

	void set_lock_bomb(bool flag);


	void kill();

	void kill_raw();

	void rescue();

	bool is_killed() const;

	int get_kill_bomb_time() const;

	void set_kill_bomb_time(int time);

	bool is_in_kill_bomb_time() const;


	void set_still_frame_index(int index);

	void set_still_from_left_frame_index(int index);

	void set_still_from_right_frame_index(int index);

	void set_left_move_frame_index(int index);

	void set_right_move_frame_index(int index);


	DeathWave* get_death_wave() const;

	void set_death_wave(DeathWave* wave);

	HitPoint* hit_point() const;


	template<class MainGun, class ... Args>
	void set_main_gun(Args ... args) 
	{
		remove_main_gun();
		main_gun_obj = ObjectSystem::instance()->create_object<MainGun>(args...);
		main_gun_obj->bind(this);
	}

	void remove_main_gun();

	Shooter* main_gun() const;


	template<class DroneSet, class ... Args>
	void set_drone_set_fast(Args ... args) 
	{
		remove_drone_set_fast();
		drone_set_fast_obj = ObjectSystem::instance()->create_object<DroneSet>(args...);
		drone_set_fast_obj->bind(this);
	}

	void remove_drone_set_fast();

	DroneSet* drone_set_fast() const;


	template<class DroneSet, class ... Args>
	void set_drone_set_slow(Args ... args)
	{
		remove_drone_set_slow();
		drone_set_slow_obj = ObjectSystem::instance()->create_object<DroneSet>(args...);
		drone_set_slow_obj->bind(this);
	}

	void remove_drone_set_slow();

	DroneSet* drone_set_slow() const;


	int get_power() const;

	void set_power(int power);

	void add_power(int power);

	int get_max_power() const;

	void set_max_power(int power);

	bool is_attacking() const;

	bool is_full_power() const;


	int get_life_count() const;

	void set_life_count(int life);

	void extend_life();

	int get_life_limit() const;

	void set_life_limit(int limit);


	int get_bomb_count() const;

	void set_bomb_count(int bomb);

	void extend_bomb();

	int get_bomb_limit() const;

	void set_bomb_limit(int limit);

	int get_bomb_initial_count() const;

	void set_bomb_initial_count(int bomb);

	void bomb();

	template <class BombType, class ... Args>
	void set_fast_bomb(Args ... args)
	{
		remove_fast_bomb();
		fast_bomb = ObjectSystem::instance()->create_object<BombType>(args...);
		fast_bomb->set_player(this);
	}

	Bomb* get_fast_bomb() const;

	void remove_fast_bomb();

	template <class BombType, class ... Args>
	void set_slow_bomb(Args ... args)
	{
		remove_slow_bomb();
		slow_bomb = ObjectSystem::instance()->create_object<BombType>(args...);
		slow_bomb->set_player(this);
	}

	Bomb* get_slow_bomb() const;

	void remove_slow_bomb();

	bool is_bombing() const;


	long long get_score() const;

	void set_score(long long score);

	void add_score(long long score);


	int get_point_score() const;

	void set_point_score(int score);

	void add_point_score(int score);

	int get_point_score_limit() const;

	void set_point_score_limit(int limit);


	int get_graze() const;

	void set_graze(int graze);

	void graze_bullet(EnemyBullet* b);

	void graze_laser(Laser* l);

	void set_graze_circle_r(double r);

	double get_graze_circle_r() const;


	double get_item_pick_line() const;

	void set_item_pick_line(double y);

	void show_item_pick_line();


	Particle::DataSet& graze_particle();

	const Particle::DataSet& graze_particle() const;

	Particle::DataSet& death_particle();

	const Particle::DataSet& death_particle() const;


	bool is_cheat_mode() const;

	void set_cheat_mode(bool flag);


	Sound::Se* get_graze_se() const;

	void set_graze_se(Sound::Se* se);

	Sound::Se* get_death_se() const;

	void set_death_se(Sound::Se* se);

	Sound::Se* get_extend_life_se() const;

	void set_extend_life_se(Sound::Se* se);

	Sound::Se* get_extend_bomb_se() const;

	void set_extend_bomb_se(Sound::Se* se);

	Sound::Se* get_power_up_se() const;

	void set_power_up_se(Sound::Se* se);

	void set_power_up_interval(int interval);

	Sound::Se* get_full_power_se() const;

	void set_full_power_se(Sound::Se* se);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_draw_ui() override;

	virtual void on_change_act_state(int state) override;

	virtual void on_collide(PhysicObject* obj) override;


	virtual void on_kill();

	virtual void on_rescue();

	virtual void on_slow_mode();

	virtual void on_fast_mode();

	virtual void on_move_left();

	virtual void on_move_right();

	virtual void on_move_up();

	virtual void on_move_down();

	virtual void on_stand_still();

	virtual void on_change_power();

	virtual void on_attack();

	virtual void on_silent();

	virtual void on_bomb();

	virtual void on_graze(EnemyBullet* b);

	virtual void on_graze(Laser* l);


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

	int power = 0;
	int max_power = 400;
	int last_full_power_time = -10000;

	double boundary_t = -10.0;
	double boundary_b = -10.0;
	double boundary_l = -10.0;
	double boundary_r = -10.0;

	bool slowmode = false;

	double speed = 4.5;
	double slowmode_speed = 2.0;

	bool killed = false;
	int last_killed_time = -10000;
	int killed_bomb_time = 10;
	int killed_ani_time = 120;
	int killed_invincible_time = 240;

	bool move_left = false;
	bool move_right = false;
	bool move_up = false;
	bool move_down = false;

	bool attacking = false;

	bool invincible = false;
	int invicible_cancel_time = -10000;

	bool lock_move = false;
	bool lock_attack = false;
	bool lock_bomb = false;

	int bomb_count = 3;
	int bomb_limit = 7;
	int bomb_initial_count = 3;
	int last_extend_bomb_time = -10000;
	
	int life_count = 2;
	int life_limit = 7;
	int last_extend_life_time = -10000;

	int point_score = 1000;
	int point_score_limit = 100000;

	long long score = 0;

	int graze = 0;
	double graze_circle_r = 20.0;

	double item_pick_line = 120;
	int last_show_item_pick_line_time = -10000;

	HitPoint* hit_point_obj = nullptr;

	Shooter* main_gun_obj = nullptr;
	DroneSet* drone_set_fast_obj = nullptr;
	DroneSet* drone_set_slow_obj = nullptr;

	DeathWave* death_wave = nullptr;

	GrazeCircle* graze_circle = nullptr;

	Particle::DataSet graze_particle_data;
	Particle::DataSet death_particle_data;

	Bomb* fast_bomb = nullptr;
	Bomb* slow_bomb = nullptr;

	Sound::Se* extend_life_se = nullptr;
	Sound::Se* extend_bomb_se = nullptr;
	Sound::Se* death_se = nullptr;
	Sound::Se* graze_se = nullptr;
	Sound::Se* power_up_se = nullptr;
	int power_up_interval = 100;
	Sound::Se* full_power_se = nullptr;

	bool cheat_mode = false;


	void on_key_press(Key key) override;

	void on_key_release(Key key) override;

};

