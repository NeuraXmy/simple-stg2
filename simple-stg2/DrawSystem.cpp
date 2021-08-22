#include <fstream>

#include "DrawSystem.h"
#include "WindowSystem.h"
#include "GameSystem.h"

#include "Settings.h"
#include "Sprite.h"

const double pi = acos(-1.0);


const sf::BlendMode sf_blendmodes[] =
{
	sf::BlendAlpha,
	sf::BlendAdd,
	sf::BlendMode(sf::BlendMode::Factor::OneMinusDstColor, sf::BlendMode::Factor::Zero),
	sf::BlendAlpha
};

namespace font
{
	sf::Font normal;
	sf::Font bold;
	sf::Font pixel;
}


static DrawSystem* instance = nullptr;

DrawSystem* DrawSystem::instance()
{
	if (!::instance)
	{
		::instance = new DrawSystem;
	}
	return ::instance;
}

void DrawSystem::release_instance()
{
	if (::instance)
	{
		delete ::instance;
	}
	::instance = nullptr;
}


DrawSystem::DrawSystem() :
	frame_count(0)
{
	font::normal.loadFromFile("res/font/SourceHanMonoSC-Normal.otf");
	font::bold.loadFromFile("res/font/SourceHanMonoSC-Bold.otf");
	font::pixel.loadFromFile("res/font/Freon.ttf");

	reset_buffer();
}

DrawSystem::~DrawSystem()
{
	clear_textures();
	clear_coords();
}


void DrawSystem::adjust_to_current_window()
{
	auto ws = WindowSystem::instance();
	if (ws->is_closed())
	{
		// todo log
		return;
	}
	target.create(ws->get_w(), ws->get_h());
	target.setSmooth(true);

	set_scale(std::min((double)ws->get_w() / draw_w, (double)ws->get_h() / draw_h));
	set_viewport(0, 0, draw_w, draw_h);
}

sf::RenderTexture* DrawSystem::get_target()
{
	return &target;
}

const sf::RenderTexture* DrawSystem::get_target() const
{
	return &target;
}

void DrawSystem::set_viewport(double x, double y, double w, double h)
{
	sf::View view = target.getView();
	view.setCenter(w * 0.5, h * 0.5);
	view.zoom(1.0 / scale);
	view.setSize(w, h);
	view.setViewport(sf::FloatRect(
		x / draw_w, y / draw_h,
		w / draw_w, h / draw_h
	));
	target.setView(view);
}

void DrawSystem::set_view_to_game(double x_offset, double y_offset)
{
	set_viewport(GameSystem::border_w + x_offset, GameSystem::border_w + y_offset, GameSystem::game_w, GameSystem::game_h);
}

void DrawSystem::set_view_to_window()
{
	set_viewport(0, 0, DrawSystem::draw_w, DrawSystem::draw_h);
}

double DrawSystem::get_scale() const
{
	return scale;
}

void DrawSystem::set_scale(double scale) 
{
	this->scale = scale;
}



Texture::Coords* DrawSystem::load_texcoords(const std::string& path)
{
	std::ifstream in(path);
	if (!in)
	{
		return nullptr;
	}

	coords.emplace_back();
	auto& coord = coords.back();

	int time, next, l, t, w, h;

	while(in >> time >> next >> l >> t >> w >> h)
	{
		coord.frame_time.push_back(time);
		coord.next_frame.push_back(next);
		coord.frame_texcoords.emplace_back(l, t, w, h);
	}

	in.close();
	return &coord;
}

void DrawSystem::load_texcoords_series(Texture::Coords* ret[], int count, const std::string& path)
{
	std::ifstream in(path);
	if (!in)
	{
		return;
	}

	coords.emplace_back();
	auto& coord = coords.back();

	int time, next, l, t, w, h;

	while (in >> time >> next >> l >> t >> w >> h)
	{
		coord.frame_time.push_back(time);
		coord.next_frame.push_back(next);
		coord.frame_texcoords.emplace_back(l, t, w, h);
		ret[0] = &coord;
	}

	int offset_x, offset_y;
	offset_x = time;
	offset_y = next;

	for (int i = 1; i < count; i++)
	{
		coords.push_back(coords.back());
		auto& coord = coords.back();
		for (auto& c : coord.frame_texcoords)
			c.left += offset_x, c.top += offset_y;
		ret[i] = &coord;
	}

	in.close();
}

