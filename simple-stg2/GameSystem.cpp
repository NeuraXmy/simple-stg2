#include "GameSystem.h"
#include "DrawSystem.h"
#include "ObjectSystem.h"
#include "CollideSystem.h"
#include "ParticleSystem.h"
#include "Settings.h"

#include "EnemyBullet.h"
#include "GamePanel.h"
#include "Item.h"
#include "Laser.h"


namespace
{
	GameSystem* instance = nullptr;
}

GameSystem* GameSystem::instance()
{
	if (!::instance)
	{
		::instance = new GameSystem;
	}
	return ::instance;
}

void GameSystem::release_instance()
{
	if (::instance)
	{
		delete ::instance;
	}
	::instance = nullptr;
}


GameSystem::GameSystem() :
	paused(true),
	update_t(),
	player(nullptr),
	current_stage_i(),
	boss_attack(nullptr),
	game_panel(nullptr),
	shake_enabled(true),
	shake_strength(0.0),
	shake_weaking_speed(0.05)
{
	ObjectSystem::instance();
	CollideSystem::instance();
	ParticleSystem::instance()->set_pool_size(settings::particle_pool_size);
}

GameSystem::~GameSystem()
{
	clear_stages();

	remove_game_panel();

	ObjectSystem::release_instance();
	CollideSystem::release_instance();
	ParticleSystem::release_instance();
}


int GameSystem::t()
{
	return update_t;
}

void GameSystem::start()
{
	update_t = 0;
	paused = false;
}

void GameSystem::update()
{
	if (paused)
	{
		return;
	}
	
	ObjectSystem::instance()->update();
	CollideSystem::instance()->update();
	ParticleSystem::instance()->update();

	if (auto s = get_current_stage())
	{
		if (s->is_ended())
		{
			current_stage_i++;

			if (get_current_stage())
			{
				get_current_stage()->start();
			}
		}
	}
	
	update_t++;
}

void GameSystem::pause()
{
	paused = true;
}

void GameSystem::resume()
{
	paused = false;
}

bool GameSystem::is_paused() const
{
	return paused;
}


void GameSystem::draw()
{
	// panel
	{
		DrawSystem::instance()->set_view_to_window();

		if (game_panel)
		{
			game_panel->draw();
		}

		DrawSystem::instance()->flush();
	}

	// objects
	{
		double shake_x = 0.0;
		double shake_y = 0.0;
		if (shake_enabled)
		{
			int t = update_t;
			shake_x = shake_strength * sin(t / 60.0 * utils::pi * 2.0 * 15);
			shake_y = shake_strength * cos(t / 60.0 * utils::pi * 2.0 * 14) * 0.8;
			if (shake_strength > 1e-5)
				shake_strength -= shake_weaking_speed;
			if (shake_strength <= 1e-5)
				shake_strength = 0.0;
		}

		DrawSystem::instance()->set_view_to_game(shake_x, shake_y);

		ObjectSystem::instance()->draw();
		ParticleSystem::instance()->draw();

		DrawSystem::instance()->flush();
	}

	// objects' ui
	{
		DrawSystem::instance()->set_view_to_game(0, 0);

		ObjectSystem::instance()->draw_ui();

		DrawSystem::instance()->flush();
	}

}

void GameSystem::clear_stages()
{
	if (get_current_stage()->is_started())
	{
		get_current_stage()->end();
	}
	for (auto stage : stages)
	{
		stage->destroy();
	}
	stages.clear();
}

void GameSystem::set_current_stage_index(int index)
{
	current_stage_i = index;
}

int GameSystem::get_current_stage_index() const
{
	return current_stage_i;
}

Stage* GameSystem::get_stage(int index) const
{
	return index >= 0 && index < stages.size() ? stages[index] : nullptr;
}

Stage* GameSystem::get_current_stage() const
{
	return get_stage(current_stage_i);
}

void GameSystem::start_from_current_stage()
{
	if (get_current_stage())
	{
		get_current_stage()->start();
	}
}

void GameSystem::start_from_stage(int index)
{
	if (get_current_stage())
	{
		get_current_stage()->end();
	}
	if (get_stage(index))
	{
		get_stage(index)->start();
	}
}

void GameSystem::clear_all_enemy_bullets(bool drop)
{
	ObjectSystem::instance()->foreach<EnemyBullet>([drop](EnemyBullet* b) { b->kill(drop);  });
	ObjectSystem::instance()->foreach<Laser>([drop](Laser* b) { b->kill(drop);  });
}


void GameSystem::remove_player()
{
	if (player)
	{
		player->destroy();
		player = nullptr;
	}
}

Player* GameSystem::get_player() const
{
	return player;
}

bool GameSystem::has_player() const
{
	return player;
}

BossAttack* GameSystem::get_current_boss_attack() const
{
	return boss_attack;
}

void GameSystem::set_current_boss_attack(BossAttack* attack)
{
	boss_attack = attack;
}

void GameSystem::remove_game_panel()
{
	if (game_panel)
	{
		delete game_panel;
		game_panel = nullptr;
	}
}

GamePanel* GameSystem::get_game_panel() const
{
	return game_panel;
}

void GameSystem::pick_all_item()
{
	if (auto p = get_player())
	{
		ObjectSystem::instance()->foreach<Item>([p](Item* item) {
			item->pick(p);
		});
	}
}


void GameSystem::set_rand_seed(unsigned int seed)
{
	rand_engine.seed(seed);
}

int GameSystem::rand_int(int l, int r)
{
	if (l > r)
		std::swap(l, r);
	std::uniform_int_distribution<int> d(l, r);
	return d(rand_engine);
}

double GameSystem::rand_real(double l, double r)
{
	if (l > r) 
		std::swap(l, r);
	std::uniform_real_distribution<double> d(l, r);
	return d(rand_engine);
}



void GameSystem::set_shake_enabled(bool flag)
{
	shake_enabled = flag;
}

void GameSystem::set_shake_effect(double strength, double weaking_speed)
{
	shake_strength = strength;
	shake_weaking_speed = weaking_speed;
}
