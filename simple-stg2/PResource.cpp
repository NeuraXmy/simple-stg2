#include "PResource.h"

tex_t tex;
coords_t coords;
se_t se;
bgm_t bgm;


void tex_t::load()
{
	auto ds = DrawSystem::instance();

	bullet       = ds->load_texture("res/tex/bullet/bullets.png");
	player       = ds->load_texture("res/tex/player/player.png");
	enemy        = ds->load_texture("res/tex/enemy/enemy.png");
	boss_1       = ds->load_texture("res/tex/boss/boss_1.png");
	sc_title     = ds->load_texture("res/tex/boss/title.png");
	sc_stripe    = ds->load_texture("res/tex/boss/sc_stripe.png");
	magic_circle = ds->load_texture("res/tex/boss/magic_circle.png");
	bg_1         = ds->load_texture("res/tex/bg/bg_1.png");
	bg_2         = ds->load_texture("res/tex/bg/bg_2.png");
	bg_3		 = ds->load_texture("res/tex/bg/bg_3.png");
	panel        = ds->load_texture("res/tex/gui/panel.png");
	panel_widget = ds->load_texture("res/tex/gui/panel_widget.png");
	item         = ds->load_texture("res/tex/item/item.png");
	bomb_title   = ds->load_texture("res/tex/player/bomb_title.png");
	warning		 = ds->load_texture("res/tex/enemy/warning.png");
}

void coords_t::load()
{
	{
		auto ds = DrawSystem::instance();

		player               = ds->load_texcoords("res/tex/player/player.txc");
		hit_point            = ds->load_texcoords("res/tex/player/hit_point.txc");
		bullet_kill_particle = ds->load_texcoords("res/tex/bullet/bullet_kill_particle.txc");
		player_bullet_1      = ds->load_texcoords("res/tex/player/player_bullet_1.txc");
		player_bullet_2      = ds->load_texcoords("res/tex/player/player_bullet_2.txc");
		player_bullet_3      = ds->load_texcoords("res/tex/player/player_bullet_3.txc");
		player_drone_1       = ds->load_texcoords("res/tex/player/player_drone_1.txc");
		player_drone_2       = ds->load_texcoords("res/tex/player/player_drone_2.txc");
		boss_1               = ds->load_texcoords("res/tex/boss/boss_1.txc");
		sc_title             = ds->load_texcoords("res/tex/boss/title.txc");
		sc_stripe            = ds->load_texcoords("res/tex/boss/sc_stripe.txc");
		magic_circle         = ds->load_texcoords("res/tex/boss/magic_circle.txc");
		bg_1                 = ds->load_texcoords("res/tex/bg/bg_1.txc");
		bg_2                 = ds->load_texcoords("res/tex/bg/bg_2.txc");
		bg_3			     = ds->load_texcoords("res/tex/bg/bg_3.txc");
		panel                = ds->load_texcoords("res/tex/gui/panel.txc");
		panel_line           = ds->load_texcoords("res/tex/gui/line.txc");
		panel_bomb           = ds->load_texcoords("res/tex/gui/bomb.txc");
		panel_no_bomb        = ds->load_texcoords("res/tex/gui/no_bomb.txc");
		panel_life           = ds->load_texcoords("res/tex/gui/life.txc");
		panel_no_life        = ds->load_texcoords("res/tex/gui/no_life.txc");
		boss_indicator       = ds->load_texcoords("res/tex/gui/boss_indicator.txc");
		item_power           = ds->load_texcoords("res/tex/item/power.txc");
		item_power_ind       = ds->load_texcoords("res/tex/item/power_ind.txc");
		item_power_big       = ds->load_texcoords("res/tex/item/power_big.txc");
		item_power_big_ind   = ds->load_texcoords("res/tex/item/power_big_ind.txc");
		item_point           = ds->load_texcoords("res/tex/item/point.txc");
		item_point_ind       = ds->load_texcoords("res/tex/item/point_ind.txc");
		item_full            = ds->load_texcoords("res/tex/item/full.txc");
		item_full_ind        = ds->load_texcoords("res/tex/item/full_ind.txc");
		item_life            = ds->load_texcoords("res/tex/item/life.txc");
		item_life_ind        = ds->load_texcoords("res/tex/item/life_ind.txc");
		item_bomb            = ds->load_texcoords("res/tex/item/bomb.txc");
		item_bomb_ind        = ds->load_texcoords("res/tex/item/bomb_ind.txc");
		item_point_small     = ds->load_texcoords("res/tex/item/point_small.txc");
		bomb_title           = ds->load_texcoords("res/tex/player/bomb_title.txc");
		enemy_big			 = ds->load_texcoords("res/tex/enemy/big_enemy.txc");
		warning				 = ds->load_texcoords("res/tex/enemy/warning.txc");

		ds->load_texcoords_series(bullet_small[(int)BSType::star],  (int)BSColor::count, "res/tex/bullet/small_star.txc");
		ds->load_texcoords_series(bullet_small[(int)BSType::round], (int)BSColor::count, "res/tex/bullet/small_round.txc");
		ds->load_texcoords_series(bullet_small[(int)BSType::round_ring], (int)BSColor::count, "res/tex/bullet/small_round_ring.txc");
		ds->load_texcoords_series(bullet_big[(int)BBType::star],	(int)BBColor::count, "res/tex/bullet/big_star.txc");
		ds->load_texcoords_series(laser_light, (int)BBColor::count, "res/tex/bullet/laser_light.txc");
		ds->load_texcoords_series(laser1, (int)BSColor::count, "res/tex/bullet/laser1.txc");
		ds->load_texcoords_series(enemy_small, (int)ESColor::count, "res/tex/enemy/small_enemy.txc");
	}
}