Texture::Texture* DrawSystem::load_texture(const std::string& path)
{
	textures.emplace_back();
	auto& tex = textures.back();

	tex.sf_texture = new sf::Texture;
	if (!tex.sf_texture->loadFromFile(path))
	{
		delete tex.sf_texture;
		textures.pop_back();
		return nullptr;
	}

	tex.sf_texture->setSmooth(false);
	return &tex;
}

Sprite DrawSystem::get_sprite(Texture::Texture* texture, Texture::Coords* coords)
{
	Sprite sprite;
	sprite.set_texture(texture);
	sprite.set_coords(coords);
	return sprite;
}

void DrawSystem::clear_coords()
{
	coords.clear();
}

void DrawSystem::clear_textures()
{
	for (auto& tex : textures)
	{
		delete tex.sf_texture;
	}
	textures.clear();
}


void DrawSystem::draw_sprite(const Sprite& sprite, double x, double y, double rot)
{
	if (!sprite.ready() || !sprite.visible())
		return;

	auto tex = sprite.texture();
	
	tex->sf_texture->setRepeated(sprite.is_repeated());

	auto quad = sprite.quad(x, y, rot);

	int l = (int)(sprite.level());
	int m = (int)(sprite.blend());

	for (int i = 0; i < 4; i++)
		tex->verts[l][m].emplace_back(quad.vert[i], quad.color[i], quad.coords[i]);

	draw_level_has_call[l] = true;

	if (tex->verts[l][m].size() == 4)
		call_tex_list[l][m].push_back(tex);
}

void DrawSystem::draw_sprite(const Sprite& sprite, const Texture::Quad& quad)
{
	if (!sprite.ready() || !sprite.visible())
		return;

	auto tex = sprite.texture();

	tex->sf_texture->setRepeated(sprite.is_repeated());

	int l = (int)(sprite.level());
	int m = (int)(sprite.blend());

	for (int i = 0; i < 4; i++)
		tex->verts[l][m].emplace_back(quad.vert[i], quad.color[i], quad.coords[i]);

	draw_level_has_call[l] = true;

	if (tex->verts[l][m].size() == 4)
		call_tex_list[l][m].push_back(tex);
}


void DrawSystem::draw_gl_func(const GLDrawFunc& func)
{
	func_gl_drawcalls.push_back(func);
}

void DrawSystem::draw_gl_func(GLDrawFunc&& func)
{
	func_gl_drawcalls.push_back(std::move(func));
}


void DrawSystem::draw_func(const DrawFunc& func, DrawLevel level)
{
	func_drawcalls[(int)(level)].push_back(func);
	draw_level_has_call[(int)(level)] = true;
}

void DrawSystem::draw_func(DrawFunc&& func, DrawLevel level)
{
	func_drawcalls[(int)(level)].push_back(std::move(func));
	draw_level_has_call[(int)(level)] = true;
}

void DrawSystem::draw_text(const TextDrawcall& call)
{
	sf::Text text;

	if (call.wstr.size())
		text.setString(call.wstr.c_str());
	else
		text.setString(call.str.c_str());

	text.setCharacterSize(call.size);
	text.setFont(*call.font);
	text.setFillColor(call.color);
	if (call.edge)
	{
		text.setOutlineColor(call.edge_color);
		text.setOutlineThickness(call.edge);
	}

	double w = text.getLocalBounds().width;
	double h = text.getLocalBounds().height;

	switch (call.anchor)
	{
	case TextAnchor::left_buttom:
		text.setPosition(call.x,			call.y - h);		break;
	case TextAnchor::right_buttom:
		text.setPosition(call.x - w,		call.y - h);		break;
	case TextAnchor::left_top:
		text.setPosition(call.x,			call.y);			break;
	case TextAnchor::right_top:
		text.setPosition(call.x - w,		call.y);			break;
	case TextAnchor::center:
		text.setPosition(call.x - w * 0.5,	call.y - h * 0.5);	break;
	default:
		text.setPosition(call.x,			call.y);		    break;
	}

	draw_func(
		[text](sf::RenderTarget* t) 
		{ 
			t->draw(text); 
		}, 
	call.level);
}

