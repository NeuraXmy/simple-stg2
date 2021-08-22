#include "PPlayer.h"
#include "PItem.h"
#include "Shooter.h"
#include "PlayerBullet.h"
#include "PResource.h"
#include "GameSystem.h"
#include "Enemy.h"
#include "Boss.h"
#include "DeathWave.h"


void Reimu::on_create()
{
	Player::on_create();
	set_pos(GameSystem::game_w * 0.5, GameSystem::game_h * 0.8);
	set_sprite(tex.player, coords.player);
	sprite().set_scale(1.2, 1.2);
	hit_point()->set_sprite(tex.player, coords.hit_point);
	set_still_frame_index(0);
	set_left_move_frame_index(1);
	set_right_move_frame_index(2);
	set_still_from_left_frame_index(3);
	set_still_from_right_frame_index(4);

	set_graze_circle_r(25.0);

	get_death_wave()->set_damage(3);

	set_death_se(se.player_death);
	set_extend_life_se(se.player_extend_life);
	set_extend_bomb_se(se.player_extend_bomb);
	set_graze_se(se.graze);
	set_power_up_se(se.player_power_up);
	set_power_up_interval(100);
	set_full_power_se(se.player_full_power);

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
				set_sprite(tex.player, coords.player_bullet_1);
				set_create_se(se.player_shoot);
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
			d.v = 15.0, d.rot_offset = -utils::pi * 0.5;
			d.y_offset = -5.0;
			d.x_offset = -10.0, shoot<B>(d);
			d.x_offset = 10.0, shoot<B>(d);
		}
	};
	set_main_gun<MainGun>();

	// drone sets
	{
		struct Pos { double x, y; };
		static auto get_p = [](double a) -> Pos { return { 40.0 * cos(a), 40.0 * sin(a) }; };
		static const double pi = std::acos(-1.0);
		static const Pos dp[2][4][4] = {
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
		static auto lerp = [](const Pos& a, const Pos& b, double k) -> Pos {
			return { a.x * (1.0 - k) + b.x * k, a.y * (1.0 - k) + b.y * k };
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
						set_sprite(tex.player, coords.player_bullet_3);
						set_boundary(40, 40, 40, 40);
						set_collider_r(6.0);
						set_create_se(se.player_shoot);
					}
				};

				void on_create() override
				{
					Shooter::on_create();
					set_sprite(tex.player, coords.player_drone_2);
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
		set_drone_set_slow<DroneSetSlow>();

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
						set_sprite(tex.player, coords.player_bullet_2);
						set_boundary(40, 40, 40, 40);
						set_collider_r(6.0);
						sprite().set_blend(BlendMode::plus);
					}
					void on_update() override
					{
						PlayerBullet::on_update();

						double k = 0.0;
						if (t() <= 10)
							k = t() / 10.0 * 0.4;
						else if (t() <= 130)
							k = (130 - t()) / 120.0 * 0.4;
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
					set_sprite(tex.player, coords.player_drone_1);
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
		set_drone_set_fast<DroneSetFast>();
	}

	set_power(0);

	set_kill_bomb_time(10);

	// bombs
	{
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
						set_create_se(se.enemy_shoot_1);
						set_penetrate(true);
						sprite().set_alpha(0.5);
						sprite().set_scale(1.8, 1.8);
						set_drag(10), set_drag_interval(1), set_drag_alpha(0.8), set_drag_scale(0.8);
						set_sprite(tex.player, coords.player_bullet_2);
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
					set_rot(0.0);
					set_omega(0.005);
				}
			};

			void on_create() override
			{
				Bomb::on_create();

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

				set_start_se(se.cast);

				title_sprite().set_tex_and_coords(tex.bomb_title, coords.bomb_title);
				title_sprite().set_scale(0.6, 0.6);

				set_len(240);
				set_invincible_time(360);
				set_pick_item_time(240);
				set_title(L"√ŒœÎÃÏ…˙");
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
		set_fast_bomb<FB>();
		set_slow_bomb<FB>();
	}

	{
		graze_particle().emplace_back();
		auto& p = graze_particle().back();
		p.lifeTime = 15, p.cnt = 2;
		p.scale = { 0.5, 0.8 }, p.rand_pos_r = 5.0;
		p.omega = { -0.1, 0.1 };
		p.rand_v_direction = true, p.v = 6.0;
		p.set_option(Particle::Options::fade_out);
		p.set_option(Particle::Options::slow_down);
		p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
		p.sprite.set_color(Color(1.0, 1.0, 1.0, 1.0));
		p.sprite.set_level(DrawLevel::player_bullet_1);
	}

	{
		death_particle().emplace_back();
		auto& p = death_particle().back();
		p.lifeTime = 60, p.cnt = 20;
		p.scale = { 0.5, 1.5 };
		p.omega = { -0.1, 0.1 };
		p.rand_v_direction = true, p.v = { 5.0, 8.0 };
		p.set_option(Particle::Options::fade_out);
		p.set_option(Particle::Options::slow_down);
		p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
		p.sprite.set_color(Color(1.0, 1.0, 1.0, 1.0));
		p.sprite.set_level(DrawLevel::player_bullet_1);
	}
}


void Reimu::on_update() 
{
	Player::on_update();
	if (auto f = drone_set_fast())
		f->set_mode(get_power() / 100);
	if (auto s = drone_set_slow())
		s->set_mode(get_power() / 100);
}


void Reimu::on_graze(EnemyBullet* b)
{
	add_point_score(10);
}

void Reimu::on_graze(Laser* l)
{
	add_point_score(10);
}


void Reimu::on_kill()
{
	Player::on_kill();
	set_power(get_power() - 50);

	double ox = GameSystem::game_w * 0.5;
	double oy = std::min(GameSystem::game_h * 0.6, y());

	double a = utils::pi / 18.0;
	double theta = -utils::pi * 0.5 - a * 3;
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