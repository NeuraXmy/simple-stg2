#pragma once

#include "PhysicObject.h"


struct Pos
{
	double x = 0.0;
	double y = 0.0;

	Pos() = default;
	Pos(double x, double y) : x(x), y(y) {}
	Pos(const PhysicObject* obj) : x(obj->x()), y(obj->y()) {}

	double get_dist(const Pos& p) 
	{
		return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
	}
	double get_len()
	{
		return sqrt(x * x + y * y);
	}

	const Pos operator+(const Pos& r) const
	{
		return { x + r.x, y + r.y };
	}
	const Pos operator-(const Pos& r) const
	{
		return { x - r.x, y - r.y };
	}
	const Pos operator*(double r) const
	{
		return { x * r, y * r };
	}
	const Pos operator/(double r) const
	{
		return { x / r, y / r };
	}
	Pos& operator+=(const Pos& r) 
	{
		return *this = *this + r;
	}
	Pos& operator-=(const Pos& r) 
	{
		return *this = *this - r;
	}
	Pos& operator*=(double r)
	{
		return *this = *this * r;
	}
	Pos& operator/=(double r)
	{
		return *this = *this / r;
	}
};
using Vec = Pos;


namespace move
{
	using Factor = double(*)(double);

	namespace factor
	{
		inline double linear(double t)
		{
			return t;
		}

		template<int in_t = 50>
		inline double ease_in(double t)
		{
			static constexpr double i = in_t * 0.01;
			static constexpr double v = 2.0 / (2.0 - i);
			static constexpr double a = v / i;
			if (t < i)
				return 0.5 * a * t * t;
			else
				return 0.5 * a * i * i + v * (t - i);
		}

		template<int out_t = 50>
		inline double ease_out(double t)
		{
			static constexpr double o = out_t * 0.01;
			static constexpr double v = 2.0 / (2.0 - o);
			static constexpr double a = v / o;
			if (t < 1.0 - o)
				return t * v;
			else
				return t * v - 0.5 * a * (t - 1.0 + o) * (t - 1.0 + o);
		}

		template<int in_t = 30, int out_t = 30>
		inline double ease_in_out(double t)
		{
			static constexpr double i = in_t * 0.01;
			static constexpr double o = out_t * 0.01;
			static constexpr double v = 2.0 / (2.0 - (i + o));
			static constexpr double a1 = v / i;
			static constexpr double a2 = v / o;
			if (t < i)
				return 0.5 * a1 * t * t;
			else if (t < 1.0 - o)
				return 0.5 * a1 * i * i + v * (t - i);
			else
				return 0.5 * a1 * i * i + v * (t - i) - 0.5 * a2 * (t - 1.0 + o) * (t - 1.0 + o);
		}
	}

	// 直线运动: 起点、终点
	void straight
	(
		PhysicObject* obj, 
		double t, double t_s, double t_t,
		const Pos& p_s, 
		const Pos& p_t,
		Factor f = factor::linear
	);

	// 二阶贝塞尔运动: 起点、终点、控制点
	void bezier2
	(
		PhysicObject* obj, 
		double t, double t_s, double t_t,
		const Pos& p_s, 
		const Pos& p_t, 
		const Pos& c,
		Factor f = factor::linear
	);

	// 三阶贝塞尔运动: 起点，起点控制向量，终点，终点控制向量
	void bezier3
	(
		PhysicObject* obj,
		double t, double t_s, double t_t,
		const Pos& p_s, const Vec& v_s,
		const Pos& p_t, const Vec& v_t,
		Factor f = factor::linear
	);

}