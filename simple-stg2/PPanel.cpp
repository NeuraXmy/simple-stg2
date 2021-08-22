#include "PPanel.h"
#include "GameSystem.h"
#include "PResource.h"
#include "DrawSystem.h"
#include <sstream>
#include "Boss.h"


PPanel::PPanel()
{
	panel.set_tex_and_coords(tex.panel, coords.panel);
	panel.set_level(DrawLevel::background_1);

	line.set_tex_and_coords(tex.panel_widget, coords.panel_line);
	line.set_w_scale(0.7);
	line.set_level(DrawLevel::background_2);

	double lb_s = 0.3;

	no_bomb.set_tex_and_coords(tex.panel_widget, coords.panel_no_bomb);
	no_bomb.set_scale(lb_s, lb_s);
	bomb.set_tex_and_coords(tex.panel_widget, coords.panel_bomb);
	bomb.set_scale(lb_s, lb_s);
	no_life.set_tex_and_coords(tex.panel_widget, coords.panel_no_life);
	no_life.set_scale(lb_s, lb_s);
	life.set_tex_and_coords(tex.panel_widget, coords.panel_life);
	life.set_scale(lb_s, lb_s);

	boss_indicator.set_tex_and_coords(tex.panel_widget, coords.boss_indicator);
	boss_indicator.set_alpha(0.5);
}

