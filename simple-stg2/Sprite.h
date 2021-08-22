#pragma once

#include "Texture.h"


// 精灵绘制材质 可以是单材质或是动画
class Sprite final
{
public:

	bool ready() const;

	void update();


	int frame_count() const;

	int current_frame() const;

	bool is_still_frame() const;

	void skip_frame(int frame);

	void set_frame(int index);

	bool is_paused() const;

	void pause();

	void resume();


	double w_scale() const;

	double h_scale() const;

	void set_w_scale(double scale);

	void set_h_scale(double scale);

	double tex_w_scale() const;

	double tex_h_scale() const;

	void set_tex_w_scale(double scale);

	void set_tex_h_scale(double scale);

	void set_scale(double w, double h);

	void set_tex_scale(double w, double h);


	Color color() const;

	double alpha() const;

	void set_color(const Color& color);

	void set_red(double r);

	void set_green(double g);

	void set_blue(double b);

	void set_alpha(double a);

	DrawLevel level() const;

	void set_level(DrawLevel level);

	BlendMode blend() const;

	void set_blend(BlendMode blend);


	Texture::Coords* coords() const;

	Texture::Texture* texture() const;

	void set_coords(Texture::Coords* coords);

	void set_texture(Texture::Texture* texture);

	void set_tex_and_coords(Texture::Texture* texture, Texture::Coords* coords);


	bool visible() const;

	void set_visible(bool flag);


	sf::IntRect& current_coord();

	const sf::IntRect& current_coord() const;

	void set_repeated(bool flag);

	bool is_repeated() const;

	
	Texture::Quad quad(double x, double y, double rot = 0.0) const;

private:

	struct Data
	{

		double w_scale = 1.0;
		double h_scale = 1.0;
		double tex_w_scale = 1.0;
		double tex_h_scale = 1.0;
		Color color = Color(1.0, 1.0, 1.0, 1.0);
		DrawLevel level = DrawLevel::top;
		BlendMode blend = BlendMode::over;

		Texture::Coords* coords = nullptr;
		Texture::Texture* texture = nullptr;

		int current_frame = 0;
		int current_frame_elapsed = 0;

		bool paused = false;

		bool visible = true;

		bool repeat = false;
	};

	Data d;
};