void DrawSystem::begin_frame()
{
	target.clear();
	++frame_count;
}

void DrawSystem::end_frame()
{
	target.display();

	if (WindowSystem::instance()->is_open())
	{
		WindowSystem::instance()->get_window()->clear();

		sf::Sprite sprite(target.getTexture());
		if (WindowSystem::instance()->is_open())
		{
			WindowSystem::instance()->get_window()->draw(sprite, sf_blendmodes[(int)BlendMode::negative]);
		}

		WindowSystem::instance()->get_window()->display();
	}
}

void DrawSystem::flush()
{
	int t = frame_count;

	// draw gl
	if (func_gl_drawcalls.size())
	{
		target.setActive();
		target.pushGLStates();
		target.resetGLStates();

		GLDrawer::before_draw();

		GLDrawer::set_viewport(
			GameSystem::border_w * scale, GameSystem::border_w * scale,
			GameSystem::game_w * scale, GameSystem::game_h * scale);

		for (auto& func : func_gl_drawcalls)
		{
			GLDrawer::Camera::set_identity();
			GLDrawer::Camera::scale(1.0, GameSystem::game_w / float(GameSystem::game_h), 1.0);

			GLDrawer::Model::set_identity();

			func();
		}
		func_gl_drawcalls.clear();

		target.popGLStates();
	}

	// draw 2d
	{
		sf::RenderStates states;
		
		for (int i = 0; i < (int)(DrawLevel::draw_level_num); i++)
		{
			if (!draw_level_has_call[i])
			{
				continue;
			}
			draw_level_has_call[i] = false;

			for (int j = 0; j < (int)(BlendMode::blend_mode_num); j++)
			{
				auto& call_list = call_tex_list[i][j];

				if (!call_list.size())
					continue;

				states.blendMode = sf_blendmodes[j];

				for(auto tex : call_list)
				{
					int size = tex->verts[i][j].size();
					if (size)
					{
						states.texture = tex->sf_texture;
						target.draw(&tex->verts[i][j].at(0), size, sf::Quads, states);
						tex->verts[i][j].clear();
					}
				}

				call_list.clear();
			}

			for (auto& func : func_drawcalls[i])
			{
				func(&target);
			}
			func_drawcalls[i].clear();
		}
	}
}

void DrawSystem::clear()
{
	for (int level = 0; level < (int)(DrawLevel::draw_level_num); level++)
	{
		draw_level_has_call[level] = false;

		func_drawcalls[level].clear();

		for (int blend = 0; blend < (int)(BlendMode::blend_mode_num); blend++)
		{
			call_tex_list[level][blend].clear();

			for (auto& tex : textures)
			{
				tex.verts[level][blend].clear();
			}
		}
	}
}

void DrawSystem::reset_buffer()
{
	clear();

	for (auto& tex : textures)
		for (int level = 0; level < (int)(DrawLevel::draw_level_num); level++)
			for (int blend = 0; blend < (int)(BlendMode::blend_mode_num); blend++) 
			{
				tex.verts[level][blend].clear();
				tex.verts[level][blend].shrink_to_fit();
			}
}


void DrawSystem::set_rand_seed(unsigned int seed)
{
	rand_engine.seed(seed);
}

int DrawSystem::rand_int(int l, int r)
{
	if (l > r)
		std::swap(l, r);
	std::uniform_int_distribution<int> d(l, r);
	return d(rand_engine);
}

double DrawSystem::rand_real(double l, double r)
{
	if (l > r)
		std::swap(l, r);
	std::uniform_real_distribution<double> d(l, r);
	return d(rand_engine);
}