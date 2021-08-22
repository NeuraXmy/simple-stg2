#pragma once

#include <random>
#include <vector>

#include "ObjectSystem.h"

#include "Player.h"
#include "Stage.h"
#include "BossAttack.h"

class GamePanel;


class GameSystem final
{
public:

	static constexpr int game_w = 385;
	static constexpr int game_h = 450;
	static constexpr int center_w = game_w / 2;
	static constexpr int center_h = game_h / 2;
	static const int ui_w = 640;
	static const int ui_h = 480;
	static const int border_w = (ui_h - game_h) / 2;


	static GameSystem* instance();

	static void release_instance();


	int t();

	void start();
	
	void update();

	void pause();

	void resume();

	bool is_paused() const;


	void draw();


	template<class StageType, class ... Args>
	StageType* append_stage(Args ... args) 
	{
		auto p = ObjectSystem::instance()->create_object<StageType>(args...);
		stages.push_back(p);
		return p;
	}

	void clear_stages();

	void set_current_stage_index(int index);

	int get_current_stage_index() const;

	Stage* get_stage(int index) const;

	Stage* get_current_stage() const;

	void start_from_current_stage();

	void start_from_stage(int index);


	void clear_all_enemy_bullets(bool drop);


	void set_rand_seed(unsigned int seed);

	int rand_int(int l, int r);

	double rand_real(double l, double r);


	template<class PlayerType, class ... Args>
	PlayerType* set_player(Args ... args)
	{
		if (player)
		{
			remove_player();
			player = nullptr;
		}
		auto p = ObjectSystem::instance()->create_object<PlayerType>(args...);
		player = p;
		return p;
	}

	void remove_player();

	Player* get_player() const;

	bool has_player() const;


	BossAttack* get_current_boss_attack() const;

	void set_current_boss_attack(BossAttack* attack);

	
	template<class GamePanelType, class ... Args>
	GamePanelType* set_game_panel(Args ... args)
	{
		remove_game_panel();
		auto p = new GamePanelType(args...);
		game_panel = p;
		return p;
	}

	void remove_game_panel();

	GamePanel* get_game_panel() const;


	void pick_all_item();

	template <class ItemType, class ... Args>
	void drop_item(PhysicObject* obj, const utils::Range& r, const utils::IntRange& count, Args ... args)
	{
		int cnt = count.get_game();
		for (int i = 0; i < cnt; i++)
		{
			double radius = r.get_game();
			double theta = r.get_game();
			ObjectSystem::instance()->create_object<ItemType>(args...)
				->set_pos(obj->x() + radius * cos(theta), obj->y() + radius * sin(theta));
		}
	}

	template <class ItemType, class ... Args>
	void drop_item(double x, double y, const utils::Range& r, const utils::IntRange& count, Args ... args)
	{
		int cnt = count.get_game();
		for (int i = 0; i < cnt; i++)
		{
			double radius = r.get_game();
			double theta = r.get_game();
			ObjectSystem::instance()->create_object<ItemType>(args...)
				->set_pos(x + radius * cos(theta), y + radius * sin(theta));
		}
	}


	void set_shake_enabled(bool flag);

	void set_shake_effect(double strength, double weaking_speed = 0.05);

	
private:

	bool paused;

	int update_t;

	std::default_random_engine rand_engine;

	std::vector<Stage*> stages;

	int current_stage_i;

	Player* player;

	BossAttack* boss_attack;

	GamePanel* game_panel;

	bool shake_enabled;
	double shake_strength;
	double shake_weaking_speed;


	GameSystem();

	~GameSystem();

};

