#include "ParticleSystem.h"
#include "PhysicObject.h"


namespace
{
	ParticleSystem* instance = nullptr;
}

ParticleSystem* ParticleSystem::instance()
{
	if (!::instance)
	{
		::instance = new ParticleSystem;
	}
	return ::instance;
}

void ParticleSystem::release_instance()
{
	if (::instance)
	{
		delete ::instance;
	}
	::instance = nullptr;
}

ParticleSystem::~ParticleSystem()
{
	if (particles)
	{
		delete[] particles;
	}
}



void ParticleSystem::set_pool_size(int size)
{
	clear();

	if (particles)
	{
		delete[] particles;
	}

	particles = new Particle::Particle[size];
	pool_size = size;
}

int ParticleSystem::get_pool_size() const
{
	return pool_size;
}

int ParticleSystem::get_count() const
{
	return count;
}

bool ParticleSystem::full() const
{
	return count == pool_size;
}


void ParticleSystem::create_particle(const Particle::Data& data, double ox, double oy)
{
	for (int i = 0; i < data.cnt; i++)
	{
		if (full())
		{
			return;
		}
		particles[count++].init(data, ox, oy);
	}
}

void ParticleSystem::create_particle(const Particle::Data& data, const PhysicObject* obj)
{
	create_particle(data, obj->x(), obj->y());
}

void ParticleSystem::create_particle(const Particle::DataSet& data, double ox, double oy)
{
	for (auto& d : data)
	{
		create_particle(d, ox, oy);
	}
}

void ParticleSystem::create_particle(const Particle::DataSet& data, const PhysicObject* obj)
{
	for (auto& d : data)
	{
		create_particle(d, obj);
	}
}



void ParticleSystem::clear()
{
	count = 0;
}


bool ParticleSystem::is_visible() const
{
	return visible;
}

void ParticleSystem::set_visible(bool flag)
{
	visible = flag;
}

int ParticleSystem::t() const
{
	return update_count;
}

void ParticleSystem::pause()
{
	paused = true;
}

void ParticleSystem::resume()
{
	paused = false;
}

bool ParticleSystem::is_paused() const
{
	return paused;
}


void ParticleSystem::update()
{
	if (paused)
	{
		return;
	}

	update_count++;

	auto& p = particles;
	for (int i = 0; i < count; )
	{
		p[i].update();

		if (p[i].is_to_be_destroy())
		{
			std::swap(p[i], p[count - 1]);
			count--;
		}
		else
		{
			i++;
		}
	}
}

void ParticleSystem::draw()
{
	if (!visible)
	{
		return;
	}
	for (int i = 0; i < count; i++)
	{
		particles[i].draw();
	}
}