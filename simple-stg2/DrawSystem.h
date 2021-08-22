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
 * 文本绘制数据封装 (括号内为默认值)
 * str			: ASCII字符串
 * wstr			: 宽字符串
 * x			: 文本x坐标 (0.0)
 * y			: 文本y坐标 (0.0)
 * anchor		: 文本锚点 (TextAnchor::left_buttom)
 * size			: 字体大小 (16)
 * level		: 绘制层 (DrawLevel::gui)
 * color		: 颜色 (Color(1, 1, 1))
 * edge			: 是否启用描边 (false)
 * edgeColor	: 描边颜色 (Color(0, 0, 0))
 * font			: 字体 (&stdFont)
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


	// 调整到窗口
	void adjust_to_current_window();

	// 获取渲染目标
	sf::RenderTexture* get_target();

	// 获取渲染目标
	const sf::RenderTexture* get_target() const;


	// 设置视口
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



	// 绘制贴图
	void draw_sprite(const Sprite& sprite, double x, double y, double rot = 0.0);

	void draw_sprite(const Sprite& sprite, const Texture::Quad& quad);

	
	using GLDrawFunc = std::function<void(void)>;
	
	// 绘制opengl函数 总是在绘制在其他层的下面
	void draw_gl_func(const GLDrawFunc& func);

	void draw_gl_func(GLDrawFunc&& func);


	using DrawFunc = std::function<void(sf::RenderTexture*)>;

	// 调用函数在对应绘制层绘制（将在绘制在对应层的上面）
	void draw_func(const DrawFunc& func, DrawLevel level = DrawLevel::top);

	// 调用函数在对应绘制层绘制（将在绘制在对应层的上面）
	void draw_func(DrawFunc&& func, DrawLevel level = DrawLevel::top);

	// 在对应绘制层绘制文字（将在绘制在对应层的上面）
	void draw_text(const TextDrawcall& call);

	// 开始一帧渲染
	void begin_frame();

	// 结束一帧渲染
	void end_frame();

	// 提交所有绘制请求
	void flush();

	// 清除所有绘制请求
	void clear();

	// 重置绘制缓存
	void reset_buffer();


	// 设置随机种子
	void set_rand_seed(unsigned int seed);

	// 获取范围内随机整数
	int rand_int(int l, int r);

	// 获取范围内随机浮点数
	double rand_real(double l, double r);


private:

	static const int draw_w = 640;
	static const int draw_h = 480;

	// 为顶点预先分配的数量
	static const int vert_initial_size = 4096;

	// 帧计数
	int frame_count;

	sf::RenderTexture target;

	double scale = 1.0;

	// 已注册的材质
	std::list<Texture::Texture> textures;

	// 已注册的材质坐标
	std::list<Texture::Coords> coords;


	// 不同层函数绘制请求
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

	// 加载材质坐标数据
	bool load_texcoord
	(
		const std::string& texcoord_path,
		std::vector<sf::IntRect>& frame_texcoords,
		std::vector<int>& frame_time
	);

};
