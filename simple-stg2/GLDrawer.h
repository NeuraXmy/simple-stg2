#pragma once

#include "Texture.h"
#include "Sprite.h"


namespace GLDrawer
{
	struct Quad3d final
	{
		sf::Vector3f vert[4] = {};
		sf::Vector2f coords[4] = {};
		Color color[4] = {};

		void set_vert(int index, const sf::Vector3f& vert);

		void set_coords(int index, const sf::Vector2f& coords);

		// set clockwise coords rect (0.0, 0.0, 1.0, 1.0)
		void set_standard_coords();

		void scale_coords(float w, float h);

		void translate_coords(float x, float y);

		void set_color(int index, const Color& color);

		void set_all_color(const Color& color);

		void set_all_alpha(double alpha);

	};


	void before_draw();
	
	void set_viewport(float left, float top, float w, float h);

	void bind_texture(Texture::Texture* tex, bool repeat = true, bool smooth = false);

	void draw_quad(const Quad3d& q);

	void draw_floor(Quad3d q, float tile_size, float radius, float camera_x, float camera_z);

	void set_fog(bool enable, float start = 10.0, float end = 50.0, const Color& color = {});

	
	namespace Camera
	{
		void push();

		void pop();

		void set_identity();

		

		void set_frustum(float l, float r, float t, float b, float near, float far);

		void translate(float x, float y, float z);

		void rotate(float angle, const sf::Vector3f& axis);

		void scale(float x, float y, float z);
	};

	namespace Model
	{
		void push();

		void pop();

		void set_identity();

		void translate(float x, float y, float z);

		void rotate(float angle, const sf::Vector3f& axis);

		void scale(float x, float y, float z);
	}
	
	
}