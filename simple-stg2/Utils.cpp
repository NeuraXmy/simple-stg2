#include "Utils.h"
#include "GameSystem.h"
#include "DrawSystem.h"

namespace utils
{
	const double pi = acos(-1.0);

	double Range::get_draw() const
	{
		return abs(r - l) < 1e-8 ? l : DrawSystem::instance()->rand_real(l, r);
	}
	double Range::get_game() const
	{
		return abs(r - l) < 1e-8 ? l : GameSystem::instance()->rand_real(l, r);
	}

	double Range::get_avg() const
	{
		return (l + r) * 0.5;
	}

	const Range Range::operator+(double r) const
	{
		return { this->l + r, this->r + r };
	}

	const Range Range::operator-(double r) const
	{
		return { this->l - r, this->r - r };
	}

	const Range Range::operator*(double r) const
	{
		return { this->l * r, this->r * r };
	}

	const Range Range::operator/(double r) const
	{
		return { this->l / r, this->r / r };
	}


	int IntRange::get_draw() const
	{
		return DrawSystem::instance()->rand_int(l, r);
	}

	int IntRange::get_game() const
	{
		return GameSystem::instance()->rand_int(l, r);
	}

	double IntRange::get_avg() const
	{
		return (l + r) * 0.5;
	}

	const IntRange IntRange::operator+(int r) const
	{
		return { this->l + r, this->r + r };
	}

	const IntRange IntRange::operator-(int r) const
	{
		return { this->l - r, this->r - r };
	}

	const IntRange IntRange::operator*(int r) const
	{
		return { this->l * r, this->r * r };
	}

	const IntRange IntRange::operator/(int r) const
	{
		return { this->l / r, this->r / r };
	}


	double lerp_angle(double start, double end, double factor)
	{
		static const double m = pi * 2.0;
		double delta = fmod(fmod(end - start, m) + m, m);
		if (delta <= pi)
			return start + delta * factor;
		else
			return start - (m - delta) * factor;
	}
}