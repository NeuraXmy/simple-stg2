#include "Movement.h"
#include "Utils.h"

namespace move
{
	// 直线运动: 起点、终点
	void straight
	(
		PhysicObject* obj,
		double t, double t_s, double t_t,
		const Pos& p_s,
		const Pos& p_t,
		Factor f
	)
	{
		double k = utils::clamp(f((t - t_s) / (t_t - t_s)), 0.0, 1.0);

		auto p = utils::lerp(p_s, p_t, k);

		obj->set_pos(p.x, p.y);
	}

	// 二阶贝塞尔运动: 起点、终点、控制点
	void bezier2
	(
		PhysicObject* obj,
		double t, double t_s, double t_t,
		const Pos& p_s,
		const Pos& p_t,
		const Pos& c,
		Factor f
	)
	{
		double k = utils::clamp(f((t - t_s) / (t_t - t_s)), 0.0, 1.0);

		auto p1 = utils::lerp(p_s, c, k);
		auto p2 = utils::lerp(c, p_t, k);
		auto p = utils::lerp(p1, p2, k);

		obj->set_pos(p.x, p.y);
	}

	// 三阶贝塞尔运动: 起点，起点控制向量，终点，终点控制向量
	void bezier3
	(
		PhysicObject* obj,
		double t, double t_s, double t_t,
		const Pos& p_s, const Vec& v_s,
		const Pos& p_t, const Vec& v_t,
		Factor f
	)
	{
		double k = utils::clamp(f((t - t_s) / (t_t - t_s)), 0.0, 1.0);

		auto p1 = utils::lerp(p_s, p_s + v_s, k);
		auto p2 = utils::lerp(p_s + v_s, p_t + v_t, k);
		auto p3 = utils::lerp(p_t + v_t, p_t, k);

		auto p4 = utils::lerp(p1, p2, k);
		auto p5 = utils::lerp(p2, p3, k);
		auto p = utils::lerp(p4, p5, k);
	
		obj->set_pos(p.x, p.y);
	}
}