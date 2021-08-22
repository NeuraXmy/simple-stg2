#include "Bullet.h"
#include "GameSystem.h"
#include "ParticleSystem.h"
#include "Sound.h"


Bullet::Bullet(CollideGroup group) : CollideCircle(group, this)
{
}


bool Bullet::is_boundary_test_enabled() const
{
	return boundary_test;
}

void Bullet::set_boundary_test(bool flag)
{
	boundary_test = flag;
}

void Bullet::set_boundary(double l, double r, double t, double b)
{
	boundary_t = t;
	boundary_b = b;
	boundary_l = l;
	boundary_r = r;
}



void Bullet::kill(bool drop_item)
{
	if (!killed)
	{
		killed = true;

		ParticleSystem::instance()->create_particle(kill_particle_data, this);

		if (kill_se)
			kill_se->play();

		if (drop_item)
		{
			on_drop_item();
		}

		on_kill();

		destroy();
	}
}

void Bullet::kill_raw()
{
	if (!killed)
	{
		killed = true;
		on_kill();
		destroy();
	}
}

bool Bullet::is_killed() const
{
	return killed;
}


Particle::DataSet& Bullet::kill_particle()
{
	return kill_particle_data;
}

const Particle::DataSet& Bullet::kill_particle() const
{
	return kill_particle_data;
}

Particle::DataSet& Bullet::create_particle()
{
	return create_particle_data;
}

const Particle::DataSet& Bullet::create_particle() const
{
	return create_particle_data;
}

Sound::Se* Bullet::get_create_se() const
{
	return create_se;
}

void Bullet::set_create_se(Sound::Se* se)
{
	create_se = se;
}

Sound::Se* Bullet::get_kill_se() const
{
	return kill_se;
}

void Bullet::set_kill_se(Sound::Se* se)
{
	kill_se = se;
}


void Bullet::on_create()
{
	SpriteObject::on_create();

}

void Bullet::on_update()
{
	SpriteObject::on_update();

	if (t() == 0)
	{
		if(create_se)
			create_se->play();
		ParticleSystem::instance()->create_particle(create_particle_data, this);
	}

	if (boundary_test && 
		(x() < -boundary_l || x() > GameSystem::game_w + boundary_r
	  || y() < -boundary_t || y() > GameSystem::game_h + boundary_b))
		kill_raw();
}

void Bullet::on_destroy()
{

	SpriteObject::on_destroy();
}

void Bullet::on_draw()
{
	SpriteObject::on_draw();

}

void Bullet::on_collide(PhysicObject* obj)
{
	
}


void Bullet::on_kill()
{
	
}

void Bullet::on_drop_item()
{
}
