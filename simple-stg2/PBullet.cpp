#include "PBullet.h"
#include "PResource.h"
#include "GameSystem.h"
#include "PItem.h"
#include "ParticleSystem.h"

void expand_particle_func(Particle::Particle* p) 
{
	p->scale = (double)p->t() / p->life_time * 0.5 + 1.0;
}

const double bs_radius[] =
{
	2.47, 2.47, 2.47
};

const Color bs_color[] =
{
	{1.00, 1.00, 1.00}, {0.90, 0.30, 0.30}, 
	{0.90, 0.30, 0.30}, {0.60, 0.30, 0.70},
	{0.90, 0.30, 0.90}, {0.30, 0.10, 0.90},
	{0.20, 0.30, 0.90}, {0.30, 0.90, 0.90}, 
	{0.30, 0.90, 0.90}, {0.30, 0.90, 0.60}, 
	{0.30, 0.90, 0.60}, {0.60, 0.90, 0.30},
	{0.90, 0.90, 0.30}, {0.90, 0.90, 0.30}, 
	{0.90, 0.80, 0.30}, {1.00, 1.00, 1.00}
};

const BBColor bs_to_bb_color[] = {
	BBColor::gray, BBColor::red, BBColor::red, BBColor::purple, 
	BBColor::purple, BBColor::blue, BBColor::blue,
	BBColor::sky_blue, BBColor::sky_blue, BBColor::green, BBColor::green,
	BBColor::green, BBColor::green, BBColor::yellow, BBColor::yellow, 
	BBColor::gray
};

const BSColor bb_to_bs_color[] = {
	BSColor::black, BSColor::red, BSColor::pink, BSColor::blue, BSColor::sky_blue, BSColor::green,
	BSColor::yellow, BSColor::white
};

void BSPrefab::on_create()
{
	set_sprite(tex.bullet, coords.bullet_small[(int)type][(int)color]);
	sprite().set_level(DrawLevel::enemy_bullet_1);
	set_collider_r(bs_radius[(int)type]);
	set_boundary(50, 50, 50, 50);

	{
		kill_particle().emplace_back();
		auto& p = kill_particle().back();
		p.lifeTime = 20, p.cnt = 5;
		p.scale = 0.8, p.rand_pos_r = 5.0;
		p.rand_v_direction = true, p.v = 2.0;
		p.set_option(Particle::Options::fade_out);
		p.set_option(Particle::Options::slow_down);
		p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
		Color c = bs_color[(int)color];	c.a = 1.0;
		p.sprite.set_color(c);
		p.sprite.set_level(DrawLevel::enemy);
	}

	if(create_effect)
	{
		create_particle().emplace_back();
		auto& p = create_particle().back();
		p.lifeTime = 15, p.cnt = 1;
		p.set_option(Particle::Options::fade_out);
		p.update_func = &expand_particle_func;
		p.sprite.set_tex_and_coords(tex.bullet, coords.laser_light[(int)bs_to_bb_color[(int)color]]);
		p.sprite.set_scale(0.8, 0.8);
		p.sprite.set_alpha(0.7);
		p.sprite.set_level(DrawLevel::enemy_bullet_5);
	}

	if (type == BSType::star)
		set_omega(0.05);
}

void BSPrefab::on_drop_item()
{
	EnemyBullet::on_drop_item();
	GameSystem::instance()->drop_item<PointSmallItem>(this, 0, 1);
}


const double bb_radius[] =
{
	6.73
};

const Color bb_color[] =
{
	{1.00, 1.00, 1.00}, {0.80, 0.30, 0.30},
	{0.60, 0.30, 0.70}, {0.30, 0.30, 0.80},
	{0.30, 0.60, 0.90}, {0.30, 1.00, 0.30},
	{1.00, 0.90, 0.30}, {1.00, 1.00, 1.00}
};

void BBPrefab::on_create()
{
	set_sprite(tex.bullet, coords.bullet_big[(int)type][(int)color]);
	sprite().set_level(DrawLevel::enemy_bullet_2);
	set_collider_r(bb_radius[(int)type]);
	set_boundary(50, 50, 50, 50);

	{
		kill_particle().emplace_back();
		auto& p = kill_particle().back();
		p.lifeTime = 20, p.cnt = 7;
		p.scale = 0.8, p.rand_pos_r = 5.0;
		p.rand_v_direction = true, p.v = 2.5;
		p.set_option(Particle::Options::fade_out);
		p.set_option(Particle::Options::slow_down);
		p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
		Color c = bb_color[(int)color];	c.a = 1.0;
		p.sprite.set_color(c);
		p.sprite.set_level(DrawLevel::enemy);
	}

	if (create_effect)
	{
		create_particle().emplace_back();
		auto& p = create_particle().back();
		p.lifeTime = 15, p.cnt = 1;
		p.set_option(Particle::Options::fade_out);
		p.update_func = &expand_particle_func;
		p.sprite.set_tex_and_coords(tex.bullet, coords.laser_light[(int)color]);
		p.sprite.set_scale(1.5, 1.5);
		p.sprite.set_alpha(0.7);
		p.sprite.set_level(DrawLevel::enemy_bullet_5);
	}

	if (type == BBType::star)
		set_omega(0.05);
}

void BBPrefab::on_drop_item()
{
	EnemyBullet::on_drop_item();
	GameSystem::instance()->drop_item<PointSmallItem>(this, 0, 1);
}
