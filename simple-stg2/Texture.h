#pragma once

#include <vector>
#include <SFML/Graphics.hpp>


enum class DrawLevel
{
	bottom,
	background_1,
	background_2,
	boss,
	item_1,
	item_2,
	player_bullet_1,
	player,
	enemy,
	player_bullet_2,
	enemy_bullet_1,
	enemy_bullet_2,
	enemy_bullet_3,
	enemy_bullet_4,
	enemy_bullet_5,
	top,
	hit_point,
	gui,
	draw_level_num
};

enum class BlendMode
{
	over,
	plus,
	negative,
	mix,
	blend_mode_num
};


struct Color
{
	double r = 1.0;
	double g = 1.0;
	double b = 1.0;
	double a = 1.0;

	Color() = default;

	Color(double r, double g, double b, double a = 1.0) :
		r(r), g(g), b(b), a(a)
	{ }

	Color(const sf::Color& c) :
		r(c.r / 255.0), g(c.g / 255.0), b(c.b / 255.0), a(c.a / 255.0)
	{ }

	operator const sf::Color() const
	{
		return sf::Color(r * 255.0, g * 255.0, b * 255.0, a * 255.0);
	}
};


namespace Texture
{
	struct Coords
	{
		std::vector<sf::IntRect>	frame_texcoords;
		std::vector<int>			frame_time;
		std::vector<int>			next_frame;
	};

	struct Quad
	{
		sf::Vector2f vert[4] = {};
		sf::Vector2f coords[4] = {};
		Color color[4] = {};
	};
	
	struct Texture
	{
		sf::Texture* sf_texture = nullptr;

		std::vector<sf::Vertex> verts
			[(int)(DrawLevel::draw_level_num)]
			[(int)(BlendMode::blend_mode_num)];

	};
}