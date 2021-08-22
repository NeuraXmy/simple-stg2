#pragma once

#include "Sprite.h"


class Background
{
public:

	virtual ~Background() = default;


	void draw(int t);

	void update(int t);


	bool is_visible() const;

	void set_visible(bool flag);

	double get_alpha() const;

	void set_alpha(double a);

	DrawLevel get_level() const;

	void set_level(DrawLevel level);


protected:

	void draw_sprite(Sprite& s, double x, double y, double rot = 0.0);

	void draw_sprite(Sprite& s, const Texture::Quad& q);

	virtual void on_draw(int t);

	virtual void on_update(int t);


private:

	bool visible = true;

	double alpha = 1.0;

	DrawLevel level = DrawLevel::background_1;

};

