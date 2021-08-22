#pragma once
#include <iostream>
#include <sstream>
#include <SFML/OpenGL.hpp>

#include "DrawSystem.h"
#include "InputSystem.h"
#include "GameSystem.h"
#include "WindowSystem.h"
#include "ObjectSystem.h"
#include "InputListener.h"
#include "Application.h"
#include "Settings.h"
#include "GameObject.h"
#include "PhysicObject.h"
#include "SpriteObject.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"
#include "Player.h"
#include "Stage.h"
#include "Shooter.h"
#include "Enemy.h"
#include "Boss.h"
#include "Background.h"
#include "Movement.h"
#include "GamePanel.h"
#include "Item.h"
#include "Laser.h"
#include "SoundSystem.h"


namespace Test
{
	using utils::pi;


	namespace tex
	{
		inline Texture::Texture* bullet;
		inline Texture::Texture* player;
		inline Texture::Texture* enemy;
		inline Texture::Texture* boss_1;
		inline Texture::Texture* sc_title;
		inline Texture::Texture* sc_stripe;
		inline Texture::Texture* magic_circle;
		inline Texture::Texture* bg_1;
		inline Texture::Texture* bg_2;
		inline Texture::Texture* panel;
		inline Texture::Texture* panel_widget;
		inline Texture::Texture* item;
		inline Texture::Texture* bomb_title;
	}

	namespace coords
	{
		inline Texture::Coords* bullet_star_small_yellow;
		inline Texture::Coords* bullet_star_small_blue;
		inline Texture::Coords* player;
		inline Texture::Coords* hit_point;
		inline Texture::Coords* bullet_kill_particle;
		inline Texture::Coords* player_bullet_1;
		inline Texture::Coords* player_bullet_2;
		inline Texture::Coords* player_bullet_3;
		inline Texture::Coords* player_drone_1;
		inline Texture::Coords* player_drone_2;
		inline Texture::Coords* enemy_small_1;
		inline Texture::Coords* boss_1;
		inline Texture::Coords* sc_title;
		inline Texture::Coords* sc_stripe;
		inline Texture::Coords* magic_circle;
		inline Texture::Coords* bg_1;
		inline Texture::Coords* bg_2;
		inline Texture::Coords* panel;
		inline Texture::Coords* panel_line;
		inline Texture::Coords* panel_no_bomb;
		inline Texture::Coords* panel_bomb;
		inline Texture::Coords* panel_no_life;
		inline Texture::Coords* panel_life;
		inline Texture::Coords* boss_indicator;
		inline Texture::Coords* item_power;
		inline Texture::Coords* item_power_ind;
		inline Texture::Coords* item_power_big;
		inline Texture::Coords* item_power_big_ind;
		inline Texture::Coords* item_point;
		inline Texture::Coords* item_point_ind;
		inline Texture::Coords* item_full;
		inline Texture::Coords* item_full_ind;
		inline Texture::Coords* item_life;
		inline Texture::Coords* item_life_ind;
		inline Texture::Coords* item_bomb;
		inline Texture::Coords* item_bomb_ind;
		inline Texture::Coords* item_point_small;
		inline Texture::Coords* bomb_title;
		inline Texture::Coords* laser_blue;
		inline Texture::Coords* light_blue;
	}

	namespace se
	{
		Sound::Se* graze;
		Sound::Se* enemy_death;
	}

	namespace bgm
	{
		Sound::Bgm* bgm_1;
	}


