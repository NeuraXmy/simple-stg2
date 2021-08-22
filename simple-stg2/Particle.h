#pragma once

#include <memory>
#include <functional>
#include <vector>

#include "Sprite.h"
#include "Utils.h"

class PhysicObject;


namespace Particle
{
	class Particle;
	using UpdateFunc = void (Particle*);

	enum class Options
	{
		none = 0,
		fade_out = 1 << 0,
		shrink = 1 << 1,
		slow_down = 1 << 2,
		destroy_when_ani_end = 1 << 3,
		gather = 1 << 4,
		appear = 1 << 5
	};

	struct Data
	{
		int cnt;

		Sprite sprite;

		utils::Range rand_pos_r = { 0.0, 0.0 };

		bool rand_v_direction = false;
		utils::Range v = { 0.0, 0.0 };
		utils::Range vx = { 0.0, 0.0 };
		utils::Range vy = { 0.0, 0.0 };

		utils::Range scale = { 1.0, 1.0 };

		utils::Range rot = { 0.0, 0.0 };
		utils::Range omega = { 0.0, 0.0 };

		utils::Range lifeTime = { -1, -1 };

		int options = (int)Options::none;

		UpdateFunc* update_func = nullptr;

		void set_option(Options option, bool flag = true);
	};


	class Particle final
	{
	public:

		Sprite sprite;

		bool visible = true;

		int life_time = -1;

		double x = 0.0;
		double y = 0.0;
		double vx = 0.0;
		double vy = 0.0;
		double rot = 0.0;
		double omega = 0.0;

		double scale = 1.0;

		int options = (int)Options::none;


		void init(const Data& data, double ox, double oy);


		bool is_to_be_destroy() const;

		void destroy();

		void update();

		void draw();

		int t() const;

		double life_factor() const;

	private:

		UpdateFunc* update_func;

		bool to_be_destroy = false;

		int update_count = 0;

	};

	using DataSet = std::vector<Data>;
}

