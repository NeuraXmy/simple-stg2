#pragma once
#include "DrawSystem.h"
#include "SoundSystem.h" 


enum class BSColor
{
	black, red_dark, red, purple, pink, blue_dark, blue, sky_blue_dark, 
	sky_blue, green_dark, cyan, green, brown, yellow, orange, white, 
	count
};

enum class BSType
{
	star, round, round_ring,
	count
};

enum class BBColor
{
	black, red, purple, blue, sky_blue, green, yellow, gray, 
	count
};

enum class BBType
{
	star, round, 
	count
};

enum class ESColor
{
	blue, red, yellow, purple, count
};



struct tex_t final
{
	using ptex = Texture::Texture*;

	ptex bullet;
	ptex player;
	ptex enemy;
	ptex item;
	ptex boss_1;
	ptex sc_stripe;
	ptex magic_circle;
	ptex sc_title;
	ptex bomb_title;
	ptex bg_1;
	ptex bg_2;
	ptex bg_3;
	ptex fog;
	ptex panel;
	ptex panel_widget;
	ptex warning;

	void load();
};
extern tex_t tex;


struct coords_t final
{
	using pcoords = Texture::Coords*;

	pcoords player;
	pcoords hit_point;
	pcoords bullet_kill_particle;
	pcoords player_bullet_1;
	pcoords player_bullet_2;
	pcoords player_bullet_3;
	pcoords player_drone_1;
	pcoords player_drone_2;
	pcoords boss_1;
	pcoords sc_title;
	pcoords sc_stripe;
	pcoords magic_circle;
	pcoords bg_1;
	pcoords bg_2;
	pcoords bg_3;
	pcoords fog;
	pcoords panel;
	pcoords panel_line;
	pcoords panel_no_bomb;
	pcoords panel_bomb;
	pcoords panel_no_life;
	pcoords panel_life;
	pcoords boss_indicator;
	pcoords item_power;
	pcoords item_power_ind;
	pcoords item_power_big;
	pcoords item_power_big_ind;
	pcoords item_point;
	pcoords item_point_ind;
	pcoords item_full;
	pcoords item_full_ind;
	pcoords item_life;
	pcoords item_life_ind;
	pcoords item_bomb;
	pcoords item_bomb_ind;
	pcoords item_point_small;
	pcoords bomb_title;
	pcoords warning;

	pcoords bullet_small[(int)BSType::count][(int)BSColor::count];
	pcoords bullet_big[(int)BBType::count][(int)BBColor::count];
	pcoords laser1[(int)BSColor::count];
	pcoords laser2[(int)BSColor::count];
	pcoords laser_light[(int)BBColor::count];
	pcoords enemy_small[(int)ESColor::count];
	pcoords enemy_big;

	void load();
};
extern coords_t coords;


struct se_t final
{
	using pse = Sound::Se*;

	pse graze;
	pse player_death;
	pse player_shoot;
	pse player_extend_life;
	pse player_extend_bomb;
	pse player_power_up;
	pse player_full_power;
	pse item_pick;
	pse enemy_damage;
	pse enemy_damage_low;
	pse enemy_kill_small;
	pse enemy_kill_big;
	pse enemy_shoot_1;
	pse enemy_shoot_2;
	pse enemy_shoot_3;
	pse laser_1;
	pse charge;
	pse cast;
	pse timeout;

	void load();
};
extern se_t se;


struct bgm_t final
{
	using pbgm = Sound::Bgm*;

	pbgm bgm_1;

	void load();
};
extern bgm_t bgm;