	class TestStage final : public Stage
	{
		void load_res()
		{
			auto gs = DrawSystem::instance();
			auto ss = SoundSystem::instance();

			tex::bullet						= gs->load_texture("res/tex/bullet/bullets.png");
			tex::player						= gs->load_texture("res/tex/player/player.png");
			tex::enemy						= gs->load_texture("res/tex/enemy/enemy.png");
			tex::boss_1						= gs->load_texture("res/tex/boss/boss_1.png");
			tex::sc_title					= gs->load_texture("res/tex/boss/title.png");
			tex::sc_stripe					= gs->load_texture("res/tex/boss/sc_stripe.png");
			tex::magic_circle				= gs->load_texture("res/tex/boss/magic_circle.png");
			tex::bg_1						= gs->load_texture("res/tex/bg/bg_1.png");
			tex::bg_2						= gs->load_texture("res/tex/bg/bg_2.png");
			tex::panel						= gs->load_texture("res/tex/gui/panel.png");
			tex::panel_widget				= gs->load_texture("res/tex/gui/panel_widget.png");
			tex::item						= gs->load_texture("res/tex/item/item.png");
			tex::bomb_title					= gs->load_texture("res/tex/player/bomb_title.png");

			coords::bullet_star_small_yellow	= gs->load_texcoords("res/tex/bullet/star_small_yellow.txc");
			coords::bullet_star_small_blue		= gs->load_texcoords("res/tex/bullet/star_small_blue.txc");
			coords::player						= gs->load_texcoords("res/tex/player/player.txc");
			coords::hit_point					= gs->load_texcoords("res/tex/player/hit_point.txc");
			coords::bullet_kill_particle		= gs->load_texcoords("res/tex/bullet/bullet_kill_particle.txc");
			coords::player_bullet_1				= gs->load_texcoords("res/tex/player/player_bullet_1.txc");
			coords::player_bullet_2				= gs->load_texcoords("res/tex/player/player_bullet_2.txc");
			coords::player_bullet_3				= gs->load_texcoords("res/tex/player/player_bullet_3.txc");
			coords::player_drone_1				= gs->load_texcoords("res/tex/player/player_drone_1.txc");
			coords::player_drone_2				= gs->load_texcoords("res/tex/player/player_drone_2.txc");
			coords::enemy_small_1				= gs->load_texcoords("res/tex/enemy/enemy_small_1.txc");
			coords::boss_1						= gs->load_texcoords("res/tex/boss/boss_1.txc");
			coords::sc_title					= gs->load_texcoords("res/tex/boss/title.txc");
			coords::sc_stripe					= gs->load_texcoords("res/tex/boss/sc_stripe.txc");
			coords::magic_circle				= gs->load_texcoords("res/tex/boss/magic_circle.txc");
			coords::bg_1						= gs->load_texcoords("res/tex/bg/bg_1.txc");
			coords::bg_2						= gs->load_texcoords("res/tex/bg/bg_2.txc");
			coords::panel						= gs->load_texcoords("res/tex/gui/panel.txc");
			coords::panel_line					= gs->load_texcoords("res/tex/gui/line.txc");
			coords::panel_bomb					= gs->load_texcoords("res/tex/gui/bomb.txc");
			coords::panel_no_bomb				= gs->load_texcoords("res/tex/gui/no_bomb.txc");
			coords::panel_life					= gs->load_texcoords("res/tex/gui/life.txc");
			coords::panel_no_life				= gs->load_texcoords("res/tex/gui/no_life.txc");
			coords::boss_indicator				= gs->load_texcoords("res/tex/gui/boss_indicator.txc");
			coords::item_power					= gs->load_texcoords("res/tex/item/power.txc");
			coords::item_power_ind				= gs->load_texcoords("res/tex/item/power_ind.txc");
			coords::item_power_big				= gs->load_texcoords("res/tex/item/power_big.txc");
			coords::item_power_big_ind			= gs->load_texcoords("res/tex/item/power_big_ind.txc");
			coords::item_point					= gs->load_texcoords("res/tex/item/point.txc");
			coords::item_point_ind			    = gs->load_texcoords("res/tex/item/point_ind.txc");
			coords::item_full					= gs->load_texcoords("res/tex/item/full.txc");
			coords::item_full_ind				= gs->load_texcoords("res/tex/item/full_ind.txc");
			coords::item_life				    = gs->load_texcoords("res/tex/item/life.txc");
			coords::item_life_ind				= gs->load_texcoords("res/tex/item/life_ind.txc");
			coords::item_bomb					= gs->load_texcoords("res/tex/item/bomb.txc");
			coords::item_bomb_ind				= gs->load_texcoords("res/tex/item/bomb_ind.txc");
			coords::item_point_small			= gs->load_texcoords("res/tex/item/point_small.txc");
			coords::bomb_title					= gs->load_texcoords("res/tex/player/bomb_title.txc");
			coords::laser_blue					= gs->load_texcoords("res/tex/bullet/laser_blue.txc");
			coords::light_blue					= gs->load_texcoords("res/tex/bullet/light_blue.txc");

			// bgm::bgm_1						= ss->load_bgm("res/bgm/bgm_1.wav", "res/bgm/bgm_1.inf");

			se::graze							= ss->load_se("res/se/se_graze.wav");
		}

		// ------------------------------------- prefabs --------------------------------- // 

		template<int color>
		class ESmall : public Enemy
		{
		protected:
			virtual void on_create() override
			{
				Enemy::on_create();

				if (color == 1)
					set_sprite(tex::enemy, coords::enemy_small_1);

				set_still_frame_index(0);
				set_left_move_frame_index(1);
				set_right_move_frame_index(2);
				set_still_from_left_frame_index(3);
				set_still_from_right_frame_index(4);

				set_collider_r(10.0);
				set_max_health(60);

				{
					kill_particle().emplace_back();
					auto& p = this->kill_particle().back();
					p.lifeTime = { 15, 45 }, p.cnt = 10;
					p.scale = { 2.0, 3.0 }, p.rand_pos_r = 5.0;
					p.rand_v_direction = true, p.v = { 3.0, 5.0 };
					p.omega = { -0.5, 0.5 }, p.rot = { 0.0, pi * 2.0 };
					p.set_option(Particle::Options::fade_out);
					p.set_option(Particle::Options::shrink);
					p.set_option(Particle::Options::slow_down);
					p.sprite.set_tex_and_coords(tex::bullet, coords::bullet_kill_particle);
					p.sprite.set_color(Color(0.9, 0.9, 1.0, 1.0));
					p.sprite.set_level(DrawLevel::enemy);
				}
			}
		public:
			virtual ~ESmall() = default;
		};

		class SpellCard : public BossAttack
		{
		protected:
			virtual void on_create() override
			{
				BossAttack::on_create();
				set_is_spellcard(true);
				set_max_health(2000);
				set_time_limit(600);
				set_prepare_time(120);

				spellcard_stripe().set_tex_and_coords(tex::sc_stripe, coords::sc_stripe);
				spellcard_stripe().set_scale(10.0, 0.5);
				spellcard_stripe().set_tex_scale(20.0, 1.0);

				title_sprite().set_tex_and_coords(tex::sc_title, coords::sc_title);
				title_sprite().set_scale(0.6, 0.6);

				set_charge_before_start(true);
			}
		public:
			virtual ~SpellCard() = default;
		};

		class Panel final : public GamePanel
		{
			Sprite panel;
			Sprite line;
			Sprite no_bomb;
			Sprite bomb;
			Sprite no_life;
			Sprite life;
			Sprite boss_indicator;

		public:

			Panel()
			{
				panel.set_tex_and_coords(tex::panel, coords::panel);
				panel.set_level(DrawLevel::background_1);

				line.set_tex_and_coords(tex::panel_widget, coords::panel_line);
				line.set_w_scale(0.7);
				line.set_level(DrawLevel::background_2);

				double lb_s = 0.3;

				no_bomb.set_tex_and_coords(tex::panel_widget, coords::panel_no_bomb);
				no_bomb.set_scale(lb_s, lb_s);
				bomb.set_tex_and_coords(tex::panel_widget, coords::panel_bomb);
				bomb.set_scale(lb_s, lb_s);
				no_life.set_tex_and_coords(tex::panel_widget, coords::panel_no_life);
				no_life.set_scale(lb_s, lb_s);
				life.set_tex_and_coords(tex::panel_widget, coords::panel_life);
				life.set_scale(lb_s, lb_s);

				boss_indicator.set_tex_and_coords(tex::panel_widget, coords::boss_indicator);
				boss_indicator.set_alpha(0.5);
			}

