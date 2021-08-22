#pragma once

#include <string>

#include "PhysicObject.h"
#include "ObjectSystem.h"
#include "Sprite.h"
#include "Background.h"

class Boss;
class Shooter;

namespace Sound { class Se; }


class BossAttack : public PhysicObject
{
public:

	enum class State
	{
		ready,
		preparing,
		casting,
		beaten,
		failed_timeout,
		failed_lost
	};


	virtual ~BossAttack() = default;


	void set_boss(Boss* boss);

	Boss* get_boss() const;

	
	std::wstring get_title() const;

	void set_title(const std::wstring& title);

	std::wstring get_subtitle() const;

	void set_subtitle(const std::wstring& subtitle);


	bool is_spellcard() const;

	void set_is_spellcard(bool flag);


	bool is_time_spellcard() const;

	void set_is_time_spellcard(bool flag);

	int get_time_limit() const;

	void set_time_limit(int time);

	int get_from_start_time() const;


	int get_max_health() const;

	void set_max_health(int hp, bool recover = true);

	int get_health() const;

	void set_health(int hp);

	void damage_health(int damage);

	int get_last_be_attacked_time() const;


	int get_max_score() const;

	void set_max_score(int score, bool recover = true);

	int get_score() const;

	void set_score(int score);


	bool is_preparing() const;

	int get_prepare_time() const;

	void set_prepare_time(int time);


	void start();

	void end(State state);
	
	State get_state() const;

	bool is_casting() const;

	bool is_ended() const;

	int get_start_time() const;

	int get_end_time() const;


	void set_player_bombed(bool flag);

	bool is_player_bombed() const;

	void set_player_killed(bool flag);

	bool is_player_killed() const;


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


	bool is_kill_bullet_when_end() const;

	void set_kill_bullet_when_end(bool flag);

	
	Sprite& title_sprite();

	const Sprite& title_sprite() const;

	Sprite& spellcard_stripe();

	const Sprite& spellcard_stripe() const;


	bool is_charge_before_start() const;

	void set_charge_before_start(bool flag);


	template<class Bg, class ... Args>
	Background* set_background(Args ... args)
	{
		remove_background();
		bg = new Bg(args...);
		return bg;
	}

	Background* background() const;

	void remove_background();


	Sound::Se* get_start_se() const;

	void set_start_se(Sound::Se* se);

	Sound::Se* get_beaten_se() const;

	void set_beaten_se(Sound::Se* se);

	Sound::Se* get_failed_se() const;

	void set_failed_se(Sound::Se* se);

	Sound::Se* get_timeout_se() const;

	void set_timeout_se(Sound::Se* se);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_draw_ui() override;


	virtual void on_attacked();

	virtual void on_prepare();
	
	virtual void on_start();

	virtual void on_casting();

	virtual void on_end();

	virtual void on_beaten();

	virtual void on_failed_lost();

	virtual void on_failed_timeout();

	virtual void on_drop_item();


private:

	std::wstring title		= L"Unknown";
	std::wstring subtitle	= L"Unknown";

	bool is_sc = false;
	bool is_time_sc = false;

	int max_health = 1000;
	int health = 1000;
	int display_health = 1000;
	int last_be_attacked_time = -1000;

	bool player_bombed = false;
	bool player_killed = false;

	Boss* boss = nullptr;

	int time_limit = 360;

	int max_score = 0;
	int score = 0;

	State state = State::ready;
	int prepare_time = 60;
	int start_time = -1000;
	int end_time = -1000;

	bool kill_bullet_when_end = true;

	bool charge_before_start = false;

	std::vector<Shooter*> shooters;

	Sprite title_sprite_d;
	Sprite spellcard_stripe_d;

	Background* bg = nullptr;

	Sound::Se* start_se = nullptr;
	Sound::Se* beaten_se = nullptr;
	Sound::Se* failed_se = nullptr;
	Sound::Se* timeout_se = nullptr;

};

