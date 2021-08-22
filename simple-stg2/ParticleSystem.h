#pragma once

#include "Particle.h"

class PhysicObject;


// 粒子系统，该系统由GameSystem管理
class ParticleSystem final
{
public:

	static ParticleSystem* instance();

	static void release_instance();


	void set_pool_size(int size);

	int get_pool_size() const;

	int get_count() const;

	bool full() const;


	void create_particle(const Particle::Data& data, double ox, double oy);
	
	void create_particle(const Particle::Data& data, const PhysicObject* obj);

	void create_particle(const Particle::DataSet& data, double ox, double oy);

	void create_particle(const Particle::DataSet& data, const PhysicObject* obj);

	void clear();


	bool is_visible() const;

	void set_visible(bool flag);


	int t() const;

	void pause();

	void resume();

	bool is_paused() const;



	void update();

	void draw();
	
	template<class Func>
	void foreach(Func&& func) const
	{
		for (int i = 0; i < count; i++)
		{
			func(particles + i);
		}
	}

private:

	~ParticleSystem();


	bool paused = false;

	int update_count = 0;

	bool visible = true;

	Particle::Particle* particles = nullptr;

	int pool_size = 0;
	int count = 0;

};