		private:

			void on_draw() override
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

					ds->draw_sprite(line,px + pw / 2, py + 145);
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
		};

		// ------------------------------------- items ----------------------------------- //

		class SmallItem : public Item
		{
		public:
			virtual	~SmallItem() = default;
		protected:
			virtual void on_create() override
			{
				Item::on_create();
				set_pick_radius(50.0);
				set_start_info(-2.5, 30, true, true);
				set_drop_info(2.0, 0.05, 30);
			}
		};

		class BigItem : public Item
		{
		public:
			virtual	~BigItem() = default;
		protected:
			virtual void on_create() override
			{
				Item::on_create();
				sprite().set_level(DrawLevel::item_2);
				set_pick_radius(80.0);
				set_start_info(-2.5, 30, true, true);
				set_drop_info(2.0, 0.05, 30);
			}
		};

		class PowerItem final : public SmallItem
		{
			void on_create() override
			{
				SmallItem::on_create();
				sprite().set_tex_and_coords(tex::item, coords::item_power);
				indicator().set_tex_and_coords(tex::item, coords::item_power_ind);
			}
			void on_pick(Player* player) override
			{
				SmallItem::on_pick(player);
				if (player->is_full_power())
					player->add_score(1000);
				else
					player->add_power(1);
			}
		};

		class PointItem final : public SmallItem
		{
			void on_create() override
			{
				SmallItem::on_create();
				sprite().set_tex_and_coords(tex::item, coords::item_point);
				indicator().set_tex_and_coords(tex::item, coords::item_point_ind);
			}
			void on_pick(Player* player) override
			{
				SmallItem::on_pick(player);
				double k = utils::clamp(1.5 - player->y() / GameSystem::game_h, 0.0, 1.0);
				player->add_score(player->get_point_score() * k);
			}
		};

		class PowerBigItem final : public BigItem
		{
			void on_create() override
			{
				BigItem::on_create();
				sprite().set_tex_and_coords(tex::item, coords::item_power_big);
				sprite().set_scale(0.8, 0.8);
				indicator().set_tex_and_coords(tex::item, coords::item_power_big_ind);
			}
			void on_pick(Player* player) override
			{
				BigItem::on_pick(player);
				if (player->is_full_power())
					player->add_score(500);
				else
					player->add_power(50);
			}
		};

		class FullItem final : public BigItem
		{
			void on_create() override
			{
				BigItem::on_create();
				sprite().set_tex_and_coords(tex::item, coords::item_full);
				sprite().set_scale(0.8, 0.8);
				indicator().set_tex_and_coords(tex::item, coords::item_full_ind);
			}
			void on_pick(Player* player) override
			{
				BigItem::on_pick(player);
				if (player->is_full_power())
					player->add_score(10000);
				else
					player->set_power(player->get_max_power());
			}
		};

		class LifeItem final : public BigItem
		{
			void on_create() override
			{
				BigItem::on_create();
				sprite().set_tex_and_coords(tex::item, coords::item_life);
				sprite().set_scale(1.5, 1.5);
				indicator().set_tex_and_coords(tex::item, coords::item_life_ind);
			}
			void on_pick(Player* player) override
			{
				player->extend_life();
			}
		};

		class BombItem final : public BigItem
		{
			void on_create() override
			{
				BigItem::on_create();
				sprite().set_tex_and_coords(tex::item, coords::item_bomb);
				sprite().set_scale(1.5, 1.5);
				indicator().set_tex_and_coords(tex::item, coords::item_bomb_ind);
			}
			void on_pick(Player* player) override
			{
				player->extend_bomb();
			}
		};

		class PointSmallItem final : public SmallItem
		{
			void on_create() override
			{
				SmallItem::on_create();
				set_start_info(-3.5, 60, false, false);
				sprite().set_tex_and_coords(tex::item, coords::item_point_small);
				sprite().set_alpha(0.5);
				sprite().set_blend(BlendMode::plus);
				if (auto p = GameSystem::instance()->get_player())
					pick(p);
			}
			void on_pick(Player* player) override
			{
				SmallItem::on_pick(player);
				player->add_point_score(10);
			}
		};


		// ------------------------------------- objects --------------------------------- // 

		class P final : public Player
		{
			class MainGun final : public Shooter
			{
				class B final : public PlayerBullet
				{
					void on_create() override
					{
						PlayerBullet::on_create();
						set_damage(20);
						sprite().set_alpha(0.6);
						set_collider_r(6.0);
						set_sprite(tex::player, coords::player_bullet_1);
					}
				};

				void on_create() override
				{
					Shooter::on_create();
					set_shot_interval(6);
				}
				void on_shoot() override
				{
					Shooter::on_shoot();
					ShotData d; 
					d.v = 15.0, d.rot_offset = -pi * 0.5;
					d.y_offset = -5.0;
					d.x_offset = -10.0, shoot<B>(d);
					d.x_offset =  10.0, shoot<B>(d);
				}
			};

			struct Pos { double x, y;  };
			static Pos get_p(double a) { return { 40.0 * cos(a), 40.0 * sin(a) }; }
			// dp[fast/slow][mode-1][drone]
			static inline const double pi = std::acos(-1.0);
			static inline const Pos dp[2][4][4] = {
				{
					{ get_p(pi * 0.5) },
					{ get_p(pi * 0.3),  get_p(pi * 0.7) },
					{ get_p(pi * 0.25), get_p(pi * 0.5),  get_p(pi * 0.75) },
					{ get_p(pi * 0.2),  get_p(pi * 0.4) , get_p(pi * 0.6), get_p(pi * 0.8) },
				},
				{
					{ get_p(-pi * 0.5) },
					{ get_p(-pi * 0.3),  get_p(-pi * 0.7) },
					{ get_p(-pi * 0.25), get_p(-pi * 0.5),  get_p(-pi * 0.75) },
					{ get_p(-pi * 0.2),  get_p(-pi * 0.4) , get_p(-pi * 0.6), get_p(-pi * 0.8) },
				}
			};
			static Pos lerp(const Pos& a, const Pos& b, double k) {
				return { a.x * (1.0 - k) + b.x * k, a.y * (1.0 - k) + b.y * k };
			}

