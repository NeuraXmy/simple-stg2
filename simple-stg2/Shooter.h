#pragma once

#include "SpriteObject.h"
#include "ObjectSystem.h"
#include "Particle.h"
#include "Utils.h"

namespace Sound { class Se;  }


struct ShotData
{
	int way = 1;
	double rot_interval = 0.0;
	utils::Range rot_offset = 0.0;
	utils::Range r_offset = 0.0;
	utils::Range x_offset = 0.0;
	utils::Range y_offset = 0.0;
	utils::Range v = 0.0;
	utils::Range a = 0.0;
	utils::Range vx = 0.0;
	utils::Range vy = 0.0;
	utils::Range ax = 0.0;
	utils::Range ay = 0.0;
	utils::Range v_max = 1e10;
	utils::Range v_min = -1e10;

	void set_fan_style(int way, double range, double r = 0.0);

	void set_cir_style(int way, double r = 0.0, bool odd = true);
};


class Shooter : public SpriteObject
{
public:

	virtual ~Shooter() = default;
	

	int get_shot_interval() const;

	void set_shot_interval(int interval);

	void start();

	void stop();

	bool is_shooting() const;


	bool is_keep_on_player() const;

	void set_keep_on_player(bool flag);

	bool is_follow_rot() const;

	void set_follow_rot(bool flag);

	
	int shot_t() const;

	int get_shot_count() const;


	void set_blocked_shoot(bool flag);

	bool is_blocked_shoot() const;


	Sound::Se* get_shoot_se() const;

	void set_shoot_se(Sound::Se* se);

	
	template<class Bullet, class ... Args>
	void shoot(const ShotData& data, Args ... args)
	{
		if (blocked)
			return;

		for (int i = 0; i < data.way; i++)
		{
			auto b = ObjectSystem::instance()->create_object<Bullet>(args ...);

			double angle = data.rot_offset.get_game() + data.rot_interval * i;
			if (follow_rot)
				angle += rot();

			double kx = cos(angle), ky = sin(angle);

			double x = this->x() + data.x_offset.get_game() + data.r_offset.get_game() * kx;
			double y = this->y() + data.y_offset.get_game() + data.r_offset.get_game() * ky;
			b->set_pos(x, y);
			b->set_rot(angle);

			b->set_v(data.v.get_game() * kx + data.vx.get_game(), data.v.get_game() * ky + data.vy.get_game());
			b->set_a(data.a.get_game() * kx + data.ax.get_game(), data.a.get_game() * ky + data.ay.get_game());

			if (data.v_min.get_avg() > -1e8)
				b->set_min_v(data.v_min.get_game());
			if (data.v_max.get_avg() < 1e8)
				b->set_max_v(data.v_max.get_game());

		}
	}

	template<class Bullet, class Func, class ... Args>
	void shoot_with_func(const ShotData& data, Func&& func, Args ... args)
	{
		if (blocked)
			return;

		for (int i = 0; i < data.way; i++)
		{
			auto b = ObjectSystem::instance()->create_object<Bullet>(args ...);

			double angle = data.rot_offset.get_game() + data.rot_interval * i;
			if (follow_rot)
				angle += rot();

			double kx = cos(angle), ky = sin(angle);

			double x = this->x() + data.x_offset.get_game() + data.r_offset.get_game() * kx;
			double y = this->y() + data.y_offset.get_game() + data.r_offset.get_game() * ky;
			b->set_pos(x, y);
			b->set_rot(angle);

			b->set_v(data.v.get_game() * kx + data.vx.get_game(), data.v.get_game() * ky + data.vy.get_game());
			b->set_a(data.a.get_game() * kx + data.ax.get_game(), data.a.get_game() * ky + data.ay.get_game());

			if (data.v_min.get_avg() > -1e8)
				b->set_min_v(data.v_min.get_game());
			if (data.v_max.get_avg() < 1e8)
				b->set_max_v(data.v_max.get_game());

			func(b);
		}
	}

protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;


	virtual void on_start();

	virtual void on_stop();
	
	virtual void on_shoot();

	virtual void on_slient();


private:

	bool follow_rot = true;

	bool shooting = false;

	bool keep_on_player = false;

	int shot_interval = 1;
	int last_shot_time = 0;

	int shot_count = 0;

	bool blocked = false;

	Sound::Se* shoot_se = nullptr;

};

