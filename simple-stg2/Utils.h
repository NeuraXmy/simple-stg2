#pragma once

#include <iostream>


namespace utils
{ 
	extern const double pi;

	struct Range
	{
		double l, r;

		Range() = default;

		Range(double x) : l(x), r(x) {}
		
		Range(double l, double r) : l(l), r(r) {}

		double get_draw() const;

		double get_game() const;

		double get_avg() const;

		const Range operator+(double r) const;
		const Range operator-(double r) const;
		const Range operator*(double r) const;
		const Range operator/(double r) const;
	};

	struct IntRange
	{
		int l, r;

		IntRange() = default;

		IntRange(int x) : l(x), r(x) {}

		IntRange(int l, int r) : l(l), r(r) {}

		int get_draw() const;

		int get_game() const;

		double get_avg() const;

		const IntRange operator+(int r) const;
		const IntRange operator-(int r) const;
		const IntRange operator*(int r) const;
		const IntRange operator/(int r) const;
	};

	template<class T>
	T lerp(T start, T end, double factor)
	{
		return start + (end - start) * factor;
	}

	double lerp_angle(double start, double end, double factor);

	template<class T>
	T clamp(T x, T l, T r)
	{
		return std::min(r, std::max(l, x));
	}
}