			class DroneSetFast final : public DroneSet
			{
				class D final : public Shooter
				{
					double rot;
					class B final : public PlayerBullet
					{
						void on_create() override
						{
							PlayerBullet::on_create();
							set_damage(5);
							sprite().set_alpha(0.3);
							set_drag(10), set_drag_interval(1), set_drag_alpha(0.6);
							set_sprite(tex::player, coords::player_bullet_2);
							set_boundary(40, 40, 40, 40);
							set_collider_r(6.0);
							sprite().set_blend(BlendMode::plus);
						}
						void on_update() override
						{
							PlayerBullet::on_update();

							double k = 0.0;
							if (t() <= 10)
								k = t() / 10.0 * 0.2;
							else if (t() <= 130)
								k = (130 - t()) / 120.0 * 0.2;
							else 
								k = 0.0;

							PhysicObject* enemy = nullptr;
							double dmin = 1e17;
							ObjectSystem::instance()->foreach<Enemy>([&](Enemy* e) {
								if (!e->is_invincible() && this->get_dist(e) < dmin)
									enemy = e, dmin = this->get_dist(e);
							});
							ObjectSystem::instance()->foreach<Boss>([&](Boss* b) {
								if (!b->is_invincible() && this->get_dist(b) < dmin)
									enemy = b, dmin = this->get_dist(b);
								});
							if (enemy)
							{
								double rot = get_point_to_rot(enemy->x() + enemy->vx(), enemy->y() + enemy->vy());
								set_rot(utils::lerp_angle(this->rot(), rot, k));
								set_v_to_rot();
							}
						}
					};

					void on_create() override
					{
						Shooter::on_create();
						set_sprite(tex::player, coords::player_drone_1);
						set_shot_interval(9);
						set_follow_rot(false);
						set_omega(0.1);
					}
					void on_shoot() override
					{
						Shooter::on_shoot();
						ShotData d;	d.v = 8.0, d.rot_offset = rot;
						d.r_offset = -5.0, shoot<B>(d);
					}
				public:	
					D(double rot) : rot(rot) {}
				};

				void on_change_mode() override
				{
					DroneSet::on_change_mode();
					remove_all();	int m = get_mode();
					auto set_p = [](D* d, double rot) { d->set_pos(40.0 * cos(rot), 40.0 * sin(rot)); };
					if (m == 1)
					{
						set_p(append_drone<D>(-pi * 0.5), pi * 0.5);
					}
					if (m == 2)
					{
						set_p(append_drone<D>(-pi * 0.3), pi * 0.3);
						set_p(append_drone<D>(-pi * 0.7), pi * 0.7);
					}
					if (m == 3)
					{
						set_p(append_drone<D>(-pi * 0.3), pi * 0.3);
						set_p(append_drone<D>(-pi * 0.5), pi * 0.5);
						set_p(append_drone<D>(-pi * 0.7), pi * 0.7);
					}
					if (m == 4)
					{
						set_p(append_drone<D>(-pi * 0.2), pi * 0.2);
						set_p(append_drone<D>(-pi * 0.4), pi * 0.4);
						set_p(append_drone<D>(-pi * 0.6), pi * 0.6);
						set_p(append_drone<D>(-pi * 0.8), pi * 0.8);
					}
				}

				void on_show_hide(double k) override
				{
					DroneSet::on_show_hide(k);
					if (!is_all_show()) return;
					auto m = get_mode();
					for (int i = 0; i < m; i++)
					{
						auto p = lerp(dp[1][m - 1][m - i - 1], dp[0][m - 1][i], k);
						drone(i)->set_pos(p.x, p.y);
						drone(i)->set_blocked_shoot(k < 0.5);
					}
				}
			};

			class DroneSetSlow final : public DroneSet
			{
				class D final : public Shooter
				{
					class B final : public PlayerBullet
					{
						void on_create() override
						{
							PlayerBullet::on_create();
							set_damage(5);
							sprite().set_alpha(0.6);
							set_sprite(tex::player, coords::player_bullet_3);
							set_boundary(40, 40, 40, 40);
							set_collider_r(6.0);
						}
					};

					void on_create() override
					{
						Shooter::on_create();
						set_sprite(tex::player, coords::player_drone_2);
						set_shot_interval(5);
						set_follow_rot(false);
						set_omega(0.1);
					}
					void on_shoot() override
					{
						Shooter::on_shoot();
						ShotData d;	d.v = 15.0, d.rot_offset = -pi * 0.5;
						d.y_offset = -5.0;
						d.x_offset = -5.0, shoot<B>(d);
						d.x_offset = 5.0, shoot<B>(d);
					}
				};

				void on_change_mode() override
				{
					DroneSet::on_change_mode();
					remove_all();	int m = get_mode();
					auto set_p = [](D* d, double rot) { d->set_pos(40.0 * cos(rot), 40.0 * sin(rot)); };
					if (m == 1)
					{
						set_p(append_drone<D>(), -pi * 0.5);
					}
					if (m == 2)
					{
						set_p(append_drone<D>(), -pi * 0.3);
						set_p(append_drone<D>(), -pi * 0.7);
					}
					if (m == 3)
					{
						set_p(append_drone<D>(), -pi * 0.3);
						set_p(append_drone<D>(), -pi * 0.5);
						set_p(append_drone<D>(), -pi * 0.7);
					}
					if (m == 4)
					{
						set_p(append_drone<D>(), -pi * 0.2);
						set_p(append_drone<D>(), -pi * 0.4);
						set_p(append_drone<D>(), -pi * 0.6);
						set_p(append_drone<D>(), -pi * 0.8);
					}
				}

