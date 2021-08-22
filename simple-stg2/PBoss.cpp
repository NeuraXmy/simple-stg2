#include "PBoss.h"
#include "PResource.h"
#include "GameSystem.h"

void PBoss::on_create()
{
	Boss::on_create();

	set_still_frame_index(0);
	set_left_move_frame_index(1);
	set_right_move_frame_index(2);
	set_still_from_left_frame_index(3);
	set_still_from_right_frame_index(4);
	set_casting_frame_index(5);

	set_damage_se(se.enemy_damage);
	set_damage_low_se(se.enemy_damage_low);
	set_charge_se(se.charge);

	magic_circle().set_tex_and_coords(tex.magic_circle, coords.magic_circle);
	magic_circle().set_scale(2.0, 2.0);

	set_collider_r(25.0);

	kill_ani_particle().emplace_back();
	{
		auto& p = this->kill_ani_particle().back();
		p.lifeTime = { 30, 45 }, p.cnt = 2;
		p.scale = { 2.0, 3.0 }, p.rand_pos_r = 5.0;
		p.rand_v_direction = true, p.v = { 4.0, 6.0 };
		p.omega = { -0.3, 0.3 }, p.rot = { 0.0, utils::pi * 2.0 };
		p.set_option(Particle::Options::fade_out);
		p.set_option(Particle::Options::shrink);
		p.set_option(Particle::Options::slow_down);
		p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
		p.sprite.set_color(Color(1.0, 1.0, 1.0, 1.0));
		p.sprite.set_level(DrawLevel::enemy);
	}

	kill_particle().emplace_back();
	{
		auto& p = this->kill_particle().back();
		p.lifeTime = { 45, 60 }, p.cnt = 30;
		p.scale = { 3.0, 5.0 }, p.rand_pos_r = 5.0;
		p.rand_v_direction = true, p.v = { 5.0, 10.0 };
		p.omega = { -0.3, 0.3 }, p.rot = { 0.0, utils::pi * 2.0 };
		p.set_option(Particle::Options::fade_out);
		p.set_option(Particle::Options::shrink);
		p.set_option(Particle::Options::slow_down);
		p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
		p.sprite.set_color(Color(1.0, 1.0, 1.0, 1.0));
		p.sprite.set_level(DrawLevel::enemy);
	}

	charge_particle().emplace_back();
	{
		auto& p = this->charge_particle().back();
		p.lifeTime = { 15, 60 }, p.cnt = 20;
		p.scale = { 6.0, 10.0 }, p.rand_pos_r = { 200.0, 400.0 };
		p.omega = { -0.3, 0.3 }, p.rot = { 0.0, utils::pi * 2.0 };
		p.set_option(Particle::Options::appear);
		p.set_option(Particle::Options::shrink);
		p.set_option(Particle::Options::gather);
		p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
		p.sprite.set_color(Color(1.0, 1.0, 1.0, 0.5));
		p.sprite.set_level(DrawLevel::enemy);
	}

	set_kill_ani_time(60);
	set_attack_interval(120);
}