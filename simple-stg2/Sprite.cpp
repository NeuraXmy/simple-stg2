#include "Sprite.h"


bool Sprite::ready() const
{
	return d.coords && d.texture;
}

void Sprite::update()
{
	if (d.paused || !d.coords || is_still_frame())
	{
		return;
	}

	if (++d.current_frame_elapsed == d.coords->frame_time[d.current_frame])
	{
		d.current_frame_elapsed = 0;

		d.current_frame += d.coords->next_frame[d.current_frame];
		if (d.current_frame < 0)
			d.current_frame = d.coords->frame_time.size() - 1;
		if (d.current_frame >= d.coords->frame_time.size())
			d.current_frame = 0;
	}
}

int Sprite::frame_count() const
{
	return d.coords->frame_time.size();
}

int Sprite::current_frame() const
{
	return d.current_frame;
}

bool Sprite::is_still_frame() const
{
	return d.coords->frame_time[d.current_frame] <= 0 || d.coords->next_frame[d.current_frame] == 0;
}

void Sprite::skip_frame(int frame)
{
	while (frame--)
	{
		if (++d.current_frame_elapsed > d.coords->frame_time[d.current_frame])
		{
			d.current_frame_elapsed = 0;

			d.current_frame += d.coords->next_frame[d.current_frame];
			if (d.current_frame < 0)
				d.current_frame = d.coords->frame_time.size() - 1;
			if (d.current_frame >= d.coords->frame_time.size())
				d.current_frame = 0;
		}
	}
}

void Sprite::set_frame(int index)
{
	d.current_frame_elapsed = 0;
	d.current_frame = index;
}

bool Sprite::is_paused() const
{
	return d.paused;
}

void Sprite::pause()
{
	d.paused = true;
}

void Sprite::resume()
{
	d.paused = false;
}


double Sprite::w_scale() const
{
	return d.w_scale;
}

double Sprite::h_scale() const
{
	return d.h_scale;
}

void Sprite::set_w_scale(double scale)
{
	d.w_scale = scale;
}

void Sprite::set_h_scale(double scale)
{
	d.h_scale = scale;
}

double Sprite::tex_w_scale() const
{
	return d.tex_w_scale;
}

double Sprite::tex_h_scale() const
{
	return d.tex_h_scale;
}

void Sprite::set_tex_w_scale(double scale)
{
	d.tex_w_scale = scale;
}

void Sprite::set_tex_h_scale(double scale)
{
	d.tex_h_scale = scale;
}

void Sprite::set_scale(double w, double h)
{
	d.w_scale = w;
	d.h_scale = h;
}

void Sprite::set_tex_scale(double w, double h)
{
	d.tex_w_scale = w;
	d.tex_h_scale = h;
}

Color Sprite::color() const
{
	return d.color;
}

double Sprite::alpha() const
{
	return d.color.a;
}

void Sprite::set_color(const Color& color)
{
	d.color = color;
}

void Sprite::set_red(double r)
{
	d.color.r = r;
}

void Sprite::set_green(double g)
{
	d.color.g = g;
}

void Sprite::set_blue(double b)
{
	d.color.b = b;
}

void Sprite::set_alpha(double a)
{
	d.color.a = a;
}

DrawLevel Sprite::level() const
{
	return d.level;
}

void Sprite::set_level(DrawLevel level)
{
	d.level = level;
}

BlendMode Sprite::blend() const
{
	return d.blend;
}

void Sprite::set_blend(BlendMode blend)
{
	d.blend = blend;
}

Texture::Coords* Sprite::coords() const
{
	return d.coords;
}

Texture::Texture* Sprite::texture() const
{
	return d.texture;
}

void Sprite::set_coords(Texture::Coords* coords)
{
	d.coords = coords;
}

void Sprite::set_texture(Texture::Texture* texture)
{
	d.texture = texture;
}

void Sprite::set_tex_and_coords(Texture::Texture* texture, Texture::Coords* coords)
{
	d.texture = texture;
	d.coords = coords;
}

bool Sprite::visible() const
{
	return d.visible;
}

void Sprite::set_visible(bool flag)
{
	d.visible = flag;
}

sf::IntRect& Sprite::current_coord()
{
	return d.coords->frame_texcoords[d.current_frame];
}

const sf::IntRect& Sprite::current_coord() const
{
	return d.coords->frame_texcoords[d.current_frame];
}

void Sprite::set_repeated(bool flag)
{
	d.repeat = flag;
}

bool Sprite::is_repeated() const
{
	return d.repeat;
}

Texture::Quad Sprite::quad(double x, double y, double rot) const
{
	Texture::Quad q;

	auto& coord = current_coord();
	float tex_w = coord.width;
	float tex_h = coord.height;
	if (is_repeated())
	{
		tex_w *= tex_w_scale();
		tex_h *= tex_h_scale();
	}
	q.coords[0] = sf::Vector2f(coord.left, coord.top);
	q.coords[1] = sf::Vector2f(coord.left + tex_w, coord.top);
	q.coords[2] = sf::Vector2f(coord.left + tex_w, coord.top + tex_h);
	q.coords[3] = sf::Vector2f(coord.left, coord.top + tex_h);

	q.color[0] = q.color[1] = q.color[2] = q.color[3] = color();

	float halfw = abs(coord.width)	* w_scale() * 0.5;
	float halfh = abs(coord.height) * h_scale() * 0.5;

	q.vert[0] = sf::Vector2f(-halfw, -halfh);
	q.vert[1] = sf::Vector2f(halfw, -halfh);
	q.vert[2] = sf::Vector2f(halfw, halfh);
	q.vert[3] = sf::Vector2f(-halfw, halfh);

	for (int i = 0; i < 4; i++)
	{
		float x = q.vert[i].x;
		float y = q.vert[i].y;
		q.vert[i].x = x * std::cos(rot) - y * std::sin(rot);
		q.vert[i].y = y * std::cos(rot) + x * std::sin(rot);
	}

	for (int i = 0; i < 4; i++)
	{
		q.vert[i].x += x;
		q.vert[i].y += y;
	}
	
	return q;
}