				void on_show_hide(double k) override
				{
					DroneSet::on_show_hide(k);
					if (!is_all_show()) return;
					auto m = get_mode();
					for (int i = 0; i < m; i++)
					{
						auto p = lerp(dp[0][m - 1][m - i - 1], dp[1][m - 1][i], k);
						drone(i)->set_pos(p.x, p.y);
						drone(i)->set_blocked_shoot(k < 0.5);
					}
				}
			};

			class FB final : public Bomb
			{
				class S : public Shooter
				{
					class B final : public PlayerBullet
					{
						void on_create() override
						{
							PlayerBullet::on_create();
							set_damage(3);
							set_penetrate(true);
							sprite().set_alpha(0.5);
							sprite().set_scale(1.8, 1.8);
							set_drag(10), set_drag_interval(1), set_drag_alpha(0.8), set_drag_scale(0.8);
							set_sprite(tex::player, coords::player_bullet_2);
							set_boundary(40, 40, 40, 40);
							set_collider_r(20.0);
							sprite().set_blend(BlendMode::plus);
							set_can_kill_enemy_bullet(true);
						}
						void on_update() override
						{
							PlayerBullet::on_update();
							set_damage(3 + t() / 5);
						}
					};

					void on_create() override
					{
						Shooter::on_create();
						set_shot_interval(4);
					}
					void on_shoot() override
					{
						Shooter::on_shoot();
						ShotData d;
						d.set_cir_style(8, (1.0 - shot_t() / 240.0) * 30.0 + 20.0);
						d.v = shot_t() / 240.0 * 7.5 + 5.0;
						d.a = 0.05;

						double rot = this->rot();
						shoot<B>(d);
						set_rot(-rot);
						shoot<B>(d);
						set_rot(rot);

						double k = 0.0;
						if (shot_t() < 235)
							k = shot_t() / 235.0 * 1.0;
						else
							k = (shot_t() - 235) / 40.0 * 14.0 + 1.0;
						GameSystem::instance()->set_shake_effect(k);

						if (shot_t() == 240)
						{
							d.set_cir_style(128, 20.0);
							d.v = d.v.get_avg() * 0.8;
							shoot<B>(d);
						}
					}
					void on_update() override
					{
						Shooter::on_update();
						set_omega(omega() + 0.00015);
					}
					void on_start() override
					{
						Shooter::on_start();
						set_rot(pi * 0.125);
						set_omega(0.005);
					}
				};

				void on_create() override
				{
					class Bg final : public Background
					{
					public:
						Bg()
						{
							set_level(DrawLevel::background_2);
						}
						void on_draw(int t) override
						{
							DrawSystem::instance()->draw_func([this](sf::RenderTarget* target) {
								sf::RectangleShape rect;
								rect.setPosition({ -100, -100 });
								rect.setSize({ GameSystem::game_w + 200, GameSystem::game_h + 200 });
								rect.setFillColor(Color(0.0, 0.0, 0.0, get_alpha() * 0.5));
								target->draw(rect);
							}, get_level());
						}
					};
					set_background<Bg>();

					title_sprite().set_tex_and_coords(tex::bomb_title, coords::bomb_title);
					title_sprite().set_scale(0.6, 0.6);

					Bomb::on_create();
					set_len(240);
					set_invincible_time(360);
					set_pick_item_time(240);
					set_title(L"梦想天生");
					set_subtitle(L"");

					append_shooter<S>();
				}
				double fv = 0.0;
				double sv = 0.0;
				void on_start() override
				{
					Bomb::on_start();
					if (auto p = get_player())
					{
						fv = p->get_speed();
						sv = p->get_slowmode_speed();
						p->set_speed(sv * 0.3);
						p->set_slowmode_speed(sv * 0.3);
						p->set_lock_attack(true);
					}
				}
				void on_end() override
				{
					Bomb::on_end();
					if (auto p = get_player())
					{
						p->set_speed(fv);
						p->set_slowmode_speed(sv);
						p->set_lock_attack(false);
					}
				}
			};

			void on_create() override
			{
				Player::on_create();
				set_pos(GameSystem::game_w * 0.5, GameSystem::game_h * 0.8);
				set_sprite(tex::player, coords::player);
				sprite().set_scale(1.2, 1.2);
				hit_point()->set_sprite(tex::player , coords::hit_point);
				set_still_frame_index(0);
				set_left_move_frame_index(1);
				set_right_move_frame_index(2);
				set_still_from_left_frame_index(3);
				set_still_from_right_frame_index(4);

				set_main_gun<MainGun>();
				set_drone_set_slow<DroneSetSlow>();
				set_drone_set_fast<DroneSetFast>();

				set_power(0);

				set_kill_bomb_time(10);

				set_fast_bomb<FB>();
				set_slow_bomb<FB>();

				{
					graze_particle().emplace_back();
					auto& p = graze_particle().back();
					p.lifeTime = 15, p.cnt = 2;
					p.scale = { 0.5, 0.8 }, p.rand_pos_r = 5.0;
					p.omega = { -0.1, 0.1 }; 
					p.rand_v_direction = true, p.v = 6.0;
					p.set_option(Particle::Options::fade_out);
					p.set_option(Particle::Options::slow_down);
					p.sprite.set_tex_and_coords(tex::bullet, coords::bullet_kill_particle);
					p.sprite.set_color(Color(1.0, 1.0, 1.0, 1.0));
					p.sprite.set_level(DrawLevel::player_bullet_1);
				}
			}

			void on_update() override
			{
				Player::on_update();
				if (auto f = drone_set_fast())
					f->set_mode(get_power() / 100);
				if (auto s = drone_set_slow())
					s->set_mode(get_power() / 100);
			}

			void on_graze(EnemyBullet* b) override
			{
				add_point_score(10);
				se::graze->play();
			}

