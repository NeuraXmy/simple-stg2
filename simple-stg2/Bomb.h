#pragma once

#include "PhysicObject.h"
#include "Sprite.h"
#include "Background.h"
#include "Shooter.h"

class Player;
namespace Sound { class Se;  }


class Bomb : public PhysicObject
{
public:

	virtual ~Bomb() = default;


	Player* get_player() const;

	void set_player(Player* player);


	std::wstring get_title() const;

	void set_title(const std::wstring& title);

	std::wstring get_subtitle() const;

	void set_subtitle(const std::wstring& subtitle);

	
	int get_len() const;

	void set_len(int len);

	int get_invincible_time() const;

	void set_invincible_time(int time);

	int get_pick_item_time() const;

	void set_pick_item_time(int time);


	int from_start_t() const;

	void start();

	void end();

	bool is_casting() const;

	bool is_ended() const;


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


	Sprite& title_sprite();

	const Sprite& title_sprite() const;


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



protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_draw_ui() override;


	virtual void on_start();

	virtual void on_casting();

	virtual void on_end();


private:

	std::wstring title = L"Unknown";
	std::wstring subtitle = L"Unknown";

	int start_time = -1000;
	int len = 0;

	bool casting = false;

	int invincible_time = 0;
	int pick_item_time = 0;

	bool kill_bullet_when_end = true;

	bool charge_before_start = false;

	std::vector<Shooter*> shooters;

	Sprite title_sprite_d;

	Background* bg = nullptr;

	Player* player = nullptr;

	Sound::Se* start_se = nullptr;

};

