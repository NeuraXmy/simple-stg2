#include "PEnemy.h"
#include "GameSystem.h"
#include "PItem.h"

Color es_color[] = 
{
	{0.4, 0.4, 0.8},
	{0.8, 0.4, 0.4},
	{0.9, 0.9, 0.4},
	{0.7, 0.4, 0.7}
};

void ESPrefab::on_create()
{
	Enemy::on_create();

	sprite().set_tex_and_coords(tex.enemy, coords.enemy_small[(int)color]);

	set_still_frame_index(0);
	set_left_move_frame_index(1);
	set_right_move_frame_index(2);
	set_still_from_left_frame_index(3);
	set_still_from_right_frame_index(4);

	set_collider_r(10.0);
	set_max_health(60);

	set_kill_se(se.enemy_kill_small);
	set_damage_se(se.enemy_damage);
	set_damage_low_se(se.enemy_damage_low);

	{
		kill_particle().emplace_back();
		auto& p = this->kill_particle().back();
		p.lifeTime = { 15, 60 }, p.cnt = 10;
		p.scale = { 2.0, 3.0 }, p.rand_pos_r = 5.0;
		p.rand_v_direction = true, p.v = { 3.0, 5.0 };
		p.omega = { -0.5, 0.5 }, p.rot = { 0.0, utils::pi * 2.0 };
		p.set_option(Particle::Options::fade_out);
		p.set_option(Particle::Options::shrink);
		p.set_option(Particle::Options::slow_down);
		p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
		p.sprite.set_color(es_color[(int)color]);
		p.sprite.set_level(DrawLevel::enemy);
	}
}

void ESPrefab::on_drop_item()
{
	Enemy::on_drop_item();
	GameSystem::instance()->drop_item<PowerItem>(this, { 5.0, 10.0 }, drop_power);
	GameSystem::instance()->drop_item<PointItem>(this, { 5.0, 10.0 }, drop_point);
}

void EBPrefab::on_create()
{
	Enemy::on_create();

	sprite().set_tex_and_coords(tex.enemy, coords.enemy_big);

	set_still_frame_index(0);
	set_left_move_frame_index(1);
	set_right_move_frame_index(2);
	set_still_from_left_frame_index(3);
	set_still_from_right_frame_index(4);

	set_collider_r(30.0);
	set_max_health(600);

	set_kill_se(se.enemy_kill_big);
	set_damage_se(se.enemy_damage);
	set_damage_low_se(se.enemy_damage_low);

	{
		kill_particle().emplace_back();
		auto& p = this->kill_particle().back();
		p.lifeTime = { 15, 60 }, p.cnt = 20;
		p.scale = { 4.0, 5.0 }, p.rand_pos_r = 5.0;
		p.rand_v_direction = true, p.v = { 5.0, 6.0 };
		p.omega = { -0.5, 0.5 }, p.rot = { 0.0, utils::pi * 2.0 };
		p.set_option(Particle::Options::fade_out);
		p.set_option(Particle::Options::shrink);
		p.set_option(Particle::Options::slow_down);
		p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
		p.sprite.set_color(Color(0.8, 0.4, 0.4));
		p.sprite.set_level(DrawLevel::enemy);
	}
}

void EBPrefab::on_drop_item()
{
	Enemy::on_drop_item();
	GameSystem::instance()->drop_item<PowerItem>(this, { 10.0, 40.0 }, drop_power);
	GameSystem::instance()->drop_item<PointItem>(this, { 10.0, 40.0 }, drop_point);
}