			void on_kill() override
			{
				Player::on_kill();
				set_power(get_power() - 50);

				double ox = GameSystem::game_w * 0.5;
				double oy = std::min(GameSystem::game_h * 0.6, y());

				double a = pi / 18.0;
				double theta = -pi * 0.5 - a * 3;
				for (int i = 0; i < 7; i++)
				{
					auto item = !get_life_count() && i == 3
						? (Item*)ObjectSystem::instance()->create_object<FullItem>()
						: (Item*)ObjectSystem::instance()->create_object<PowerItem>();
					item->set_start_info(0.0, 45, true, true);
					item->set_drop_info(2.0, 0.0, 30);
					item->set_pos(this);
					double tx = ox + 200 * cos(theta);
					double ty = oy + 100 * sin(theta);
					item->set_v((tx - x()) / 60.0, (ty - y()) / 60.0);
					item->add_task([item]() {
						item->set_v(0, 0);
						item->set_ay(0.05);
					}, 60);

					theta += a;
				}
			}
		};

		class B final : public EnemyBullet
		{
			void on_create() override
			{
				EnemyBullet::on_create();

				auto gs = GameSystem::instance();

				set_sprite(tex::bullet, coords::bullet_star_small_yellow);
				set_drag(10);
				set_drag_interval(1);
				sprite().set_blend(BlendMode::over);
				set_drag_alpha(0.3);
				set_drag_scale(0.8);

				set_anchored(false);
				set_omega(0.05);

				set_boundary(20, 20, 20, 20);
				set_collider_r(2.47);

				{
					kill_particle().emplace_back();
					auto& p = kill_particle().back();
					p.lifeTime = 15, p.cnt = 5;
					p.scale = 0.8, p.rand_pos_r = 5.0;
					p.rand_v_direction = true, p.v = 2.0;
					p.set_option(Particle::Options::fade_out);
					p.set_option(Particle::Options::slow_down);
					p.sprite.set_tex_and_coords(tex::bullet, coords::bullet_kill_particle);
					p.sprite.set_color(Color(1.0, 1.0, 0.4, 1.0));
					p.sprite.set_level(DrawLevel::enemy);
				}
			}
			void on_drop_item() override
			{
				EnemyBullet::on_drop_item();
				GameSystem::instance()->drop_item<PointSmallItem>(this, 0, 1);
			}
		};

		class S final : public Shooter
		{
			void on_create() override
			{
				Shooter::on_create();

				set_pos(GameSystem::game_w * 0.5, GameSystem::game_h * 0.3);
				set_shot_interval(120);
				set_keep_on_player(true);
			}
			void on_shoot() override
			{
				Shooter::on_shoot();

				ShotData data;
				data.set_cir_style(20, 0.0);
				data.v = 2.0;
				shoot<B>(data);

				data = ShotData();
				data.way = 5;
				data.rot_interval = 0.0;
				data.rot_offset = { -0.3, 0.3 };
				data.v = { 3.0, 6.0 };
				shoot<B>(data);
			}
		};