void PPanel::on_draw()
{
	auto ds = DrawSystem::instance();
	auto gs = GameSystem::instance();
	auto player = gs->get_player();

	const int pw = gs->ui_w - gs->game_w - gs->border_w * 3;
	const int ph = gs->ui_h - gs->border_w * 2;
	const int px = gs->game_w + gs->border_w * 2;
	const int py = gs->border_w;

	int t = gs->t();

	line.set_alpha((abs(sin(t * 0.02) * 0.25) + 0.75) * 0.5);

	// bg
	{
		ds->draw_sprite(panel, gs->ui_w / 2, gs->ui_h / 2);
	}

	// diff
	{
		TextDrawcall t;
		t.str = "DEMO", t.font = &font::pixel;
		t.size = 20, t.anchor = TextAnchor::center;
		t.x = px + pw / 2, t.y = py + 20;
		t.set_edge(Color(0.5, 0.2, 1.0), 2);
		ds->draw_text(t);
	}

	// high score
	{
		TextDrawcall t;
		t.wstr = L"历史";
		t.size = 17, t.anchor = TextAnchor::right_top;
		t.x = px + 40, t.y = py + 50;
		t.set_edge(Color(0.5, 0.5, 0.5), 1);
		ds->draw_text(t);

		int hscore = 0;
		std::string str;
		str.resize(10);
		for (int i = 1; i <= 10; i++)
			str[10 - i] = hscore % 10 + '0', hscore /= 10;

		t.str = str, t.wstr.clear();
		t.size = 19, t.anchor = TextAnchor::left_top;
		t.x = px + 70, t.y = py + 49;
		t.set_edge(Color(0.5, 0.5, 0.5), 1);
		ds->draw_text(t);
	}

	// score
	{
		TextDrawcall t;
		t.wstr = L"得分";
		t.size = 17, t.anchor = TextAnchor::right_top;
		t.x = px + 40, t.y = py + 80;
		t.set_edge(Color(0.5, 0.5, 0.5), 1);
		ds->draw_text(t);

		int score = player ? player->get_score() : 0;
		std::string str;
		str.resize(10);
		for (int i = 1; i <= 10; i++)
			str[10 - i] = score % 10 + '0', score /= 10;

		t.str = str, t.wstr.clear();
		t.size = 19, t.anchor = TextAnchor::left_top;
		t.x = px + 70, t.y = py + 79;
		t.set_edge(Color(0.5, 0.5, 0.5), 1);
		ds->draw_text(t);

		ds->draw_sprite(line, px + pw / 2, py + 100);
	}

	// life
	{
		TextDrawcall t;
		t.wstr = L"残机";
		t.size = 17, t.anchor = TextAnchor::right_top;
		t.x = px + 40, t.y = py + 125;
		t.set_edge(Color(1.0, 0.2, 0.5), 1);
		ds->draw_text(t);

		int limit = player ? player->get_life_limit() : 0;
		int count = player ? player->get_life_count() : 0;
		for (int i = 1; i <= limit; i++)
		{
			auto& s = (i <= count ? life : no_life);
			ds->draw_sprite(s, px + 70 + (i - 1) * 21, py + 133);
		}

		ds->draw_sprite(line, px + pw / 2, py + 145);
	}

	// bomb
	{
		TextDrawcall t;
		t.wstr = L"符卡";
		t.size = 17, t.anchor = TextAnchor::right_top;
		t.x = px + 40, t.y = py + 155;
		t.set_edge(Color(0.2, 1.0, 0.2), 1);
		ds->draw_text(t);

		int limit = player ? player->get_bomb_limit() : 0;
		int count = player ? player->get_bomb_count() : 0;
		for (int i = 1; i <= limit; i++)
		{
			auto& s = (i <= count ? bomb : no_bomb);
			ds->draw_sprite(s, px + 70 + (i - 1) * 21, py + 163);
		}

		ds->draw_sprite(line, px + pw / 2, py + 175);
	}

	// power
	{
		TextDrawcall t;
		t.wstr = L"灵力";
		t.size = 17, t.anchor = TextAnchor::right_top;
		t.x = px + 40, t.y = py + 200;
		t.set_edge(Color(1.0, 0.2, 0.2), 1);
		ds->draw_text(t);

		int power = player ? player->get_power() : 0;
		int limit = player ? player->get_max_power() : 0;

		std::ostringstream out;
		out << power / 100 << '.' << power / 10 % 10 << power % 10 << "/"
			<< limit / 100 << '.' << limit / 10 % 10 << limit % 10;

		t.wstr.clear();
		t.str = out.str();
		t.size = 17, t.anchor = TextAnchor::left_top;
		t.x = px + 90, t.y = py + 200;
		t.set_edge(Color(1.0, 0.2, 0.2), 1);
		ds->draw_text(t);

		ds->draw_sprite(line, px + pw / 2, py + 220);
	}

	// point score
	{
		TextDrawcall t;
		t.wstr = L"最大得点";
		t.size = 17, t.anchor = TextAnchor::right_top;
		t.x = px + 75, t.y = py + 230;
		t.set_edge(Color(0.2, 0.2, 1.0), 1);
		ds->draw_text(t);

		int score = player ? player->get_point_score() : 0;

		t.wstr.clear();
		t.str = std::to_string(score);
		t.size = 17, t.anchor = TextAnchor::left_top;
		t.x = px + 90, t.y = py + 230;
		t.set_edge(Color(0.5, 0.5, 0.5), 1);
		ds->draw_text(t);

		ds->draw_sprite(line, px + pw / 2, py + 250);
	}

	// graze
	{
		TextDrawcall t;
		t.wstr = L"Graze";
		t.size = 12, t.anchor = TextAnchor::right_top;
		t.x = px + 50, t.y = py + 255;
		t.set_edge(Color(0.4, 0.4, 0.4), 1);
		ds->draw_text(t);

		int graze = player ? player->get_graze() : 0;

		t.wstr.clear();
		t.str = std::to_string(graze);
		t.size = 12, t.anchor = TextAnchor::left_top;
		t.x = px + 70, t.y = py + 255;
		t.set_edge(Color(0.4, 0.4, 0.4), 1);
		ds->draw_text(t);
	}

	// boss indicator
	{
		if (auto a = gs->get_current_boss_attack())
		{
			if (auto b = a->get_boss())
			{
				ds->draw_sprite(boss_indicator, b->x() + gs->border_w, gs->game_h + gs->border_w + 5);
			}
		}
	}

}
