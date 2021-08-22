#include "Shooter.h"
#include "GameSystem.h"
#include "Sound.h"

static const double pi = std::acos(-1.0);


void ShotData::set_fan_style(int way, double range, double r)
{
	this->way = way;

	if (way <= 1)
	{
		this->rot_interval = 0.0;
		this->rot_offset = 0.0;
	}
	else 
	{
		this->rot_interval = range / (way - 1.0);
		this->rot_offset = -rot_interval * (way - 1.0) * 0.5;
	}
	this->r_offset = r;
}

void ShotData::set_cir_style(int way, double r, bool odd)
{
	set_fan_style(way, pi * 2.0 * (1.0 - 1.0 / way), r);
	if (!odd)
		rot_offset = pi * 2.0 / way * 0.5;
	else
		rot_offset = 0.0;
}


int Shooter::get_shot_interval() const
{
	return shot_interval;
}

void Shooter::set_shot_interval(int interval)
{
	shot_interval = interval;
}


void Shooter::start()
{
	if (!shooting)
	{
		last_shot_time = t() - shot_interval;
		shot_count = 0;
		shooting = true;
		on_start();
	}
}

void Shooter::stop()
{
	if (shooting)
	{
		shooting = false;
		on_stop();
	}
}

bool Shooter::is_shooting() const
{
	return shooting;
}

bool Shooter::is_keep_on_player() const
{
	return keep_on_player;
}

void Shooter::set_keep_on_player(bool flag)
{
	keep_on_player = flag;
}

bool Shooter::is_follow_rot() const
{
	return follow_rot;
}

void Shooter::set_follow_rot(bool flag)
{
	follow_rot = flag;
}

int Shooter::shot_t() const
{
	return shot_count * shot_interval;
}

int Shooter::get_shot_count() const
{
	return shot_count;
}

void Shooter::set_blocked_shoot(bool flag)
{
	blocked = flag;
}

bool Shooter::is_blocked_shoot() const
{
	return blocked;
}

Sound::Se* Shooter::get_shoot_se() const
{
	return shoot_se;
}

void Shooter::set_shoot_se(Sound::Se* se)
{
	shoot_se = se;
}


void Shooter::on_create()
{
	SpriteObject::on_create();

	sprite().set_level(DrawLevel::player);
}

void Shooter::on_update()
{
	SpriteObject::on_update();

	if (keep_on_player)
	{
		if (auto p = GameSystem::instance()->get_player())
		{
			this->point_to(p);
		}
	}

	if (shooting)
	{
		if (t() - last_shot_time >= shot_interval)
		{
			last_shot_time = t();
			if (shoot_se)
				shoot_se->play();
			on_shoot();
			shot_count++;
		}
	}
	else
	{
		on_slient();
	}
}

void Shooter::on_destroy()
{

	SpriteObject::on_destroy();
}

void Shooter::on_draw()
{
	SpriteObject::on_draw();

}


void Shooter::on_start()
{
	
}

void Shooter::on_stop()
{
}

void Shooter::on_shoot()
{
}

void Shooter::on_slient()
{
}