		class E final : public ESmall<1>
		{
			class S final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shot_interval(15);
					set_rot(pi * 0.5);
				}
				void on_shoot() override
				{
					Shooter::on_shoot();
					ShotData d;	d.set_fan_style(3, pi * 0.5, 0.0);
					for (int i = 0; i < 3; i++)
						d.v = 3.0 + i * 2.0, shoot<B>(d);
				}
			};
			void on_create() override
			{
				ESmall::on_create();
				set_health(30);
				append_shooter<S>();
				start_all_shooter();
			}
			void on_drop_item() override
			{
				ESmall::on_drop_item();
				GameSystem::instance()->drop_item<PowerItem>(this, 0.0, { 0, 1 });
				GameSystem::instance()->drop_item<PointItem>(this, { 5.0, 10.0 }, { 1, 2 });
			}
		};

		class L final : public Laser
		{
			void on_create() override
			{
				Laser::on_create();

				set_sprite(tex::bullet, coords::laser_blue);
				sprite().set_w_scale(-1.0);
				sprite().set_blend(BlendMode::plus);

				light_sprite().set_tex_and_coords(tex::bullet, coords::light_blue);
				light_sprite().set_blend(BlendMode::plus);
				light_sprite().set_scale(0.8, 0.8);
				set_head_light(true);

				set_full_len(100.0);
				set_len(100.0);
				set_w(4.0);
				turn_on(0);

				set_boundary(20, 10000, 10000, 20);
				set_can_be_killed_by_player(true);

				set_collider_w(1.5);
				set_collider_interval(2.0);

				{
					kill_particle().emplace_back();
					auto& p = kill_particle().back();
					p.lifeTime = 15, p.cnt = 5;
					p.scale = 0.8, p.rand_pos_r = 5.0;
					p.rand_v_direction = true, p.v = 2.0;
					p.set_option(Particle::Options::fade_out);
					p.set_option(Particle::Options::slow_down);
					p.sprite.set_tex_and_coords(tex::bullet, coords::bullet_kill_particle);
					p.sprite.set_color(Color(0.4, 0.8, 1.0, 1.0));
					p.sprite.set_level(DrawLevel::enemy);
				}
			}
			void on_drop_item(double x, double y) override
			{
				Laser::on_drop_item(x, y);
				GameSystem::instance()->drop_item<PointSmallItem>(x, y, 0, 1);
			}
		};

		class Boss1 final : public Boss
		{
		public:
			class NA1 final : public BossAttack
			{
				class S final : public Shooter
				{
					void on_create() override
					{
						Shooter::on_create();
						set_shot_interval(15);
					}
					void on_shoot() override
					{
						auto gs = GameSystem::instance();
						set_pos(gs->rand_real(-50.0, 50.0), gs->rand_real(-10.0, 10.0));
						set_rot(gs->rand_real(0, pi * 2.0));
						Shooter::on_shoot();
						ShotData d;	d.v = 4.0;
						d.set_cir_style(36);
						this->shoot<B>(d);
					}
				};

				void on_create() override
				{
					BossAttack::on_create();
					set_max_health(5000);
					set_time_limit(1800);
					set_prepare_time(60);
					append_shooter<S>();
				}

				void on_drop_item() override
				{
					GameSystem::instance()->drop_item<PowerItem>(this, { 50, 100 }, 50);
					GameSystem::instance()->drop_item<PowerBigItem>(this, 0, 1);
				}
			};

			class SC1 final : public SpellCard
			{
				class Bg final : public Background
				{
				public:
					Sprite s;
					Bg()
					{
						set_level(DrawLevel::background_2);
						s.set_tex_and_coords(tex::bg_1, coords::bg_1);
						s.set_color(Color(0.6, 0.6, 0.6));
						s.set_repeated(true);
						s.set_scale(10.0, 10.0);
						s.set_tex_scale(10.00, 10.0);
					}
					void on_draw(int t) override
					{
						double y1 = std::fmod(t, s.current_coord().height);
						double y2 = std::fmod(-t * 0.5, s.current_coord().height);
						s.set_alpha(1.0);
						draw_sprite(s, 0.0, y1);
						s.set_alpha(0.5);
						draw_sprite(s, 100.0, y2);
					}
				};

				class S final : public Shooter
				{
					bool flag = false;
					void on_create() override
					{
						Shooter::on_create();
						set_shot_interval(1);
						set_keep_on_player(true);
					}
					void on_shoot() override
					{
						Shooter::on_shoot();
						int t = (shot_t() - 30 + 120) % 120;
						if (t == 0) flag ^= 1;
						if (t / 2 < 5 && (t & 1))
						{
							ShotData d;
							d.rot_offset = pi * 0.15 * ((flag ? t / 2 : 4 - t / 2) - 2);
							for (int i = 0; i < 5; i++)
								d.v = i * 0.6 + 3.0, this->shoot<B>(d);
						}
					}
				};

				void on_create() override
				{
					SpellCard::on_create();
					set_background<Bg>();
					set_subtitle(L"大空");
					set_title(L"星河瀑布");
					set_max_health(10000);
					set_max_score(1000000);
					set_time_limit(3600);
					append_shooter<S>();
				}

				void on_casting() override
				{
					SpellCard::on_casting();

					int t = get_from_start_time();
					auto gs = GameSystem::instance();

					class B1 final : public EnemyBullet
					{
						void on_create() override
						{
							EnemyBullet::on_create();

							auto gs = GameSystem::instance();

							set_sprite(tex::bullet, coords::bullet_star_small_blue);
							set_drag(20);
							set_drag_interval(1);
							sprite().set_blend(BlendMode::plus);
							set_drag_alpha(0.3);
							set_drag_scale(0.8);

							set_anchored(false);
							set_omega(0.05);
							sprite().set_alpha(0.0), sprite().set_scale(0.0, 0.0);

							set_boundary(20, 20, 20, 20);
							set_collider_r(2.47);
							set_collide(false);
							set_can_be_killed_by_player(false);

							{
								kill_particle().emplace_back();
								auto& p = kill_particle().back();
								p.lifeTime = 15, p.cnt = 5;
								p.scale = 0.8, p.rand_pos_r = 5.0;
								p.rand_v_direction = true, p.v = 2.0;
								p.set_option(Particle::Options::fade_out);
								p.set_option(Particle::Options::slow_down);
								p.sprite.set_tex_and_coords(tex::bullet, coords::bullet_kill_particle);
								p.sprite.set_color(Color(0.4, 0.4, 1.0, 1.0));
								p.sprite.set_level(DrawLevel::enemy);
							}
						}
						void on_update() override
						{
							EnemyBullet::on_update();

							if (t() <= 120)
							{
								double k = t() / 120.0;
								sprite().set_alpha(k), sprite().set_scale(k, k);
							}
							else
								set_collide(true), set_can_be_killed_by_player(true);
						}
						void on_drop_item() override
						{
							EnemyBullet::on_drop_item();
							GameSystem::instance()->drop_item<PointSmallItem>(this, 0, 1);
						}
					};

					if (t & 1)
					{
						for (int i = 0; i < 1; i++)
						{
							auto b = ObjectSystem::instance()->create_object<B1>();
							b->set_pos(gs->rand_real(0.0, gs->game_w), gs->game_h * gs->rand_real(0.6, 0.7));
							b->set_v(gs->rand_real(-0.5, 0.5), gs->rand_real(-3.5, -3.0));
							b->set_ay(0.02);
						}
					}

					if ((t - 60) % 240 == 0)
					{
						get_boss()->move_randomly(60, 0.8, 
							{ 10.0, 50.0 }, { 0.0, 30.0 },
							{ 120, GameSystem::game_w - 120 }, { GameSystem::game_h * 0.2, GameSystem::game_h * 0.35 },
							move::factor::ease_in_out);
					}
				}

				void on_end() override
				{
					SpellCard::on_end();
					get_boss()->move_to_center(60);
				}

				void on_drop_item() override
				{
					GameSystem::instance()->drop_item<LifeItem>(this, 0, 1);
					GameSystem::instance()->drop_item<PointItem>(this, { 50.0, 100.0 }, 100);
				}
			};

			void on_create() override
			{
				Boss::on_create();

				set_still_frame_index(0);
				set_left_move_frame_index(1);
				set_right_move_frame_index(2);
				set_still_from_left_frame_index(3);
				set_still_from_right_frame_index(4);
				set_casting_frame_index(5);

				set_sprite(tex::boss_1, coords::boss_1);
				sprite().set_scale(0.8, 0.8);

				magic_circle().set_tex_and_coords(tex::magic_circle, coords::magic_circle);
				magic_circle().set_scale(2.0, 2.0);

				set_collider_r(25.0);

				if (auto p = GameSystem::instance()->get_player())
					set_pos(p->x(), p->y() - 200);

				kill_ani_particle().emplace_back();
				{
					auto& p = this->kill_ani_particle().back();
					p.lifeTime = { 30, 45 }, p.cnt = 2;
					p.scale = { 2.0, 3.0 }, p.rand_pos_r = 5.0;
					p.rand_v_direction = true, p.v = { 4.0, 6.0 };
					p.omega = { -0.3, 0.3 }, p.rot = { 0.0, pi * 2.0 };
					p.set_option(Particle::Options::fade_out);
					p.set_option(Particle::Options::shrink);
					p.set_option(Particle::Options::slow_down);
					p.sprite.set_tex_and_coords(tex::bullet, coords::bullet_kill_particle);
					p.sprite.set_color(Color(1.0, 1.0, 1.0, 1.0));
					p.sprite.set_level(DrawLevel::enemy);
				}

				kill_particle().emplace_back();
				{
					auto& p = this->kill_particle().back();
					p.lifeTime = { 45, 60 }, p.cnt = 30;
					p.scale = { 3.0, 5.0 }, p.rand_pos_r = 5.0;
					p.rand_v_direction = true, p.v = { 5.0, 10.0 };
					p.omega = { -0.3, 0.3 }, p.rot = { 0.0, pi * 2.0 };
					p.set_option(Particle::Options::fade_out);
					p.set_option(Particle::Options::shrink);
					p.set_option(Particle::Options::slow_down);
					p.sprite.set_tex_and_coords(tex::bullet, coords::bullet_kill_particle);
					p.sprite.set_color(Color(1.0, 1.0, 1.0, 1.0));
					p.sprite.set_level(DrawLevel::enemy);
				}

				charge_particle().emplace_back();
				{
					auto& p = this->charge_particle().back();
					p.lifeTime = { 15, 60 }, p.cnt = 20;
					p.scale = { 6.0, 10.0 }, p.rand_pos_r = { 200.0, 400.0 };
					p.omega = { -0.3, 0.3 }, p.rot = { 0.0, pi * 2.0 };
					p.set_option(Particle::Options::appear);
					p.set_option(Particle::Options::shrink);
					p.set_option(Particle::Options::gather);
					p.sprite.set_tex_and_coords(tex::bullet, coords::bullet_kill_particle);
					p.sprite.set_color(Color(1.0, 1.0, 1.0, 0.5));
					p.sprite.set_level(DrawLevel::enemy);
				}

				set_kill_ani_time(60);
				set_attack_interval(120);
			}	
		};

		// ----------------------------------------- stage ------------------------------------- //

		void on_create() override
		{
			Stage::on_create();

			load_res();

			GameSystem::instance()->set_game_panel<Panel>();

			GameSystem::instance()->set_player<P>();

			class Bg final : public Background
			{
			public:
				Bg()
				{
					set_level(DrawLevel::background_1);
				}
				void on_draw(int t) override
				{
					DrawSystem::instance()->draw_gl_func([=]() {
						GLDrawer::Camera::set_frustum(-0.5, 0.5, -0.5, 0.5, 0.5, 100.0);
						GLDrawer::Camera::rotate(sin(t * 0.0001) * 10.0, { 0.0, 0.0, 1.0 });
						GLDrawer::Camera::rotate(-40.0, { 1.0, 0.0, 0.0 });
						GLDrawer::Camera::translate(0.0, 2.0, -t * 0.02);
						GLDrawer::bind_texture(tex::bg_2, true, true);
						GLDrawer::Quad3d q;

						GLDrawer::Model::scale(10.0, 0.0, 10.0);
						GLDrawer::Model::rotate(30.0, { 0.0, 1.0, 0.0 });
						q.set_standard_coords();
						q.scale_coords(1000.0, 1000.0);
						q.set_all_color({ 0.5, 0.5, 0.5 });
						q.set_all_alpha(this->get_alpha());
						q.set_vert(0, { -1000.0, 0.0, -1000.0 });
						q.set_vert(1, { 1000.0, 0.0, -1000.0 });
						q.set_vert(2, { 1000.0, 0.0, 1000.0 });
						q.set_vert(3, { -1000.0, 0.0, 1000.0 });
						GLDrawer::draw_quad(q);
					});
				}
			};
			set_background<Bg>();

		}


		Boss* boss1 = nullptr;

		void on_update() override
		{
			Stage::on_update();

			int t = this->t();
			auto gs = GameSystem::instance();

			goto start;

		start:

			if (t == 0)
			{
				gs->get_player()->set_invincible(true, 60);
				gs->get_player()->show_item_pick_line();
				gs->get_player()->set_power(150);
				// bgm::bgm_1->play();
			}

			t -= 60;

		halfway:
			if (t >= 0 && t <= 120)
			{
				if (t % 10 == 0)
				{
					auto e = ObjectSystem::instance()->create_object<E>();
					e->set_pos(GameSystem::game_w * -0.1, GameSystem::game_h * 0.3);
					e->set_vx(2.0);
					e->set_ax(0.05);
				}
			} t -= 300;

			if (t >= 0 && t <= 120)
			{
				if (t % 10 == 0)
				{
					auto e = ObjectSystem::instance()->create_object<E>();
					e->set_pos(GameSystem::game_w * 1.1, GameSystem::game_h * 0.3);
					e->set_vx(-2.0);
					e->set_ax(-0.05);
				}
			} t -= 300;

		boss:
			if (t == 0)
			{
				gs->clear_all_enemy_bullets(false);
				boss1 = ObjectSystem::instance()->create_object<Boss1>();
			} t -= 0;

			if (t >= 0 && t <= 90)
			{
				move::straight(boss1, t, 0, 90,
					{ GameSystem::game_w * -0.1, GameSystem::game_h * -0.1 }, 
					{ GameSystem::game_w * 0.5, GameSystem::game_h * 0.3 }, 
					move::factor::ease_out<50>);
			} t -= 90;

			if (t == 60)
			{
				boss1->append_attack<Boss1::NA1>();
				boss1->append_attack<Boss1::SC1>();
				boss1->start_attacks();
				boss1->set_magic_circle(true);
			} t -= 60;
		}

	};

}