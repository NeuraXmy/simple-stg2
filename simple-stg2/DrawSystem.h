#pragma once

#include <vector>
#include <functional>
#include <random>
#include <list>
#include <SFML/Graphics.hpp>

#include "Texture.h"
#include "GLDrawer.h"

class Sprite;


extern const sf::BlendMode sf_blendmodes[];

enum class TextAnchor
{
	left_buttom,
	right_buttom,
	left_top,
	right_top,
	center
};

namespace font
{
	extern sf::Font normal;
	extern sf::Font bold;
	extern sf::Font pixel;
}


/**
 * �ı��������ݷ�װ (������ΪĬ��ֵ)
 * str			: ASCII�ַ���
 * wstr			: ���ַ���
 * x			: �ı�x���� (0.0)
 * y			: �ı�y���� (0.0)
 * anchor		: �ı�ê�� (TextAnchor::left_buttom)
 * size			: �����С (16)
 * level		: ���Ʋ� (DrawLevel::gui)
 * color		: ��ɫ (Color(1, 1, 1))
 * edge			: �Ƿ�������� (false)
 * edgeColor	: �����ɫ (Color(0, 0, 0))
 * font			: ���� (&stdFont)
 */
struct TextDrawcall
{
	std::string str;
	std::wstring wstr;
	double x = 0.0;
	double y = 0.0;
	TextAnchor anchor = TextAnchor::left_top;
	int size = 16;
	DrawLevel level = DrawLevel::gui;
	Color color = Color(1.0, 1.0, 1.0);
	int edge = 0;
	Color edge_color = Color(0.0, 0.0, 0.0);
	sf::Font* font = &font::normal;

	void set_edge(const Color& c, int thickness = 1)
	{
		edge_color = c;
		edge = thickness;
	}
};


class DrawSystem final
{
public:

	static DrawSystem* instance();

	static void release_instance();


	// ����������
	void adjust_to_current_window();

	// ��ȡ��ȾĿ��
	sf::RenderTexture* get_target();

	// ��ȡ��ȾĿ��
	const sf::RenderTexture* get_target() const;


	// �����ӿ�
	void set_viewport(double x, double y, double w, double h);

	void set_view_to_game(double x_offset, double y_offset);

	void set_view_to_window();

	double get_scale() const;

	void set_scale(double scale);


	Texture::Coords* load_texcoords(const std::string& path);

	void load_texcoords_series(Texture::Coords* coords[], int count, const std::string& path);

	Texture::Texture* load_texture(const std::string& path);

	Sprite get_sprite(Texture::Texture* texture, Texture::Coords* coords);

	void clear_coords();

	void clear_textures();



	// ������ͼ
	void draw_sprite(const Sprite& sprite, double x, double y, double rot = 0.0);

	void draw_sprite(const Sprite& sprite, const Texture::Quad& quad);

	
	using GLDrawFunc = std::function<void(void)>;
	
	// ����opengl���� �����ڻ����������������
	void draw_gl_func(const GLDrawFunc& func);

	void draw_gl_func(GLDrawFunc&& func);


	using DrawFunc = std::function<void(sf::RenderTexture*)>;

	// ���ú����ڶ�Ӧ���Ʋ���ƣ����ڻ����ڶ�Ӧ������棩
	void draw_func(const DrawFunc& func, DrawLevel level = DrawLevel::top);

	// ���ú����ڶ�Ӧ���Ʋ���ƣ����ڻ����ڶ�Ӧ������棩
	void draw_func(DrawFunc&& func, DrawLevel level = DrawLevel::top);

	// �ڶ�Ӧ���Ʋ�������֣����ڻ����ڶ�Ӧ������棩
	void draw_text(const TextDrawcall& call);

	// ��ʼһ֡��Ⱦ
	void begin_frame();

	// ����һ֡��Ⱦ
	void end_frame();

	// �ύ���л�������
	void flush();

	// ������л�������
	void clear();

	// ���û��ƻ���
	void reset_buffer();


	// �����������
	void set_rand_seed(unsigned int seed);

	// ��ȡ��Χ���������
	int rand_int(int l, int r);

	// ��ȡ��Χ�����������
	double rand_real(double l, double r);


private:

	static const int draw_w = 640;
	static const int draw_h = 480;

	// Ϊ����Ԥ�ȷ��������
	static const int vert_initial_size = 4096;

	// ֡����
	int frame_count;

	sf::RenderTexture target;

	double scale = 1.0;

	// ��ע��Ĳ���
	std::list<Texture::Texture> textures;

	// ��ע��Ĳ�������
	std::list<Texture::Coords> coords;


	// ��ͬ�㺯����������
	std::vector<DrawFunc> func_drawcalls
		[(int)(DrawLevel::draw_level_num)];

	std::vector<GLDrawFunc> func_gl_drawcalls;


	std::default_random_engine rand_engine;


	bool draw_level_has_call
		[(int)(DrawLevel::draw_level_num)];

	std::vector<Texture::Texture*> call_tex_list
		[(int)(DrawLevel::draw_level_num)]
		[(int)(BlendMode::blend_mode_num)];


	DrawSystem();

	~DrawSystem();

	// ���ز�����������
	bool load_texcoord
	(
		const std::string& texcoord_path,
		std::vector<sf::IntRect>& frame_texcoords,
		std::vector<int>& frame_time
	);

};