void se_t::load()
{
	auto ss = SoundSystem::instance();

	graze                 = ss->load_se("res/se/graze.wav");				
	player_death          = ss->load_se("res/se/player_death.wav");
	player_shoot          = ss->load_se("res/se/player_shoot.wav");
	player_extend_life    = ss->load_se("res/se/player_extend_life.wav");
	player_extend_bomb    = ss->load_se("res/se/player_extend_bomb.wav");
	player_full_power     = ss->load_se("res/se/player_full_power.wav");
	item_pick             = ss->load_se("res/se/item_pick.wav");
	enemy_damage          = ss->load_se("res/se/enemy_damage.wav");
	enemy_damage_low      = ss->load_se("res/se/enemy_damage_low.wav");
	enemy_kill_small      = ss->load_se("res/se/enemy_kill_small.wav");
	enemy_kill_big        = ss->load_se("res/se/enemy_kill_big.wav");
	player_power_up       = ss->load_se("res/se/player_power_up.wav");
	enemy_shoot_1         = ss->load_se("res/se/enemy_shoot_1.wav");
	enemy_shoot_2         = ss->load_se("res/se/enemy_shoot_2.wav");
	enemy_shoot_3         = ss->load_se("res/se/enemy_shoot_3.wav");
	laser_1               = ss->load_se("res/se/laser_1.wav");
	charge                = ss->load_se("res/se/charge.wav");
	cast                  = ss->load_se("res/se/cast.wav");
	timeout				  = ss->load_se("res/se/timeout.wav");

	graze->set_vol(3.0);
	player_death->set_vol(2.0);
	enemy_damage->set_vol(2.0),		enemy_damage->set_play_interval(5);
	enemy_damage_low->set_vol(3.0), enemy_damage_low->set_play_interval(5);
	enemy_kill_big->set_vol(1.5);
	player_power_up->set_vol(2.0);
	player_shoot->set_vol(0.4);
	enemy_shoot_2->set_vol(1.5);
	enemy_shoot_3->set_vol(1.5);
	player_full_power->set_vol(2.0);
	player_extend_bomb->set_vol(2.0);
	player_extend_life->set_vol(2.0);
	laser_1->set_vol(1.5);
	charge->set_vol(3.0);
	cast->set_vol(2.0);
	timeout->set_vol(2.0);
}

void bgm_t::load()
{
	auto ss = SoundSystem::instance();

	bgm_1 = ss->load_bgm("res/bgm/bgm_1.wav", "res/bgm/bgm_1.inf");
}