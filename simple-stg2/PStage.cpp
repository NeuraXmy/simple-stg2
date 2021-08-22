#include "PStage.h"
#include "ObjectSystem.h"
#include "ParticleSystem.h"
#include "GameSystem.h"
#include "PBullet.h"
#include "PItem.h"
#include "PPlayer.h"
#include "PPanel.h"
#include "PEnemy.h"
#include "PBossAttack.h"
#include "PBoss.h"
#include "Laser.h"
#include "DeathWave.h"
#include "ClearBulletWave.h"

using utils::pi;

namespace
{
	std::pair<double, double> get_star_pos(double t, double w)
	{
		t = fmod(t, 1.0);
		double ang1 = pi * 2.0 * 0.2;
		double k = fmod(t + 1e-6, 0.2) / 0.2;
		double rot = floor(t / 0.2 + 1e-6) * ang1 * 2.0;
		double x = w * (k - 0.5);
		double y = -w * 0.5 / tan(ang1);
		return { x * cos(rot) - y * sin(rot), y * cos(rot) + x * sin(rot) };
	}
	int lsgn(bool left) {
		return left ? 1 : -1;
	}
	double lx(double x, bool left) {
		return left ? x : GameSystem::game_w - x;
	}

	void warning_particle_update(Particle::Particle* p)
	{
		if (p->t() <= 15)
		{
			double k = p->t() / 15.0;
			p->scale = 1.0 + 3.0 * (1.0 - k);
			p->sprite.set_alpha((p->t() >> 1 & 1) ? k * 1.0 : 0.0);
		}
		else
		{
			p->sprite.set_alpha((p->t() >> 1 & 1) ? 1.0 : 0.0);
		}
	}
	void create_warning(double x, double y, int life_time)
	{
		Particle::Data data;
		data.lifeTime = life_time;
		data.sprite.set_tex_and_coords(tex.warning, coords.warning);
		data.sprite.set_level(DrawLevel::player_bullet_2);
		data.update_func = &warning_particle_update;
		data.cnt = 1;
		ParticleSystem::instance()->create_particle(data, x, y);
	}

	void clear_bullets(double x, double y, double v = 8.0, bool force = true, bool drop = false, double r = 500.0, int damage = 0) {
		auto w = ObjectSystem::instance()->create_object<ClearBulletWave>();
		w->set_bullet_drop(drop);
		w->set_damage(damage);
		w->set_pos(x, y);
		w->set_initial_expand_v(v);
		w->set_max_radius(r);
		w->set_force_kill(force);
		w->start_expand();
	}
}

void PStage::on_create()
{
	Stage::on_create();

	tex.load();
	coords.load();
	se.load();
	bgm.load();

	GameSystem::instance()->set_game_panel<PPanel>();
	GameSystem::instance()->set_player<Reimu>();

	class Bg final : public Background
	{
		double now_x = 0.0;
		double ddx = 0.0;
		double z = 0.0;
		double speed = 0.0;
		double rot = 0.0;
		double dim = 0.0;
		double star_rot = 0.0;

	public:
		Bg()
		{
			set_level(DrawLevel::background_1);
		}
		void on_update(int t) override
		{
			{
				double player_x = 0.0;
				if (auto p = GameSystem::instance()->get_player())
					player_x = (p->x() - GameSystem::center_w) * 0.01;
				double dx = utils::lerp(now_x, player_x, 0.015) - now_x;
				ddx = utils::lerp(ddx, dx, 0.05);
				now_x += ddx;
			}

			star_rot += 0.02;

			if (t <= 1200)
				dim = 1.0;
			else if (t < 10300)
				dim = 0.0;
			else if (t >= 10300 && t <= 10420)
				dim = utils::lerp(0.0, 1.0, (t - 10300) / 120.0);
			else
				dim = 1.0;
			dim = 1.0 - ((1.0 - dim) * 0.6 + sin(t / 60.0) * 0.4);

			if (t <= 1200)
			{
				rot = -60.0;
				speed = -0.02;
			}
			if (t >= 1200 && t <= 1440)
			{
				double k = (t - 1200) / 240.0;
				rot = utils::lerp(-60.0, -30.0, move::factor::ease_in_out(k));
				speed = utils::lerp(-0.02, -0.005, k);
			}
			if (t >= 1440 && t <= 1560)
			{
				double k = (t - 1440) / 120.0;
				rot = -30.0;
				speed = utils::lerp(-0.005, -0.03, k);
			}
			if (t >= 1560 && t <= 2230)
			{
				rot = -30.0;
				speed = -0.03;
			}
			if (t >= 2230 && t <= 2290)
			{
				double k = (t - 2230) / 60.0;
				rot = -30.0;
				speed = utils::lerp(-0.03, -0.08, k);
			}
			if (t >= 2290 && t <= 2980)
			{
				rot = -30.0;
				speed = -0.08;
			}
			if (t >= 2980 && t <= 3100)
			{
				double k = (t - 2980) / 120.0;
				rot = utils::lerp(-30.0, 0.0, move::factor::ease_in_out(k));
				speed = utils::lerp(-0.08, -0.01, k);
			}
			if (t >= 3100 && t <= 3700)
			{
				rot = 0.0;
				speed = -0.01;
			}
			if (t >= 3700 && t <= 3820)
			{
				double k = (t - 3700) / 120.0;
				rot = utils::lerp(0.0, 30.0, move::factor::ease_in_out(k));
				speed = utils::lerp(-0.01, -0.03, k);
			}
			if (t >= 3820 && t <= 8020)
			{
				rot = 30.0;
				speed = -0.03;
			}
			if (t >= 8020 && t <= 8140)
			{
				double k = (t - 8020) / 120.0;
				rot = utils::lerp(30.0, 70.0, move::factor::ease_in_out(k));
				speed = -0.03;
			}
			if (t >= 8140)
			{
				rot = 90.0;
				speed = -0.03;
			}
			

			z += speed;
		}
		void on_draw(int t) override
		{
			DrawSystem::instance()->draw_gl_func([=]() {
				

				GLDrawer::Camera::set_frustum(-0.5, 0.5, -0.5, 0.5, 0.5, 100.0);
				GLDrawer::Camera::rotate(sin(t * 0.0001) * 5.0, { 0.0, 0.0, 1.0 });

				if(t >= 800)
				{
					GLDrawer::Quad3d q;
					GLDrawer::bind_texture(tex.bg_3, true, true);
					GLDrawer::Camera::rotate(star_rot, { 0.0, 0.0, 1.0 });
					q.set_standard_coords();
					q.scale_coords(1.0, 1.0);
					q.set_all_color({ 1.0 - dim, 1.0 - dim, 1.0 - dim });
					q.set_all_alpha(this->get_alpha());
					q.set_vert(0, { -180.0, -180.0, -99.0 });
					q.set_vert(1, { 180.0, -180.0, -99.0 });
					q.set_vert(2, { 180.0, 180.0, -99.0 });
					q.set_vert(3, { -180.0, 180.0, -99.0 });
					GLDrawer::draw_quad(q);
					GLDrawer::Camera::rotate(-star_rot, { 0.0, 0.0, 1.0 });
				}

				GLDrawer::Camera::rotate(rot, { 1.0, 0.0, 0.0 });

				{
					GLDrawer::Camera::translate(now_x, 2.0, 0.0);

					GLDrawer::Camera::translate(0.0, 0.0, z);
					GLDrawer::Model::scale(10.0, 0.0, 10.0);
					GLDrawer::Model::rotate(30.0, { 0.0, 1.0, 0.0 });
					GLDrawer::Quad3d q;
					GLDrawer::bind_texture(tex.bg_2, true, true);
					q.set_standard_coords();
					q.scale_coords(1000.0, 1000.0);
					q.set_all_color({ 0.4, 0.4, 0.4 });
					q.set_all_alpha(this->get_alpha());
					q.set_vert(0, { -1000.0, 0.0, -1000.0 });
					q.set_vert(1, { 1000.0, 0.0, -1000.0 });
					q.set_vert(2, { 1000.0, 0.0, 1000.0 });
					q.set_vert(3, { -1000.0, 0.0, 1000.0 });
					GLDrawer::draw_quad(q);
				}
			});
		}
	};
	set_background<Bg>();
}

void PStage::on_update()
{
	Stage::on_update();
	static auto gs = GameSystem::instance();
	static auto ds = DrawSystem::instance();
	static auto os = ObjectSystem::instance();
	static auto ps = ParticleSystem::instance();
	int t = this->t();
	static auto player = GameSystem::instance()->get_player();
	
start:
	{
		if (t == 0)
		{
			player->set_invincible(true, 120);
			player->set_power(100);
			player->show_item_pick_line();
			bgm.bgm_1->play();
		}			
		// goto part5;
		if (t >= 0 && t <= 120)
		{
			background()->set_alpha(t / 120.0);
			bgm.bgm_1->set_vol(t / 120.0);
		} 
		t -= 180;
	}

part1:   // if (t == 0) bgm.bgm_1->set_time(180); 
	{
		class E1 final : public ESPrefab
		{
			double sx, sy;
			class S final : public Shooter
			{
				class B final : public BSPrefab
				{
				public:
					B() : BSPrefab(BSType::star, BSColor::yellow) {}
					void on_create() {
						BSPrefab::on_create();
						sprite().set_blend(BlendMode::plus);
						set_drag(10);
						set_drag_shrink(true);
						set_drag_fade(true);
						set_drag_alpha(0.5);
						set_drag_scale(0.8);
					}
				};

				void on_create() override
				{
					Shooter::on_create();
					set_shot_interval(60);
					set_rot(utils::pi * 0.5);
					set_shoot_se(se.enemy_shoot_1);
				}
				void on_shoot() override
				{
					Shooter::on_shoot();
					ShotData d;	d.set_fan_style(3, utils::pi * 0.4, 0.0);
					for (int i = 0; i < 5; i++)
						d.v = 2.0 + i * 1.0, shoot<B>(d);
				}
			};
			void on_create() override
			{
				ESPrefab::on_create();
				set_max_health(60);
				append_shooter<S>();
				set_boundary(30, 30, 200, 0);
			}
			void on_update() override
			{
				ESPrefab::on_update();	int t = this->t();
				if (t == 0) sx = x(), sy = y();
				if (t == 45) start_all_shooter();
				if (t <= 90)
					move::straight(this, t, 0, 90, { sx, sy }, { sx, sy + 200 }, move::factor::ease_out);
				if (t == 120) {
					set_v(2.0, 0.1);
					if (sx > gs->center_w) set_vx(-vx());
				}
			}
		public:
			E1() : ESPrefab(ESColor::yellow, 3, 1) {}
		};

		if (t == 0)
		{
			os->create_object<E1>()->set_pos(30, -40);
			os->create_object<E1>()->set_pos(70, -40);
		}
		if (t == 60)
		{
			os->create_object<E1>()->set_pos(20, -70);
			os->create_object<E1>()->set_pos(60, -70);
			os->create_object<E1>()->set_pos(100, -70);
		}

		if (t == 360)
		{
			os->create_object<E1>()->set_pos(gs->game_w - 30, -40);
			os->create_object<E1>()->set_pos(gs->game_w - 70, -40);
		}
		if (t == 420)
		{
			os->create_object<E1>()->set_pos(gs->game_w - 20, -70);
			os->create_object<E1>()->set_pos(gs->game_w - 60, -70);
			os->create_object<E1>()->set_pos(gs->game_w - 100, -70);
		}
		if (t == 480)
		{
			os->create_object<E1>()->set_pos(gs->game_w - 10, -100);
			os->create_object<E1>()->set_pos(gs->game_w - 50, -100);
			os->create_object<E1>()->set_pos(gs->game_w - 90, -100);
			os->create_object<E1>()->set_pos(gs->game_w - 130, -100);
		}

		t -= 720;
	}	

part2:   // if (t == 0) bgm.bgm_1->set_time(900);									
	{
		class E1 final : public ESPrefab
		{
			double sx, sy;
			class S final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shot_interval(60);
					set_shoot_se(se.enemy_shoot_2);
				}
				void on_shoot() override
				{
					Shooter::on_shoot();
					auto sh = [this]() {
						ShotData d;
						d.v = 6.0;
						d.v_min = 4.0;
						d.a = -0.04;
						shoot<BSPrefab>(d, BSType::star, BSColor::sky_blue);
						for (int i = 1; i < 5; i++) {
							d.set_fan_style(2, i * 0.04, 0.0);
							d.v = 6.0 - i * 0.1;
							d.v_min = (6.0 - i * 0.1) / 3.0 * 2.0;
							shoot<BSPrefab>(d, BSType::star, BSColor::sky_blue);
						}
					};
					point_to(player);
					sh();
					set_rot(rot() + utils::pi * 2.0 / 5.0);
					sh();
					set_rot(rot() + utils::pi * 2.0 / 5.0);
					sh();
					set_rot(rot() + utils::pi * 2.0 / 5.0);
					sh();
					set_rot(rot() + utils::pi * 2.0 / 5.0);
					sh();
				}
			};
			void on_create() override
			{
				ESPrefab::on_create();
				set_max_health(60);
				append_shooter<S>();
				set_boundary(30, 30, 200, 0);
			}
			void on_update() override
			{
				ESPrefab::on_update();	int t = this->t();
				if (t == 0) sx = x(), sy = y();
				if (t == 45) start_all_shooter();
				if (t <= 90)
					move::straight(this, t, 0, 90, { sx, sy }, { sx, sy + 200 }, move::factor::ease_out);
				if (t == 120) {
					set_v(2.0, -0.5);
					if (sx > gs->center_w) set_vx(-vx());
				}
			}
		public:
			E1() : ESPrefab(ESColor::blue, 0, 3) {}
		};
		class E2 final : public EBPrefab
		{
			double sx, sy;
		public:
			E2() : EBPrefab(10, 5) {}
			class B final : public BSPrefab
			{
			public:
				B() : BSPrefab(BSType::star, BSColor::blue) {}
				void on_create() {
					BSPrefab::on_create();
					sprite().set_blend(BlendMode::plus);
					set_boundary(220, 220, 100, 100);
					set_can_be_killed_by_player(false);
					sprite().set_level(DrawLevel::enemy_bullet_3);
				}
				void explode() {
					double x = this->x() - bound_obj()->x();
					double y = this->y() - bound_obj()->y();
					set_rot(atan2(y, x) + pi + 0.5);
					double vx = x * 0.06;
					double vy = y * 0.06;
					double v = sqrt(vx * vx + vy * vy);
					set_vx(v), set_ax(0.001), set_min_v(v * 0.2);
					set_a_to_rot(), set_v_to_rot(), set_a(-ax(), -ay());
					bind(nullptr);
					set_drag(10);
					set_drag_shrink(true);
					set_drag_fade(true);
					set_drag_alpha(0.5);
					set_drag_scale(0.8);
					sprite().set_blend(BlendMode::over);
				}
			};
			class BC final : public PhysicObject
			{
			public:
				void on_create() override {
					PhysicObject::on_create();
					set_omega(0.01);
					set_rot(gs->rand_real(0, pi));
					for (int i = 0; i < 540; i++) if(i % 9 < 3)
					{
						auto b = os->create_object<B>();
						auto [x, y] = get_star_pos(i / 540.0, 100.0);
						b->set_pos(x, y);
						b->bind(this);
					}
				}
				void explode() {
					auto list = child_list();
					for (auto l : list)
						if (auto p = dynamic_cast<B*>(l))
							p->explode();
					destroy();
				}
			};
			BC* bc = nullptr;
			void on_create() override
			{
				EBPrefab::on_create();
				set_boundary(120, 120, 100, 100);
				set_max_health(500);
				bc = os->create_object<BC>();
				bc->bind(this);
				set_invincible(true, 60);
			}
			void on_update() override
			{
				EBPrefab::on_update(); int t = this->t();
				if (t == 0) sx = x(), sy = y();
				if (t <= 360)
				{
					double tx = sx + 420 * (sx < gs->center_w ? 1 : -1);
					double ty = sy - 150;
					move::bezier3(this, t, 0, 360,
						{ sx, sy }, { 0, -100 },
						{ tx, ty }, { -100 * (sx < gs->center_w ? 1.0 : -1.0), 0 }, move::factor::ease_out);
				}
				if (t == 360)
					kill(false);
				if (t <= 240 && t % 4 == 0)
				{
					auto b = os->create_object<BSPrefab>(BSType::star, BSColor::yellow, 1);
					b->sprite().set_blend(BlendMode::plus);
					b->set_a(gs->rand_real(-0.005, 0.005) + (sx < gs->center_w ? -0.02 : 0.02), 0.02);
					b->set_max_v(3.0);
					b->set_pos(this);
				}
			}
			void on_kill() override
			{
				if (bc) bc->explode(), bc = nullptr;
				EBPrefab::on_kill();
			}
		};

		if (t >= 0 && t <= 180 && t % 30 == 0)
		{
			os->create_object<E1>()->set_pos(gs->rand_real(50, gs->game_w - 50), gs->rand_real(-40, -100));
		}
		if (t == 90)
		{
			create_warning(20, gs->center_h - 10, 60);
			create_warning(gs->game_w - 20, gs->center_h- 10, 60);
		}
		if (t == 120)
		{
			os->create_object<E2>()->set_pos(gs->game_w + 100, gs->center_h + 50);
			os->create_object<E2>()->set_pos(-100, gs->center_h + 50);
		}
		if (t == 630)
		{
			clear_bullets(gs->center_w, 0, 20.0);
		} 
		t -= 600;
	}	

part3:   // if (t == 0) bgm.bgm_1->set_time(1500);		
	{
		class E1 final : public EBPrefab
		{
			double sx, sy;
			bool dir;
		public:
			E1(bool dir) : EBPrefab(5, 10), dir(dir) {}
			class L final : public Laser
			{
				void on_create() override
				{
					Laser::on_create();
					set_sprite(tex.bullet, coords.laser1[(int)BSColor::sky_blue]);
					sprite().set_blend(BlendMode::plus);
					set_full_len(80.0);
					set_len(0.0);
					sprite().set_w_scale(-1.0);
					set_w(5);
					sprite().set_alpha(0.8);
					set_collider_interval(2.0);
					set_collider_w(1.0);
					set_head_light(true);
					light_sprite().set_tex_and_coords(tex.bullet, coords.bullet_small[(int)BSType::star][(int)BSColor::sky_blue]);
					light_sprite().set_blend(BlendMode::plus);
					create_sprite().set_tex_and_coords(tex.bullet, coords.laser_light[(int)BBColor::sky_blue]);
					create_sprite().set_blend(BlendMode::plus);
					set_can_be_killed_by_player(true);
					turn_on(0);

					{
						kill_particle().emplace_back();
						auto& p = kill_particle().back();
						p.lifeTime = 15, p.cnt = 5;
						p.scale = 0.8, p.rand_pos_r = 5.0;
						p.rand_v_direction = true, p.v = 2.0;
						p.set_option(Particle::Options::fade_out);
						p.set_option(Particle::Options::slow_down);
						p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
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
			class S final : public Shooter
			{
			public:
				bool dir;
				S(bool dir) : dir(dir) {}
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.laser_1);
					set_shot_interval(5);
				}
				void on_shoot() override
				{
					Shooter::on_shoot();
					set_rot(rot() + pi * 2.0 / 8.0 * 0.22 * (dir ? -1.0 : 1.0));
					ShotData d;
					d.set_cir_style(8, 30.0 + 30.0 * shot_t() / 150.0);
					d.v = 6.0;
					shoot<L>(d);
				}
			};
			void on_create() override
			{
				EBPrefab::on_create(); 
				set_max_health(600);
				append_shooter<S>(dir);
				set_boundary(50, 50, 200, 0);
				set_invincible(true, 120);
			}
			void on_update() override
			{
				EBPrefab::on_update(); int t = this->t();
				if (t == 0) sx = x(), sy = y();
				if (t <= 90) {
					move::straight(this, t, 0, 90,
						{ sx, sy }, { sx, sy + 200 }, move::factor::ease_out);
				}
				if (t == 90) start_all_shooter();
				if (t == 240) stop_all_shooter();
				if (t == 300) set_vx(-1.0 * lsgn(sx < gs->center_w));
			}
			void on_drop_item() override
			{
				EBPrefab::on_drop_item();
				gs->drop_item<PowerBigItem>(this, 0.0, 1);
			}
		};
		class E2 final : public ESPrefab
		{
			double sx, sy;
			class S final : public Shooter
			{
				class B final : public BSPrefab
				{
				public:
					B() : BSPrefab(BSType::star, BSColor::yellow) {}
					void on_create() {
						BSPrefab::on_create();
						sprite().set_blend(BlendMode::plus);
						set_drag(15);
						set_drag_shrink(true);
						set_drag_fade(true);
						set_drag_alpha(0.5);
						set_drag_scale(0.8);
					}
					void on_update() {
						BSPrefab::on_update();
						if (t() == 15) {
							point_to(player);
							set_vx(4.5);
							set_v_to_rot();
							set_a(0.0, 0.0);
						}
					}
				};
				void on_create() override
				{
					Shooter::on_create();
					set_shot_interval(60);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = shot_t();
					if (t == 0)
					{
						se.enemy_shoot_2->play();
						set_rot(gs->rand_real(0.0, pi));
						ShotData d;
						d.set_cir_style(5, 2.0);
						d.v = 2.0, d.a = 2.0 / 15.0;
						shoot<B>(d);
					}
					else
					{
						se.enemy_shoot_1->play();
						point_to(player);
						ShotData d;
						d.set_cir_style(18, 2.0, true);
						double ro = d.rot_offset.get_avg();
						for (int i = 0; i < 6; i++)
						{
							d.v = 1.0 - i * 0.2;
							d.v_max = 2.0 + i * 1.0;
							d.rot_offset = ro + gs->rand_real(-0.05, 0.05);
							d.a = 0.2;
							shoot<BSPrefab>(d, BSType::round_ring, BSColor::yellow, 1);
						}
						stop();
					}	
					
				}
			};
			void on_create() override
			{
				ESPrefab::on_create();
				set_max_health(15);
				append_shooter<S>();
				set_boundary(200, 200, 200, 0);
				set_invincible(true, 60);
			}
			void on_update() override
			{
				ESPrefab::on_update();	int t = this->t();
				if (t == 0) sx = x(), sy = y();
				if (t <= 60)
					move::straight(this, t, 0, 60, { sx, sy }, { sx, sy + 200 }, move::factor::ease_out);
				if (t == 60) start_all_shooter();
				if (t == 120) set_ay(-0.02);
			}
		public:
			E2() : ESPrefab(ESColor::red, 5, 0) {}
		};

		if (t == 60)
		{
			os->create_object<E1>(0)->set_pos(150, -100);
		}
		if (t >= 150 && t <= 225 && t % 15 == 0)
		{
			int i = (t - 150) / 15;
			os->create_object<E2>()->set_pos(gs->game_w - 30 - i * 35, -100);
		}

		if (t == 420)
		{
			os->create_object<E1>(1)->set_pos(gs->game_w - 150, -100);
		}
		if (t >= 510 && t <= 585 && t % 15 == 0)
		{
			int i = (t - 510) / 15;
			os->create_object<E2>()->set_pos(30 + i * 35, -100);
		} t -= 790;
	}	

part4:   // if (t == 0) bgm.bgm_1->set_time(2290);	
	{	 

		static const int h_interval = 50; 
		static const int w_interval = 36; 

		class E1 final : public ESPrefab
		{
			bool left = false;
			double sx, sy;
			class S final : public Shooter
			{
				bool left;
			public:
				S(bool left) : left(left) {}
				void on_create() override
				{
					Shooter::on_create();
					set_shot_interval(60);
				}
				void on_shoot() override
				{
					Shooter::on_shoot();
					point_to(player);
					ShotData d;
					d.a = -0.04;
					auto c = BSColor::white;
					for (int i = 0; i < 8; i++)
					{
						d.v = 2.0 + i * 0.4;
						d.v_min = (2.0 + i * 0.4) * 0.5;
						shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
							b->sprite().set_level(DrawLevel::enemy_bullet_3);
						}, BSType::star, c);
					}
				}
			};
			class B final : public BBPrefab
			{
				double sx, sy, tx;
			public:
				B(bool left, double tx) : BBPrefab(BBType::star, left ? BBColor::yellow : BBColor::purple), tx(tx) {}
				void on_create() {
					BBPrefab::on_create();
					// sprite().set_blend(BlendMode::plus);
					set_drag(20);
					set_drag_shrink(true);
					set_drag_fade(true);
					set_drag_alpha(0.4);
					set_drag_scale(0.8);
					sprite().set_scale(1.2, 1.2);
					set_collider_r(get_collide_r() * 1.1);
					set_can_be_killed_by_player(false);
				}
				void on_update() {
					BBPrefab::on_update();	int t = this->t();
					if (t == 0)		sx = x(), sy = y();
					if (t <= 30)	move::straight(this, t, 0, 30, { sx, sy }, { tx, sy }, move::factor::ease_out);
					if (t >= 45) {
						t = (t - 45) % 90;
						if (t <= 30)
						{
							if (t == 0) sx = x(), sy = y(), se.enemy_shoot_1->play();
							move::straight(this, t, 0, 30, { sx, sy }, { sx, sy + h_interval * 2 }, move::factor::ease_out);
						}
					}
				}
			};
			void on_create() override
			{
				ESPrefab::on_create();
				set_max_health(60);
				set_boundary(40, 40, 0, 0);
				set_invincible(true, 45);
				append_shooter<S>(left);
			}
			void on_update() override
			{
				ESPrefab::on_update(); int t = this->t();
				if (t == 0)		sx = x(), sy = y();
				if (t <= 45)	move::straight(this, t, 0, 45, { sx, sy }, { sx + 50 * lsgn(left), sy }, move::factor::ease_out);
				if (t == 45)	set_v(1.0 * lsgn(left), 0.3);
				if (t == 45) {
					double tx = 21, sy = y();
					for (int i = 0; tx + i * w_interval <= gs->game_w - 21; i++)
						os->create_object<B>(left, lx(tx + i * w_interval, left))->set_pos(this);
					se.enemy_shoot_1->play();
				}
				if (t == 60)
					start_all_shooter();
			}
		public:
			E1(bool left) : ESPrefab(ESColor::purple, 1, 1), left(left) {}
		};

		if (t >= 0 && t / 45 < 10 && t / 45 != 4)
		{
			if (t % 90 == 0)
				os->create_object<E1>(0)->set_pos(lx(-30, 0), 17);
			if (t % 90 == 45)
				os->create_object<E1>(1)->set_pos(lx(-30, 1), 17 + h_interval);
		} t -= 810;

	}	

part5:   // if (t == 0) bgm.bgm_1->set_time(3100);
	{	 
		class E1 final : public EBPrefab
		{
			double sx, sy;
			bool left;
		public:
			E1(bool left) : EBPrefab(20, 5), left(left) {}
			class S final : public Shooter
			{
				class B final : public BSPrefab
				{
					void on_create() override
					{
						BSPrefab::on_create();
						set_pos_fixed(true);
						set_boundary(200, 200, 200, 10);
						sprite().set_blend(BlendMode::plus);
					}
					void on_update() override
					{
						BSPrefab::on_update();
						if (t() == 90) {
							set_pos_fixed(false);
							sprite().set_blend(BlendMode::over);
						}
					}
				public:
					B() : BSPrefab(BSType::star, BSColor::sky_blue, 1) {}
				};
			public:
				void on_create() override
				{
					Shooter::on_create();
					set_shot_interval(2);
					set_omega(0.06);
				}
				void on_shoot() override
				{
					Shooter::on_shoot();	int t = shot_t();
					ShotData d;
					d.set_cir_style(4, (1.0 - t / 200.0) * 50.0 + 0.0);
					d.v = 0.0, d.a = 0.02, d.v_max = 4.5;
					shoot<B>(d);
					if (t >= 200) stop();
				}
			};
			void on_create() override
			{
				EBPrefab::on_create();
				set_max_health(1000);
				set_boundary(60, 60, 50, 50);
				append_shooter<S>();
				start_all_shooter();
			}
			void on_update() override
			{
				EBPrefab::on_update(); int t = this->t();

				if (t / 200 >= 2)
				{
					kill_raw();
					return;
				}

				if (t == 230)
					set_health(get_health() * 0.5);

				if (t % 200 <= 150)
				{
					double k = 1.0 - (t % 200) / 150.0;
					set_v((k * 1.1 + 2.5) * lsgn(left), k * k * 2.0);
					if (t % 4 == 0)
					{
						auto b = os->create_object<BBPrefab>(BBType::star, BBColor::gray, 1);
						b->set_boundary(100, 100, 100, 0);
						b->sprite().set_level(DrawLevel::enemy_bullet_2);
						b->set_pos(this);
						b->set_rot((-(t % 200) / 100.0 + pi * 0.3) * lsgn(left) + pi * 0.5);
						b->set_ax(0.055), b->set_a_to_rot();
						b->set_max_v(10.0);

						b = os->create_object<BBPrefab>(BBType::star, BBColor::gray, 1);
						b->set_boundary(100, 100, 100, 0);
						b->sprite().set_level(DrawLevel::enemy_bullet_2);
						b->set_pos(this);
						b->set_rot(1.5 * pi - (-(t % 200) / 100.0 + pi * 0.3) * lsgn(left));
						b->set_ax(0.055), b->set_a_to_rot();
						b->set_max_v(10.0);
						
						se.enemy_shoot_2->play();
					}
					if ((t + 3) % 6 == 0)
					{
						auto b = os->create_object<BBPrefab>(BBType::star, BBColor::yellow, 1);
						b->set_boundary(100, 100, 100, 0);
						b->sprite().set_level(DrawLevel::enemy_bullet_2);
						b->set_pos(this);
						b->set_rot((-(t % 200) / 100.0 + pi * 0.3) * lsgn(left) + pi * 0.5);
						b->set_ax(0.035), b->set_a_to_rot();
						b->set_max_v(10.0);

						b = os->create_object<BBPrefab>(BBType::star, BBColor::yellow, 1);
						b->set_boundary(100, 100, 100, 0);
						b->sprite().set_level(DrawLevel::enemy_bullet_2);
						b->set_pos(this);
						b->set_rot(1.5 * pi - (-(t % 200) / 100.0 + pi * 0.3) * lsgn(left));
						b->set_ax(0.045), b->set_a_to_rot();
						b->set_max_v(10.0);
					}
					if (t == 150) left ^= 1;
				}
				else
				{
					set_v(0, 0);
				}
			}
		};

		if (t == 0)
		{
			os->create_object<E1>(1)->set_pos(lx(-40, 1), -20);
		}

		if (t == 360)
		{
			os->create_object<E1>(0)->set_pos(lx(-40, 0), -20);
		}
		t -= 720;
		if (t == 0)
			gs->clear_all_enemy_bullets(false);
	}

part6:	 // if (t == 0) bgm.bgm_1->set_time(3820);
	{
		struct Point { double x, y;  };
		struct Line { Point a, b;  };	
		struct Pattern { 
			std::vector<Point> p; 
			std::vector<Line> l;
		};
		static const int appear_time = 120;
		static const int link_time = 60;
		static const Pattern pat1 = {
			{
				{ -115,	-2 },
				{ -62, -25 },
				{ -27, -16 },
				{ 19, -7 },
				{ 37, 27 },
				{ 105, 15 },
				{ 103, -33 }
			},
			{
				{ { -62, -25 }, { -115, -2 } },
				{ { -62, -25 }, { -27, -16 } },
				{ { 19, -7 }, { -27, -16 } },
				{ { 19, -7 }, { 37, 27 } },
				{  { 37, 27 }, { 105, 15 } },
				{ { 103, -33 }, { 105, 15 } },
				{ { 103, -33 }, { 19, -7 } }
			}
		};
		static const Pattern pat2 = {
			{
				{ -124, 7 },
				{ -105, 31 },
				{ -64, 41 },
				{ 8, -69 },
				{ 108, -16 },
				{ 73, 52 }
			},
			{
				{ { -105, 31 }, { -124, 7 } },
				{ { -105, 31 }, { -64, 41 } },
				{ { -64, 41 }, { 8, -69 } },
				{ { 8, -69 }, { -124, 7 } },
				{ { 108, -16 }, { 8, -69 } },
				{ { 108, -16 }, { 73, 52 } },
			}
		};
		static const Pattern pat3 = {
			{
				{ -110, -33 },
				{ -93, 35 },
				{ -64, -55 },
				{ 10, -19 },
				{ 18, -6 },
				{ 24, 12 },
				{ 110, -53 },
				{ 110, 50 }
			},
			{
				{ { -110, -33 }, { -93, 35 } },
				{ { -110, -33 }, { -64, -55 } },
				{ { -93, 35 }, { 24, 12 } },
				{ { -64, -55 }, { 10, -19 } },
				{ { 18, -6 }, { 10, -19 } },
				{ { 18, -6 }, { 24, 12 } },
				{ { 24, 12 }, { 110, 50 } },
				{ { 18, -6 }, { 110, -53 } },
			}
		};
		static const Pattern pat4 = {
			{
				{ -47, -79 },
				{ -77, -14 },
				{ -61, 21 },
				{ -31, 49 },
				{ 4, 65 },
				{ 67, 48 },
				{ 82, -12 }
			},
			{
				{ { -77, -14 }, { -47, -79 } },
				{ { -77, -14 }, { -61, 21 } },
				{ { -61, 21 }, { -31, 49 } },
				{ { -31, 49 }, { 4, 65 } },
				{ { 67, 48 }, { 4, 65 } },
				{ { 67, 48 }, { 82, -12 } }
			}
		};
		
		class StarGroup final : public PhysicObject
		{
			const Pattern& pat;
			class E final : public ESPrefab
			{
				void on_create() override
				{
					ESPrefab::on_create();
					set_max_health(10);
					set_boundary(30, 30, 30, 30), set_boundary_test(false);
					sprite().set_scale(0.5, 0.5);
					sprite().set_alpha(0.0);
					set_invincible(true, appear_time);
					set_collide(false);
				}
				void on_update() override
				{
					ESPrefab::on_update(); int t = this->t();
					if (t <= appear_time) {
						double k = (double)t / appear_time;
						sprite().set_scale(0.5 + k * 0.5, 0.5 + k * 0.5);
						sprite().set_alpha(k);
					}
					if (t == appear_time)
						bind(nullptr), set_ay(-0.01), set_boundary_test(true), set_collide(true);
				}
			public:
				E() : ESPrefab(ESColor::blue, 0, 1) {}
			};
			class B final : public BBPrefab
			{
			public:
				void on_create() override
				{
					BBPrefab::on_create();
					sprite().set_blend(BlendMode::plus);
					sprite().set_scale(0.4, 0.4);
					set_collider_r(get_collide_r() * 0.8);
					sprite().set_alpha(0.0);
					set_can_be_killed_by_player(false);
					set_boundary_test(false);
					set_collide(false);
					set_omega(0.0);
					sprite().set_level(DrawLevel::enemy_bullet_5);
				}
				void on_update() override
				{
					BBPrefab::on_update(); int t = this->t();
					if (t <= appear_time) {
						double k = (double)t / appear_time;
						sprite().set_scale(0.4 + k * 0.4, 0.4 + k * 0.4);
						sprite().set_alpha(k);
					} t -= appear_time;
					if (t == 0) set_boundary_test(true), set_collide(true);
					if (t >= 0 && t <= link_time)
						set_omega(0.05 * t / link_time);
				}
				B() : BBPrefab(BBType::star, BBColor::blue) {}
			};
			class L final : public Laser 
			{
				Line line;
				void on_create() 
				{
					Laser::on_create();
					set_sprite(tex.bullet, coords.laser1[(int)BSColor::blue]);
					sprite().set_blend(BlendMode::plus);
					set_len(0.0), set_w(4);
					sprite().set_w_scale(-1.0);
					sprite().set_alpha(1.0);
					set_collider_interval(2.0);
					set_collider_w(1.0);
					set_head_light(true);
					set_tail_light(true);
					light_sprite().set_tex_and_coords(tex.bullet, coords.laser_light[(int)BBColor::blue]);
					light_sprite().set_blend(BlendMode::plus);
					set_can_be_killed_by_player(false);
					set_boundary_test(false);
					sprite().set_level(DrawLevel::enemy_bullet_5);
					light_sprite().set_level(DrawLevel::enemy_bullet_5);
					turn_on(0);
					{
						kill_particle().emplace_back();
						auto& p = kill_particle().back();
						p.lifeTime = 15, p.cnt = 5;
						p.scale = 0.8, p.rand_pos_r = 5.0;
						p.rand_v_direction = true, p.v = 2.0;
						p.set_option(Particle::Options::fade_out);
						p.set_option(Particle::Options::slow_down);
						p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
						p.sprite.set_color(Color(0.4, 0.4, 1.0));
						p.sprite.set_level(DrawLevel::enemy_bullet_1);
					}
				}
				void on_update()
				{
					Laser::on_update(); int t = this->t();
					t -= appear_time;
					if (t >= 0 && t <= link_time) {
						double k = double(t) / link_time;
						double x = line.a.x + (line.b.x - line.a.x) * k;
						double y = line.a.y + (line.b.y - line.a.y) * k;
						set_two_point(line.a.x, line.a.y, x, y);
					}
					if (t == link_time)
						set_boundary_test(true);
				}
			public:
				L(const Line& line) : line(line) {}
			};
			void on_create() override
			{
				PhysicObject::on_update();
				for (auto& p : pat.p) {
					auto e = os->create_object<E>();
					e->set_pos(p.x, p.y - 10);
					e->bind(this);
					auto b = os->create_object<B>();
					b->set_pos(p.x, p.y);
					b->bind(this);
				}
				for (auto& l : pat.l)
					os->create_object<L>(l)->bind(this);
			}
			void on_update() override
			{
				PhysicObject::on_update(); int t = this->t();
				if (child_list().empty()) destroy();
				if (t <= appear_time) {
					set_vy(2.0 * (1.0 - (double)t / appear_time));
				}
				if (t == appear_time) {
					set_ay(0.004);
					set_omega(0.0012 * lsgn(x() < gs->center_w));
					set_max_v(1.5);
				}
			}
		public:
			StarGroup(const Pattern& pat) : pat(pat) {}
		};

		class E1 final : public ESPrefab
		{
			class S final : public Shooter
			{
				class G : public PhysicObject
				{
					class B : public BSPrefab
					{
						double sx, sy;
						void on_create() override
						{
							BSPrefab::on_create();
							sprite().set_blend(BlendMode::plus);
							sprite().set_scale(0.8, 0.8);
							set_collider_r(get_collide_r() * 0.6);
						}
						void on_update() override
						{
							BSPrefab::on_update();	int t = this->t();
							if (t == 0) sx = x_relative(), sy = y_relative();
							if (t <= 45) {
								double k = 1.0 - t / 45.0;
								set_pos(sx * k, sy * k);
							}   t -= 45;
							if (t >= 0 && t <= 240) {
								double k = t / 30.0 * 10.0;
								set_pos(sx * -k, sy * -k);
							}
						}
					public:
						B() : BSPrefab(BSType::star, BSColor::sky_blue) {}
					};
					void on_create() override
					{
						PhysicObject::on_create();
						set_omega(0.01);
						set_rot(gs->rand_real(0.0, pi * 2.0));
						for (int i = 0; i < 40; i++) {
							auto [x, y] = get_star_pos(i / 40.0, 5.0);
							auto b = os->create_object<B>();
							b->set_pos(x, y);
							b->bind(this);
						}
					}
					void on_update() override
					{
						PhysicObject::on_update();
						if (child_list().empty()) destroy();
					}
				};
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_2);
					set_shot_interval(30);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = shot_t();

					point_to(player);
					ShotData d;
					d.v = 4.0;
					d.a = -0.02;
					d.v_min = 3.0;
					shoot<G>(d);
				}
			};
			double sx, sy, svx, svy; int st;
			void on_create() override
			{
				ESPrefab::on_create();
				set_max_health(45);
				append_shooter<S>();
				set_invincible(true, 30);
				set_boundary(200, 200, 200, 0);
			}
			void on_update() override
			{
				ESPrefab::on_update();	int t = this->t();
				if (t == 0)
				{
					sx = x(), sy = y();
					svx = gs->rand_real(4.0, 5.0);
					svy = gs->rand_real(-0.5, 0.5);
					st = gs->rand_int(30, 60);
				}
				if (t <= 60) {
					double k = (1.0 - t / 60.0) * 0.5 + 0.5;
					set_v(svx * k * lsgn(sx < gs->center_w), svy * k);
				}
				if (t >= 60 && t <= 180) {
					double k = (t - 60) / 120.0;
					set_vy(svy * 0.5 + k * 2.0);
				}
				if (t == st) start_all_shooter();
				if (t == st + 60) stop_all_shooter();
			}
		public:
			E1() : ESPrefab(ESColor::yellow, 2, 1) {}
		};
		class E2 final : public ESPrefab
		{
			class S final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_1);
					set_shot_interval(30);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); 
					set_rot(gs->rand_real(0.0, pi * 2.0));
					ShotData d;
					d.v = 3.0;
					d.set_cir_style(12, 5.0, true);
					shoot<BSPrefab>(d, BSType::star, BSColor::yellow);
					d.set_cir_style(12, 5.0, false);
					shoot<BSPrefab>(d, BSType::star, BSColor::green);
				}
			};
			double sx, sy, svx, svy; int st;
			void on_create() override
			{
				ESPrefab::on_create();
				set_max_health(15);
				append_shooter<S>();
				set_boundary(200, 200, 200, 0);
				set_invincible(true, 30);
			}
			void on_update() override
			{
				ESPrefab::on_update();	int t = this->t();
				if (t == 0)
				{
					sx = x(), sy = y();
					set_v(gs->rand_real(-0.2, 0.2), gs->rand_real(3.0, 4.0));
					st = gs->rand_int(15, 30);
				}
				if (t == st) start_all_shooter();
				if (y() >= 150) stop_all_shooter();
			}
		public:
			E2() : ESPrefab(ESColor::red, 2, 0) {}
		};
		class E3 final : public EBPrefab
		{
			class E final : public ESPrefab
			{
				class S final : public Shooter
				{
					void on_create() override
					{
						Shooter::on_create();
						set_shot_interval(1);
						set_omega(0.05 * lsgn(left));
						set_rot(left ? rot_offset : 2.0 * pi - rot_offset);
					}
					void on_shoot() override
					{
						Shooter::on_shoot(); int t = shot_t();
						if (t % 5 == 0) 
						{
							ShotData d;
							d.set_fan_style(3, 0.8);
							d.v = 3.5;
							shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
								b->sprite().set_blend(BlendMode::plus);
							}, BSType::star, BSColor::yellow);
							se.enemy_shoot_1->play();

							d.set_fan_style(1, 0.0);
							d.v = { 2.0, 2.2 };
							d.rot_offset = { (pi - 0.01) * lsgn(left), (pi + 0.01) * lsgn(left) };
							shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
								b->sprite().set_blend(BlendMode::plus);
							}, BSType::star, BSColor::pink);
						}
					}
					double rot_offset; bool left;
				public:
					S(bool left, double rot_offset) : left(left), rot_offset(rot_offset) {}
				};
				void on_create() override
				{
					ESPrefab::on_create();
					set_max_health(120);
					append_shooter<S>(left, rot_offset);
					set_invincible(true, 30);
					set_boundary(100, 100, 100, 100);
				}
				void on_update() override
				{
					ESPrefab::on_update();	int t = this->t();
					double angle = (t * 0.05 + rot_offset) * lsgn(left);
					set_pos(40.0 * cos(angle), 40.0 * sin(angle));
					if (t == 30) start_all_shooter();
					if (t == 240) stop_all_shooter();
				}
				void on_bound_obj_destroyed(PhysicObject* obj) override
				{
					ESPrefab::on_bound_obj_destroyed(obj);
					kill(true);
				}
				bool left;	double rot_offset;
			public:
				E(bool left, double rot_offset) : left(left), rot_offset(rot_offset), ESPrefab(ESColor::purple, 3, 1) {}
			};

			double sx, sy;
			void on_create() override
			{
				EBPrefab::on_create();
				set_max_health(900);
				set_invincible(true, 30);
				set_boundary(100, 100, 100, 100);
			}
			void on_update() override
			{
				EBPrefab::on_update(); int t = this->t();
				if (t == 0) {
					sx = x(), sy = y();
					for (int i = 0; i < 4; i++)
						os->create_object<E>(sx < gs->center_w, pi * 0.5 * i)->bind(this);
				}
				if (t <= 60) {
					double k = (1.0 - t / 60.0) * 0.8 + 0.2;
					set_v(3.0 * k * lsgn(sx < gs->center_w), 0.5 * k);
				}
			}
			void on_kill() override
			{
				EBPrefab::on_kill();
				clear_bullets(x(), y(), 15.0, false, true, 300.0);
			}
		public:
			E3() : EBPrefab(0, 10) {}
		};
		class E4 final : public ESPrefab
		{
			class S final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_1);
				}
				void on_shoot() override
				{
					Shooter::on_shoot();
					ShotData d;
					for (int i = 0; i < 80; i++) {
						auto [x, y] = get_star_pos(i / 80.0, 1.0);
						double v = sqrt(x * x + y * y);
						d.rot_offset = atan2(y, x) - (gs->center_w - this->x()) * 0.002;
						d.v = v * 9.0;
						d.v_min = v * 4.0;
						d.a = -(v * 5.0) / 120.0;
						static BSColor color[] = {
							BSColor::yellow,
							BSColor::green,
							BSColor::sky_blue
						};
						shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
							// b->sprite().set_blend(BlendMode::plus);
							b->set_can_be_killed_by_player(false);
						}, BSType::round_ring, color[bc]);
					}
					stop();
				}
				int bc;
			public:
				S(int bc) : bc(bc) {}
			};
			class B final : public BBPrefab
			{
				void on_create() 
				{
					BBPrefab::on_create();
					sprite().set_level(DrawLevel::enemy_bullet_4);
					set_boundary(1000, 1000, 1000, 30);
					set_omega(0.0);
					sprite().set_blend(BlendMode::plus);
					set_can_be_killed_by_player(false);
				}
				void on_update()
				{
					BBPrefab::on_update(); int t = this->t();
					if (t == 0) {
						set_pos(10 * lsgn(x() < gs->center_w), 10);
					} t -= 120;
					if (t >= 0 && t <= 60) {
						if (t == 0) {
							bind(nullptr);
							svx = (x() - gs->center_w) * 0.01;
							svy = 4.0;
						}
						double k = t / 60.0;
						set_v(svx * k, svy * k);
						set_omega(0.05 * k);
					}
					if (t >= 0 && t % 2 == 0) {
						static BSColor color[] = {
							BSColor::yellow,
							BSColor::green,
							BSColor::sky_blue
						};
						auto b2 = os->create_object<BSPrefab>(BSType::star, color[bc]);
						b2->set_can_be_killed_by_player(false);
						b2->set_pos(this), b2->set_ax(0.004), b2->set_max_v(2.0);
						b2->set_rot(gs->rand_real(0.0, pi * 2.0)), b2->set_a_to_rot();
						b2->set_ay(b2->ay() + 0.001);
						b2->sprite().set_level(DrawLevel::enemy_bullet_3);
						b2->add_task([b2]() { b2->kill(false); }, gs->rand_int(120, 240));
						se.enemy_shoot_2->play();
					}
				}
				int bc; double svx, svy;
			public:
				B(BBColor color, int bc) : BBPrefab(BBType::star, color), bc(bc) {}
			};
			double sx, sy, tx, ty; int bc, t_offset;
			void on_create() override
			{
				ESPrefab::on_create();
				set_max_health(15);
				append_shooter<S>(bc);
				set_boundary(200, 200, 200, 0);
				set_invincible(true, 1000);

				static BBColor color[] = {
					BBColor::yellow,
					BBColor::green,
					BBColor::sky_blue,
				};
				os->create_object<B>(color[bc], bc)->bind(this);
			}
			void on_update() override
			{
				ESPrefab::on_update();	int t = this->t();
				if (t == 0)
					sx = x(), sy = y();
				if (t <= 120)
				{
					double k = t / 120.0;
					move::straight(this, t, 0, 120, { sx, sy }, { tx, ty }, move::factor::ease_out);
				}
				if (t == 120)
					t_offset = (gs->center_w - abs(gs->center_w - x())) * 0.6;
				if (t == 150 + t_offset)
					start_all_shooter();
				if (t == 150 + t_offset + 5)
					kill(true);
			}
		public:
			E4(double tx, double ty, int bc, ESColor ecolor, int power, int point) 
				: tx(tx), ty(ty), bc(bc), ESPrefab(ecolor, power, point) {}
		};
		class E5 final : public EBPrefab
		{
			class S final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_1);
				}
				void on_shoot() override
				{
					Shooter::on_shoot();
					ShotData d;
					for (int i = 0; i < 120; i++) {
						auto [x, y] = get_star_pos(i / 120.0, 1.0);
						double v = sqrt(x * x + y * y);
						d.rot_offset = atan2(y, x) - (gs->center_w - this->x()) * 0.002;
						d.v = v * 8.0;
						d.v_min = v * 4.5;
						d.a = -(v * 3.5) / 120.0;
						shoot_with_func<BBPrefab>(d, [](BBPrefab* b) {
							b->sprite().set_blend(BlendMode::plus);
							b->sprite().set_level(DrawLevel::enemy_bullet_4);
							b->set_can_be_killed_by_player(false);
						}, BBType::star, BBColor::blue);
					}
					stop();
				}
			};
			class S2 final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shot_interval(4);
					set_shoot_se(se.enemy_shoot_2);
					start();
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = this->t();
					if (t >= 120) return stop();
					ShotData d;
					d.set_fan_style(8, pi * 0.8, 30.0);
					d.v = 4.0;
					double angle = sin(t * 0.01) * pi * 0.5 + pi * 0.1;
					set_rot(angle - pi * 0.5);
					shoot_with_func<BBPrefab>(d, [](BBPrefab* b) { b->set_can_be_killed_by_player(false); }, BBType::star, BBColor::black);
					set_rot(-angle - pi * 0.5);
					shoot_with_func<BBPrefab>(d, [](BBPrefab* b) { b->set_can_be_killed_by_player(false); }, BBType::star, BBColor::black);
				}
			};
			void on_create() override
			{
				EBPrefab::on_create();
				set_max_health(3600);
				set_invincible(true, 210);
				set_boundary(100, 100, 100, 100);
				append_shooter<S>();
				append_shooter<S2>();
			}
			void on_update() override
			{
				EBPrefab::on_update(); int t = this->t();
				if (t <= 120) 
				{
					if (t == 0) sx = x(), sy = y();
					move::straight(this, t, 0, 120, { sx, sy }, { sx, sy + 200 }, move::factor::ease_out);
				}
				if (t == 210) start_all_shooter();
				if (t == 390) kill(true);
			}
			void on_drop_item() override
			{
				EBPrefab::on_drop_item();
				gs->drop_item<BombItem>(this, 0, 1);
				gs->drop_item<PowerBigItem>(this, { 10, 20 }, 1);
			}
			double sx, sy;
		public:
			E5() : EBPrefab(20, 20) {}
		};

		if (t == 0)
		{
			os->create_object<StarGroup>(pat1)->set_pos(lx(150, 1), -30);
		}
		if (t == 240)
		{
			os->create_object<StarGroup>(pat2)->set_pos(lx(150, 0), -30);
		}
		if (t == 480)
		{
			os->create_object<StarGroup>(pat3)->set_pos(lx(150, 1), -30);
		}
		if (t == 720)
		{
			os->create_object<StarGroup>(pat4)->set_pos(lx(120, 0), -30);
		}

		if (t >= 0 && t <= 120 && t % 15 == 0) 
		{
			os->create_object<E1>()->set_pos(lx(-50, 0), 100);
		}
		if (t >= 150 && t <= 300 && t % 15 == 0)
		{
			os->create_object<E2>()->set_pos(lx(gs->rand_int(30, 300), 1), -50);
		}
		if (t >= 360 && t <= 480 && t % 15 == 0)
		{
			os->create_object<E1>()->set_pos(lx(-50, 1), 100);
		}
		if (t >= 510 && t <= 660 && t % 15 == 0)
		{
			os->create_object<E2>()->set_pos(lx(gs->rand_int(30, 300), 0), -50);
		}
		if (t == 720)
		{
			os->create_object<E3>()->set_pos(lx(-40, 1), 60);
		}
		if (t == 900)
		{
			os->create_object<E3>()->set_pos(lx(-40, 0), 60);
		}

		t -= 1080;
	 
		if (t == 0)
		{
			os->create_object<E4>(lx(35,	1),	100, 0, ESColor::blue, 0, 3)->set_pos(lx(-100,	1),	120);
			os->create_object<E4>(lx(70,	1),	110, 1, ESColor::blue, 0, 3)->set_pos(lx(-80,	1),	110);
			os->create_object<E4>(lx(105,	1),	120, 1, ESColor::red,  3, 0)->set_pos(lx(-60,	1),	100);
			os->create_object<E4>(lx(140,	1), 130, 2, ESColor::red,  3, 0)->set_pos(lx(-60,   1),	 90);
			os->create_object<E4>(lx(35,	0),	100, 0, ESColor::blue, 0, 3)->set_pos(lx(-100,	0),	120);
			os->create_object<E4>(lx(70,	0),	110, 1, ESColor::blue, 0, 3)->set_pos(lx(-80,	0),	110);
			os->create_object<E4>(lx(105,	0),	120, 1, ESColor::red,  3, 0)->set_pos(lx(-60,	0),	100);
			os->create_object<E4>(lx(140,	0), 130, 2, ESColor::red,  3, 0)->set_pos(lx(-60,	0),	 90);
		}
		if (t == 60)
		{
			os->create_object<E5>()->set_pos(gs->center_w, -50);
		}
		
		t -= 540;
	}

part7:	 // if (t == 0) bgm.bgm_1->set_time(5440);
	{
		class Boss final : public PBoss
		{
			class NA1 final : public NormalAttack
			{
				void on_create() override
				{
					NormalAttack::on_create();
					set_max_health(5000);
					set_time_limit(960);
					set_prepare_time(60);
				}
				void on_casting() override
				{
					NormalAttack::on_casting(); int t = get_from_start_time();

					if (t % 120 < 60 && t % 4 == 0)
					{
						class B final : public BBPrefab
						{
							class S1 final : public Shooter
							{
								BBColor color;
								void on_shoot() override
								{
									Shooter::on_shoot(); int t = shot_t();
									point_to(player);
									set_rot(rot() + gs->rand_real(-0.12, 0.12));
									ShotData d;
									d.set_cir_style(18);
									d.v = 4.0;
									shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
										b->set_drag(10);
										b->set_drag_scale(0.8);
										b->set_drag_alpha(0.3);
										b->set_boundary(30, 30, 100, 30);
									}, BSType::star, bb_to_bs_color[(int)color], true);
									se.enemy_shoot_1->play();
									stop();
								}
							public:
								S1(BBColor color) : color(color) {}
							};
							S1* s;	BBColor color;
							double sx, sy, tx, ty;
							void on_create() override
							{
								BBPrefab::on_create();
								set_boundary(30, 30, 100, 30);
								s = os->create_object<S1>(color);
							}
							void on_update() override
							{
								BBPrefab::on_update();	int t = this->t();
								if (t == 0)
									sx = x(), sy = y();
								if (t <= 15)
									move::straight(this, t, 0, 15, { sx, sy }, { tx, ty }, move::factor::linear);
								if (t == 15)
									s->set_pos(this), s->start();
								if (t == 16)
									kill(false), s->destroy();
							}
						public:
							B(double tx, double ty, BBColor color) : tx(tx), ty(ty), BBPrefab(BBType::star, color), color(color) {}
						};
						static const BBColor color[] = {
							BBColor::yellow,
							BBColor::green,
							BBColor::sky_blue,
						};
						os->create_object<B>(
							x() + lsgn(gs->rand_int(0, 1)) * gs->rand_real(20, 100), 
							y() + gs->rand_real(-50, -80), color[t / 4 % 3])
						->set_pos(this);
					}

					if (t % 120 == 60)
					{
						get_boss()->move_randomly(45, 0.5, { -30, 30 }, { -10, 10 },
							{ lx(100, 1), lx(100, 0) }, { get_boss()->get_game_center_y() - 20, get_boss()->get_game_center_y() + 20 },
							move::factor::ease_out);
					}
				}
				void on_start() override
				{
					NormalAttack::on_start();
					get_boss()->set_magic_circle(true);
				}
				void on_end() override
				{
					NormalAttack::on_end();
					get_boss()->move_to_center(60, move::factor::ease_out);
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
						s.set_tex_and_coords(tex.bg_1, coords.bg_1);
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

				class Star final : public BBPrefab
				{
					class L final : public Laser
					{
						void on_create() override
						{
							Laser::on_create();
							set_sprite(tex.bullet, coords.laser1[(int)bb_to_bs_color[(int)color]]);
							sprite().set_blend(BlendMode::plus);
							set_full_len(80.0);
							set_len(0.0);
							sprite().set_w_scale(-1.0);
							set_w(5);
							set_collider_interval(2.0);
							set_collider_w(1.0);
							set_can_be_killed_by_player(true);
							turn_on(0);
							sprite().set_level(DrawLevel::enemy_bullet_4);

							{
								kill_particle().emplace_back();
								auto& p = kill_particle().back();
								p.lifeTime = 15, p.cnt = 5;
								p.scale = 0.8, p.rand_pos_r = 5.0;
								p.rand_v_direction = true, p.v = 2.0;
								p.set_option(Particle::Options::fade_out);
								p.set_option(Particle::Options::slow_down);
								p.sprite.set_tex_and_coords(tex.bullet, coords.bullet_kill_particle);
								p.sprite.set_color(bb_color[(int)color]);
								p.sprite.set_level(DrawLevel::enemy);
							}
						}
						void on_drop_item(double x, double y) override
						{
							Laser::on_drop_item(x, y);
							GameSystem::instance()->drop_item<PointSmallItem>(x, y, 0, 1);
						}
						BBColor color;
					public:
						L(BBColor color) : color(color) {}
					};
					void on_create() override
					{
						BBPrefab::on_create();
						sprite().set_blend(BlendMode::plus);
						sprite().set_scale(0.5, 0.5);
						sprite().set_alpha(0.0);
						set_collide(false);
						sprite().set_level(DrawLevel::enemy_bullet_5);
					}
					void on_update() override
					{
						BBPrefab::on_update(); int t = this->t();
						if (t <= 30)
						{
							double k = t / 30.0;
							sprite().set_scale(k * 0.5 + 0.5, k * 0.5 + 0.5);
							sprite().set_alpha(k);
							set_v(0.0, -4.0 * (1.0 - k));
							if (t == 30)
								set_collide(true);
						}
						if (t == 30)
						{
							if (type == 0)
							{
								set_v(0.0, 0.0);
								set_a(0.0, 0.2);
								set_max_v(10.0);
							}
							else if(type == 1)
							{
								set_v(0.0, 0.0);
								set_a(0.0, 0.2);
								set_max_v(10.0);
								double r = get_point_to_rot(player);
								point_a_to(r + gs->rand_real(-0.02, 0.02));
							}
							auto l = os->create_object<L>(color);
							l->set_pos(this);
							l->set_v(vx(), vy());
							l->set_a(ax(), ay());
							l->set_max_v(get_max_v());
							l->set_anchored(true);
							se.enemy_shoot_2->play();
						}
					}
					BBColor color;	int type;
				public:
					Star(BBColor color, int type) : BBPrefab(BBType::star, color), color(color), type(type) {}
				};

				double px, py;

				void on_create() override
				{
					SpellCard::on_create();
					set_background<Bg>();
					set_subtitle(L"星符");
					set_title(L"流星般的魔法使");
					set_max_score(1000000);
					set_time_limit(1800);
					set_max_health(4000);
				}
				void on_casting() override
				{
					SpellCard::on_casting(); 

					int t = get_from_start_time() % 720;
					int left = (get_from_start_time() / 660) & 1;
					auto boss = get_boss();
					double cx = boss->get_game_center_x(), cy = boss->get_game_center_y();

					if (t >= 0 && t <= 180)
					{
						move::bezier3(boss, t, 0, 180,
							{ cx, cy }, { 800.0 * lsgn(left), -60 },
							{ cx, cy + 50 }, { -800.0 * lsgn(left), 0 },
							move::factor::ease_in_out<40, 40>);
						if (t % 2 == 0)
						{
							double x = boss->x() + gs->rand_real(-100.0, 100.0);
							double y = boss->y() + gs->rand_real(-80.0, 80.0);
							os->create_object<Star>(t / 3 % 2 ? BBColor::sky_blue : BBColor::blue, 0)->set_pos(x, y);
							se.enemy_shoot_1->play();

							for(int i = 0; i < 2; i++)
							{
								double x = boss->x() + gs->rand_real(-80.0, 80.0);
								double y = boss->y() + gs->rand_real(-80.0, 20.0);
								auto b = os->create_object<BSPrefab>(BSType::star, (t / 2) % 2 ? BSColor::sky_blue : BSColor::blue, true);
								b->sprite().set_blend(BlendMode::plus);
								b->set_pos(x, y);
								b->set_v(0.0, -gs->rand_real(1.0, 2.0));
								b->set_a(gs->rand_real(-0.04, 0.04),
										 gs->rand_real(-0.04, -0.08));
								b->set_max_v(5.0);
								b->add_task([b]() { b->kill(false); }, gs->rand_int(120, 180));
							}
						}
					}	t -= 240;

					if (t >= 0 && t <= 90)
					{
						if (t == 0)  px = player->x();
						if (t == 30) boss->charge();
						for (int i = 0; i < 6; i++) 
							if (t == 30 + i * 5)
								create_warning(px, cy - 30 + i * 60, 60);
						if (t <= 60)
							move::straight(boss, t, 0, 60,
								{ cx, cy + 50 }, { px, cy - 80 }, move::factor::ease_out);
					}  t -= 90;

					if (t >= 0 && t <= 60)
					{
						move::straight(boss, t, 0, 60,
							{ px, cy - 80 }, { px, gs->game_h + 80 }, move::factor::ease_in<80>);
						if (t % 1 == 0)
						{
							double r = (1.0 - t / 60.0) * 30.0 + 20.0;
							double angle = t * (t / 20.0 + 1.0) * 0.05;
							for (int i = 0; i < 4; i++) {
								double a = angle + pi * 2.0 / 4.0 * i;
								auto b = os->create_object<BSPrefab>(BSType::star, i % 2 ? BSColor::yellow : BSColor::green, true);
								b->sprite().set_blend(BlendMode::plus);
								b->set_pos(r * cos(a) + boss->x(), r * sin(a) + boss->y());
								b->add_task([a, b]() {
									b->set_a(0.01 * cos(a) + gs->rand_real(-0.002, 0.002), 
											 0.01 * sin(a) + gs->rand_real(-0.002, 0.002));
									b->set_max_v(3.0);
								}, 60);
								b->add_task([b]() { b->kill(false); }, gs->rand_int(120, 180));

								class B final : public BSPrefab
								{
									void on_create() override
									{
										BSPrefab::on_create();
										set_drag(15);
										set_drag_interval(1);
										sprite().set_blend(BlendMode::plus);
										set_drag_alpha(0.5);
										set_drag_scale(0.8);
										sprite().set_alpha(0.0), sprite().set_scale(0.0, 0.0);
										set_collide(false);
										set_can_be_killed_by_player(false);
									}
									void on_update() override
									{
										BSPrefab::on_update();
										double flash = DrawSystem::instance()->rand_real(0.6, 1.0);
										if (t() <= 60.0)
										{
											double k = sqrt(t() / (double)60.0);
											sprite().set_alpha(flash * k), sprite().set_scale(k, k);
										}
										else
											set_collide(true), set_can_be_killed_by_player(true), sprite().set_alpha(flash);
									}
								public:
									B(BSColor color) : BSPrefab(BSType::star, color) {}
								};
								auto b2 = os->create_object<B>((t + i) % 2 ? BSColor::yellow : BSColor::green);
								b2->set_pos(r* cos(a) + boss->x(), r * sin(a) + boss->y());
								b2->set_v(gs->rand_real(-3.0, 3.0), -gs->rand_real(3.0, 4.0));
								b2->set_ay(0.03);
								b2->set_max_v(5.0);
							}
							se.enemy_shoot_1->play();
						}
					}  t -= 120;

					if (t >= 0 && t <= 150)
					{
						if (t == 0) px = player->x(), py = player->y();

						double angle = atan2(py - cy, px - cx);
						double tx = cx + cos(angle) * 700.0, ty = cy + sin(angle) * 700.0;
						double dx = (cx - tx) * 0.1, dy = (cy - ty) * 0.1;
						for (int i = 0; i < 11; i++) if (t == i * 3)
							create_warning(tx + dx * i, ty + dy * i, 60);

						if (t >= 60)
						{
							move::straight(boss, t, 60, 150,
								{ tx, ty }, { cx, cy }, move::factor::ease_out<80>);

							if (t % 3 == 0 && boss->y() < gs->game_w)
							{
								double x = boss->x() + gs->rand_real(-50.0, 50.0);
								double y = boss->y() + gs->rand_real(-50.0, 50.0);
								os->create_object<Star>(BBColor::red, 1)->set_pos(x, y);
								se.enemy_shoot_1->play();
							}
						}
					}
				}
			};

			void on_create() override
			{
				PBoss::on_create();

				set_still_frame_index(0);
				set_left_move_frame_index(1);
				set_right_move_frame_index(2);
				set_still_from_left_frame_index(3);
				set_still_from_right_frame_index(4);
				set_casting_frame_index(5);
				set_sprite(tex.boss_1, coords.boss_1);
				sprite().set_scale(0.8, 0.8);

				set_kill_when_attacks_end(false);

				append_attack<NA1>();
				append_attack<SC1>();

				set_pos(-100, -100);
				move_to_center(90, move::factor::ease_out);
				add_task([this]() { start_attacks(); }, 60);
			}

			int st = -1e9;
			double sx, sy;
			bool dropped = false;
			void on_update() override
			{
				PBoss::on_update(); int t = this->t();
				if (is_attack_ended())
				{
					set_collide(false);
					if (st < 0) st = t, sx = x(), sy = y();
					if (t == st + 60) kill_raw();
					move::straight(this, t, st, st + 60, { sx, sy }, { gs->center_w, -60 });
					if (y() <= gs->center_h && !dropped)
					{
						dropped = true;
						gs->drop_item<LifeItem>(this, 0, 1);
						gs->drop_item<PowerItem>(this, { 60, 100 }, 50);
						gs->drop_item<PointItem>(this, { 60, 100 }, 50);
					}
				}
			}
		};

		if (t == 0)
		{
			gs->clear_all_enemy_bullets(false);
			os->create_object<Boss>();
		} 

		t -= 1885;
	}

part8:	 // if (t == 0) bgm.bgm_1->set_time(7325);
	{
		if (!gs->get_current_boss_attack())
		{
			static const int h_interval = 70;
			static const int w_interval = 75;

			class E1 final : public ESPrefab
			{
				bool left = false;
				double sx, sy;
				class S final : public Shooter
				{
					bool left;
				public:
					S(bool left) : left(left) {}
					void on_create() override
					{
						Shooter::on_create();
						set_shot_interval(60);
					}
					void on_shoot() override
					{
						Shooter::on_shoot();
						point_to(player);
						ShotData d;
						d.set_cir_style(48, 0.0, false);
						d.a = -0.04;
						d.v = 3.0;
						d.v_min = 2.0;
						shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
							b->sprite().set_level(DrawLevel::enemy_bullet_3);
							}, BSType::star, BSColor::white);
					}
				};
				class B final : public BBPrefab
				{
					double sx, sy, tx;
				public:
					B(bool left, double tx) : BBPrefab(BBType::star, left ? BBColor::yellow : BBColor::purple), tx(tx) {}
					void on_create() {
						BBPrefab::on_create();
						// sprite().set_blend(BlendMode::plus);
						set_drag(20);
						set_drag_shrink(true);
						set_drag_fade(true);
						set_drag_alpha(0.4);
						set_drag_scale(0.8);
						sprite().set_scale(1.2, 1.2);
						set_collider_r(get_collide_r() * 1.1);
						set_can_be_killed_by_player(false);
					}
					void on_update() {
						BBPrefab::on_update();	int t = this->t();
						if (t == 0)		sx = x(), sy = y();
						if (t <= 30)	move::straight(this, t, 0, 30, { sx, sy }, { tx, sy }, move::factor::ease_out);
						if (t >= 45) {
							t = (t - 45) % 90;
							if (t <= 30)
							{
								if (t == 0) sx = x(), sy = y(), se.enemy_shoot_1->play();
								move::straight(this, t, 0, 30, { sx, sy }, { sx, sy + h_interval * 2 }, move::factor::ease_out<80>);
							}
						}
					}
				};
				void on_create() override
				{
					ESPrefab::on_create();
					set_max_health(60);
					set_boundary(40, 40, 0, 0);
					set_invincible(true, 46);
					append_shooter<S>(left);
				}
				void on_update() override
				{
					ESPrefab::on_update(); int t = this->t();
					if (t == 0)		sx = x(), sy = y();
					if (t <= 45)	move::straight(this, t, 0, 45, { sx, sy }, { sx + 50 * lsgn(left), sy }, move::factor::ease_out);
					if (t == 45)	set_v(1.0 * lsgn(left), 0.3);
					if (t == 45) {
						double tx = 21, sy = y();
						for (int i = 0; tx + i * w_interval <= gs->game_w - 21; i++)
							os->create_object<B>(left, lx(tx + i * w_interval, left))->set_pos(this);
						se.enemy_shoot_1->play();
					}
					if (t == 60)
						start_all_shooter();
				}
			public:
				E1(bool left) : ESPrefab(ESColor::purple, 1, 1), left(left) {}
			};

			if (t >= 0 && t / 45 < 12)
			{
				if (t % 90 == 0)
					os->create_object<E1>(0)->set_pos(lx(-30, 0), 17);
				if (t % 90 == 45)
					os->create_object<E1>(1)->set_pos(lx(-30, 1), 17 + h_interval);
			} 
		}
		t -= 815;
	}

part9:	 // if (t == 0) bgm.bgm_1->set_time(8140);
	{
		if (!gs->get_current_boss_attack())
		{
			class E1 final : public EBPrefab
			{
				class S1 final : public Shooter
				{
					class B final : public BSPrefab
					{
						void on_create() override
						{
							BSPrefab::on_create();
							sprite().set_blend(BlendMode::plus);
							sprite().set_scale(0.4, 0.4);
							set_collider_r(get_collide_r() * 0.5);
						}
						void on_update() override
						{
							BSPrefab::on_update(); int t = this->t();
							if (t == 0) {
								point_v_to(cx, cy), set_v(-vx(), -vy());
								point_a_to(cx, cy), set_a(-ax(), -ay());
							}
							point_v_to(get_v_rot() - 0.01 * lsgn(left));
							point_a_to(get_a_rot() - 0.01 * lsgn(left));
							if (t <= 60)
							{
								double k = t / 120.0 + 0.5;
								sprite().set_scale(k * 0.8, k * 0.8);
								sprite().set_alpha(k);
							}
							
						}
						bool left; double cx, cy;
					public:
						B(bool left, double cx, double cy) : cx(cx), cy(cy), left(left), BSPrefab(BSType::star, BSColor::sky_blue, 1) {}
					};
					class B2 final : public BSPrefab
					{
						void on_create() override
						{
							BSPrefab::on_create();
							sprite().set_blend(BlendMode::plus);
							sprite().set_scale(0.4, 0.4);
							set_collider_r(get_collide_r() * 0.6);
						}
						void on_update() override
						{
							BSPrefab::on_update();	int t = this->t();
							if (t == 0)
								kill_time = gs->rand_int(10, 120);
							if (t >= kill_time)
								kill(false);
							point_v_to(get_v_rot() - 0.01 * lsgn(left));
							point_a_to(get_a_rot() - 0.01 * lsgn(left));
							if (t <= 60)
							{
								double k = t / 120.0 + 0.5;
								sprite().set_scale(k * 0.8, k * 0.8);
								sprite().set_alpha(k);
							}
						}
						bool left; int kill_time;
					public:
						B2(bool left) : left(left), BSPrefab(BSType::star, BSColor::white, 1) {}
					};
					void on_create() override
					{
						Shooter::on_create();
						set_shoot_se(se.enemy_shoot_2);
						set_shot_interval(3);
						set_omega(0.015 * lsgn(left));
						set_rot(lsgn(left) ? pi * 0.1 : pi * 1.8);
					}
					void on_shoot() override
					{
						Shooter::on_shoot();
						ShotData d;
						d.set_cir_style(5);
						d.v = 0.5;
						d.v_max = 3.0;
						d.a = 0.02;
						d.x_offset = { -10, 10 };
						d.y_offset = { -10, 10 };
						d.r_offset = 40.0;
						shoot<B>(d, left, x(), y());

						d.x_offset = { -10, 10 };
						d.y_offset = { -10, 10 };
						d.rot_offset = pi * 0.2;
						set_rot(-rot());
						shoot<B2>(d, left);
						set_rot(-rot());
					}
				public:
					bool left;
					S1(bool left) : left(left) {}
				};
				class S2 final : public Shooter
				{
					class B final : public BSPrefab
					{
						void on_create() override
						{
							BSPrefab::on_create();
							sprite().set_level(DrawLevel::enemy_bullet_3);
							set_drag(10);
							set_drag_scale(0.8);
							set_drag_alpha(0.3);
						}
						void on_update() override
						{
							BSPrefab::on_update();
							point_v_to(get_v_rot() + 0.006 * lsgn(left));
							point_a_to(get_a_rot() + 0.006 * lsgn(left));
						}
						bool left;
					public:
						B(BSColor color, bool left) : left(left), BSPrefab(BSType::star, color, 1) {}
					};

					void on_create() override
					{
						Shooter::on_create();
						set_shot_interval(90);
					}
					void on_shoot() override
					{
						Shooter::on_shoot();	
						if (get_shot_count() == 0)
							return;
						point_to(player);
						ShotData d;
						d.set_cir_style(30, 5.0, 1);
						d.v = 3.8;
						shoot<B>(d, BSColor::yellow, left);
						d.set_cir_style(30, 5.0, 0);
						d.v = 3.4;
						shoot<B>(d, BSColor::orange, left);
						se.enemy_shoot_1->play();
					}
				public:
					bool left;
					S2(bool left) : left(left) {}
				};
				void on_create() override
				{
					EBPrefab::on_create();
					set_max_health(1000);
					append_shooter<S1>(left);
					append_shooter<S2>(left);
					start_all_shooter();
					set_boundary(50, 50, 200, 0);
					set_invincible(true, 120);
				}
				void on_update() override
				{
					EBPrefab::on_update(); int t = this->t();
					if (t == 0) sx = x(), sy = y();
					if (t <= 90) {
						move::straight(this, t, 0, 90,
							{ sx, sy }, { sx, sy + 200 }, move::factor::ease_out);
					}
					if (t == 90)
						set_vy(0.1);
					if (t == 300) set_ay(-0.02);
				}
				void on_drop_item() override
				{
					EBPrefab::on_drop_item();
					gs->drop_item<PowerBigItem>(this, 0.0, 1);
				}
				double sx, sy; bool left;
			public:
				E1(bool left) : EBPrefab(5, 10), left(left) {}
			};

			if (t == 0)
			{
				os->create_object<E1>(1)->set_pos(lx(120, 1), -100);
			}
			if (t == 360)
			{
				os->create_object<E1>(0)->set_pos(lx(120, 0), -100);
			}
		}
		t -= 720;
	}

part10:  // if (t == 0) bgm.bgm_1->set_time(8860);
	{
		class EB : public EBPrefab
		{
		protected:
			virtual void on_create() override
			{
				EBPrefab::on_create();
				set_max_health(1000);
				set_boundary_test(false);
				set_invincible(true, 12000);
			}
			virtual void on_update() override
			{
				EBPrefab::on_update(); int t = this->t();
				rev_rot += rev_omega;
				rev_r += rev_dr;
				set_pos(rev_r * cos(rev_rot) + gs->center_w, rev_r * sin(rev_rot) + gs->center_h);
			}
			virtual void on_kill() override
			{
				EBPrefab::on_kill();
				auto l = child_list();
				for (auto c : l)
					if (auto p = dynamic_cast<ESPrefab*>(c))
						p->kill();
			}
			virtual void on_drop_item() override
			{
				EBPrefab::on_drop_item();
				
			}
			virtual void on_draw() override
			{
				EBPrefab::on_draw();
				for (auto r : orbits)
				{
					ds->draw_func([=](sf::RenderTarget* target) {
						sf::CircleShape cir;
						cir.setOrigin(r, r);
						cir.setRadius(r);
						cir.setPosition(x(), y());
						cir.setFillColor(Color(0, 0, 0, 0));
						cir.setOutlineColor(Color(1, 1, 1, 0.5));
						cir.setOutlineThickness(2);
						cir.setPointCount(200);
						target->draw(cir);
					}, DrawLevel::item_1);
				}
			}
			int pt() const { return st + t(); }
			double rev_omega, rev_r = 0.0, rev_rot = 0.0, rev_dr = 0.0;
			std::vector<double> orbits;	int st;
		public:
			EB(int st, const std::vector<double>& orbits) : st(st), EBPrefab(5, 10), orbits(orbits) {}
		};
		class ES : public ESPrefab
		{
		protected:
			virtual void on_create() override
			{
				ESPrefab::on_create();
				set_max_health(600);
				set_boundary_test(false);
			}
			virtual void on_update() override
			{
				ESPrefab::on_update(); int t = this->t();
				angle += omega;
				set_pos(r * cos(angle), r * sin(angle));
			}
			virtual void on_drop_item() override
			{
				ESPrefab::on_drop_item();

			}
			int pt() const { return st + t(); }
			double get_angle() const { return t() * omega + rot_offset; }
			double r, omega, rot_offset, angle;	int st;
		public:
			ES(int st, ESColor color, int power, int point, double r, double omega, double rot_offset) 
				: ESPrefab(color, power, point), st(st), r(r), omega(omega), angle(rot_offset), rot_offset(rot_offset) {}
		};

		static const ESColor e_color[] = { ESColor::red, ESColor::yellow, ESColor::blue };
		static const int e_power[] = { 5, 3, 0 };
		static const int e_point[] = { 0, 3, 5 };
		static const BSColor b1_color[] = { BSColor::red, BSColor::cyan, BSColor::blue };
		static const BSColor b2_color[] = { BSColor::orange, BSColor::green, BSColor::sky_blue };
		static const BBColor b3_color[] = { BBColor::red, BBColor::green, BBColor::blue };

		static auto warn_circle = [](int t, int st, double x, double y, double r = 200.0) {
			for (int i = 0; i < 20; i++) if (t == st + i * 3)
				create_warning(x + r * cos(pi / 10.0 * i), y + r * sin(pi / 10.0 * i), 90);
		};

		class LineStar : public BSPrefab
		{
			void on_create() override
			{
				BSPrefab::on_create();
				set_collide(false);
				set_can_be_killed_by_player(false);
				set_pos(sx, sy);
				set_vx(6.0);
				point_to(tx, ty);
				set_v_to_rot();

				sprite().set_blend(BlendMode::plus);
				sprite().set_alpha(0.6);
				sprite().set_scale(0.6, 0.6);
				set_drag(50);
				set_drag_alpha(0.5);
			}
			double sx, sy, tx, ty;
		public:
			LineStar(double sx, double sy, double tx, double ty) :
				BSPrefab(BSType::star, BSColor::red), sx(sx), sy(sy), tx(tx), ty(ty) {}
		};

		class ES1 final : public ES
		{
			class S2 final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_2);
					set_shot_interval(7);
					set_omega(omega);
					set_rot(rot_offset);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = shot_t();
					ShotData d;
					d.v = 3.0;
					d.a = -0.01;
					d.v_min = 2.0;
					shoot<BSPrefab>(d, BSType::star, b1_color[c]);
				}
				double omega, rot_offset; int c;
			public:
				S2(int c, double rot_offset, double omega) : c(c), rot_offset(rot_offset), omega(omega) {}
			};
			class S3 final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_2);
					set_shot_interval(15);
					set_omega(omega);
					set_rot(rot_offset);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = shot_t();
					ShotData d;
					d.v = 3.0;
					d.a = -0.01;
					d.v_min = 2.0;
					shoot<BSPrefab>(d, BSType::star, b1_color[c]);
				}
				double omega, rot_offset; int c;
			public:
				S3(int c, double rot_offset, double omega) : c(c), rot_offset(rot_offset), omega(omega) {}
			};
			class S4 final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_1);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = shot_t();
					point_to(tx, ty);
					ShotData d;
					d.a = 0.03, d.v = 0.0;
					for (int i = 0; i < 8; i++)
					{
						d.v_max = 1.0 + i * 0.4;
						d.set_fan_style(1, 0.0);
						shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
							b->set_boundary(200, 200, 200, 200);
							b->set_can_be_killed_by_player(false);
							b->sprite().set_blend(BlendMode::plus);
						}, BSType::star, b1_color[c]);
						d.set_fan_style(16, pi * 1.5);
						d.rot_offset = d.rot_offset + pi;
						shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
							b->set_boundary(200, 200, 200, 200);
							b->set_can_be_killed_by_player(false);
							b->sprite().set_blend(BlendMode::plus);
						}, BSType::star, b1_color[c]);
					}
					stop();
				}
				int c; double tx, ty;
			public:
				S4(double tx, double ty, int c) : tx(tx), ty(ty), c(c) {}
			};
			void on_create() override
			{
				ES::on_create();
				append_shooter<S2>(c, get_angle(), omega);
				start_all_shooter();
				set_invincible(true, 60);
				set_max_health(100);
			}
			void on_update() override
			{
				ES::on_update(); int t = this->t(), pt = this->pt(); if (is_killed()) return;
				if (t == 360 && pt < 1020) {
					clear_all_shooter();
					append_shooter<S3>(c, get_angle(), omega);
					start_all_shooter();
				}
				if (pt == 1020)
				{
					clear_all_shooter();
					sprite().set_alpha(0.8);
					set_can_hit_player(false);
				}
				if (pt >= 1020 && pt <= 1140) {
					double k = (pt - 1020) / 120.0;
					omega = utils::lerp(0.02, 0.005, k);
				}
				if (pt >= 1200 && pt <= 1260)
				{
					double k = (pt - 1200) / 60.0;
					omega = utils::lerp(0.005, 0.0, k);
				}
				if (get_dist(gs->center_w, gs->center_h) > 80.0)
				{
					if (pt == 1260)
					{
						double px = player->x(), py = player->y();
						px = gs->center_w, py = gs->center_h;
						os->create_object<LineStar>(x(), y(), px, py);	
						clear_all_shooter();
						append_shooter<S4>(px, py, c);
					}
					if (pt == 1290)
					{
						start_all_shooter();
					}
				}
			}
			int c;
		public:
			ES1(int st, int c, double rot_offset) : c(c), ES(st, e_color[c], e_power[c], e_point[c], 200.0, 0.02, rot_offset) {}
		};
		class ES2 final : public ES
		{
			class S1 final : public Shooter
			{
				class G : public PhysicObject
				{
					class B : public BSPrefab
					{
						double sx, sy;
						void on_create() override
						{
							BSPrefab::on_create();
							sprite().set_blend(BlendMode::plus);
							set_boundary_test(false);
						}
						void on_update() override
						{
							BSPrefab::on_update();	int t = this->t();
							if (t == 0) sx = x_relative(), sy = y_relative();
							if (t >= 0 && t <= 150) {
								double k = t / 150.0 * 8.0 + 1.0;
								set_pos(sx * k, sy * k);
							}
							if (t == 360)
								set_boundary_test(true);
						}
					public:
						B(int c) : BSPrefab(BSType::star, b2_color[c]) {}
					};
					void on_create() override
					{
						PhysicObject::on_create();
						set_omega(0.01);
						set_rot(gs->rand_real(0.0, pi * 2.0));
						for (int i = 0; i < 20; i++) {
							auto [x, y] = get_star_pos(i / 20.0, 5.0);
							auto b = os->create_object<B>(c);
							b->set_pos(x, y);
							b->bind(this);
						}
					}
					void on_update() override
					{
						PhysicObject::on_update();
						if (child_list().empty()) destroy();
					}
					int c;
				public:
					G(int c) : c(c) {}
				};
				int next_shoot_time = 0;
				void on_create() override
				{
					Shooter::on_create();
					set_shot_interval(1);
					next_shoot_time = gs->rand_int(60, 240);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = shot_t();
					point_to(player);
					if (t >= next_shoot_time)
					{
						ShotData d;
						d.v = 4.0;
						d.a = -0.01;
						d.v_min = 3.5;
						shoot<G>(d, c);
						next_shoot_time += gs->rand_int(90, 150);
						se.enemy_shoot_1->play();
						if(get_shot_count() == 1)
							stop();
					}
				}
				double omega, rot_offset; int c;
			public:
				S1(int c, double rot_offset, double omega) : c(c), rot_offset(rot_offset), omega(omega) {}
			};
			class S4 final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_1);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = shot_t();
					point_to(tx, ty);
					ShotData d;
					d.a = 0.03, d.v = 0.0;
					for (int i = 0; i < 8; i++)
					{
						d.v_max = 1.0 + i * 0.2;
						d.set_fan_style(1, 0.0);
						shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
							b->set_boundary(200, 200, 200, 200);
							b->set_can_be_killed_by_player(false);
							b->sprite().set_blend(BlendMode::plus);
						}, BSType::star, b1_color[c]);
						d.set_fan_style(16, pi * 1.5);
						d.rot_offset = d.rot_offset + pi;
						shoot_with_func<BSPrefab>(d, [](BSPrefab* b) {
							b->set_boundary(200, 200, 200, 200);
							b->set_can_be_killed_by_player(false);
							b->sprite().set_blend(BlendMode::plus);
						}, BSType::star, b1_color[c]);
					}
					stop();
				}
				int c; double tx, ty;
			public:
				S4(double tx, double ty, int c) : tx(tx), ty(ty), c(c) {}
			};
			void on_create() override
			{
				ES::on_create();
				append_shooter<S1>(c, get_angle(), omega);
				set_invincible(true, 1200);
				start_all_shooter();
			}
			void on_update() override
			{
				ES::on_update(); int t = this->t(), pt = this->pt(); if (is_killed()) return;
				if (t == 360 || pt == 1020)
					stop_all_shooter();
				if (pt == 1020)
				{
					clear_all_shooter();
					sprite().set_alpha(0.6);
					set_collide(false);
				}
				if (pt >= 1020 && pt <= 1140) {
					double k = (pt - 1020) / 120.0;
					omega = utils::lerp(-0.02, -0.005, k);
				}
				if (pt >= 1200 && pt <= 1260)
				{
					double k = (pt - 1200) / 60.0;
					omega = utils::lerp(-0.005, 0.0, k);
				}
				if (get_dist(gs->center_w, gs->center_h) > 80.0)
				{
					if (pt == 1260)
					{
						double px = player->x(), py = player->y();
						px = gs->center_w, py = gs->center_h;
						os->create_object<LineStar>(x(), y(), px, py);
						clear_all_shooter();
						append_shooter<S4>(px, py, c);
					}
					if (pt == 1290)
					{
						start_all_shooter();
					}
				}
			}
			int c;
		public:
			ES2(int st, int c, double rot_offset) : c(c), ES(st, e_color[c], e_power[c], e_point[c], 150.0, -0.02, rot_offset) {}
		};
		class EB1 final : public EB
		{
			class S1 final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_1);
					set_shot_interval(30);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = shot_t();
					point_to(player);
					ShotData d;
					if (get_shot_count() == 0)
					{
						d.set_cir_style(40, 5.0);
						d.v = 3.0;
						d.a = -0.02;
						d.v_min = 1.5;
					}
					else
					{
						d.set_cir_style(16, 5.0);
						d.v = 1.5;
						d.a = -0.04;
						d.v_min = 1.3;
					}
					shoot_with_func<BBPrefab>(d, [](BBPrefab* b) {
						b->set_boundary(200, 200, 200, 200);
						b->set_can_be_killed_by_player(false);
					}, BBType::star, b3_color[c]);
					if (get_shot_count() == 0)
						stop();
				}
				int c;
			public:
				S1(int c) : c(c) {}
			};
			class S2 final : public Shooter
			{
				void on_create() override
				{
					Shooter::on_create();
					set_shoot_se(se.enemy_shoot_1);
				}
				void on_shoot() override
				{
					Shooter::on_shoot(); int t = shot_t();
					point_to(tx, ty);
					ShotData d;
					d.a = 0.04, d.v = 0.0;
					for (int i = 0; i < 8; i++)
					{
						d.v_max = 1.0 + i * 0.4;
						d.set_fan_style(1, 0.0);
						shoot_with_func<BBPrefab>(d, [](BBPrefab* b) {
							b->set_boundary(200, 200, 200, 200);
							b->set_can_be_killed_by_player(false);
							b->sprite().set_blend(BlendMode::plus);
						}, BBType::star, b3_color[c]);
						d.set_fan_style(16, pi * 1.5);
						d.rot_offset = d.rot_offset + pi;
						shoot_with_func<BBPrefab>(d, [](BBPrefab* b) {
							b->set_boundary(200, 200, 200, 200);
							b->set_can_be_killed_by_player(false);
							b->sprite().set_blend(BlendMode::plus);
						}, BBType::star, b3_color[c]);
					}
					stop();
				}
				int c; double tx, ty;
			public:
				S2(double tx, double ty, int c) :tx(tx), ty(ty), c(c) {}
			};
			void on_create() override
			{
				EB::on_create();
				for (int i = 0; i < 10; i++)
					os->create_object<ES1>(st, c, pi * 2.0 / 10.0 * i)->bind(this);
				for (int i = 0; i < 5; i++)
					os->create_object<ES2>(st, c, pi * 2.0 / 5.0 * i)->bind(this);
				rev_r = 500.0, rev_rot = start_rot, rev_omega = 0.003;
				append_shooter<S1>(c);
			}
			void on_update() override
			{
				EB::on_update(); int t = this->t(), pt = this->pt();	 if (is_killed()) return;
				if (t <= 120)
				{
					double k = t / 120.0;
					rev_dr = utils::lerp(-2.5, 0.0, k);
				}
				if (t >= 240 && t <= 285)
				{
					double k = (t - 240) / 45.0;
					rev_dr = utils::lerp(0.0, 0.4, k);
				}
				if (t >= 285 && t <= 300)
				{
					double k = (t - 285) / 15.0;
					rev_dr = utils::lerp(0.4, 0.0, k);
				}

				
				if (pt >= 1020 && pt <= 1080)
				{
					double k = (pt - 1020) / 60.0;
					rev_dr = utils::lerp(0.0, -1.8, k);
				}
				if (pt >= 1080 && pt <= 1200)
				{
					double k = (pt - 1080) / 120.0;
					rev_dr = utils::lerp(-1.8, 0.0, k);
				}
				if (pt >= 1200 && pt <= 1260)
				{
					double k = (pt - 1200) / 60.0;
					rev_omega = utils::lerp(0.003, 0.0, k);
				}

				if (pt == 1020)
					start_all_shooter();

				if (pt == 1260)
				{
					double px = player->x(), py = player->y();
					px = gs->center_w, py = gs->center_h;
					os->create_object<LineStar>(x(), y(), px, py)->sprite().set_scale(1.0, 1.0);
					clear_all_shooter();
					append_shooter<S2>(px, py, c);
				}
				if (pt == 1290)
				{
					start_all_shooter();
				}
				if (pt == 1291)
					kill(true);

			}
			int c, st; double start_rot;
		public:
			EB1(int st, int c, double start_rot) : st(st), start_rot(start_rot), c(c), EB(st, { 150.0, 200.0 }) {}
		};

		if (t == 0) player->get_death_wave()->set_damage(1);

		double a1 = -pi * 0.8;
		double a2 = a1 + 0.003 * 360 + pi * 2.0 / 3.0;
		double a3 = a2 + 0.003 * 360 + pi * 2.0 / 3.0;

		if (t >= -60 && t <= 0)
		{
			warn_circle(t, -60, gs->center_w + 350.0 * cos(a1), gs->center_h + 350.0 * sin(a1));
		}
		if (t == 0)
		{
			os->create_object<EB1>(0, 0, a1)->set_pos(-200, -200);
		}

		if (t >= 270 && t <= 330)
		{
			warn_circle(t, 270, gs->center_w + 350.0 * cos(a2), gs->center_h + 350.0 * sin(a2));
		}
		if (t == 360)
		{
			os->create_object<EB1>(360, 1, a2)->set_pos(-200, -200);
		}

		if (t >= 660 && t <= 720)
		{
			warn_circle(t, 660, gs->center_w + 350.0 * cos(a3), gs->center_h + 350.0 * sin(a3));
		}
		if (t == 720)
		{
			os->create_object<EB1>(720, 2, a3)->set_pos(-200, -200);
		}

		if (t >= 900 && t <= 960)
		{
			warn_circle(t, 900, gs->center_w, gs->center_h, 230.0);
		}

		if (t == 1080) player->get_death_wave()->set_damage(3);

		if (t == 1500)
		{
			clear_bullets(gs->center_w, gs->center_h);
		}
		t -= 1440;
	}

part11:	 // if (t == 0) bgm.bgm_1->set_time(10300);
	{
		class E1 final : public ESPrefab
		{
			class B final : public BSPrefab
			{
				void on_create() 
				{
					BSPrefab::on_create();
					set_can_be_killed_by_player(false);
					kill_particle().clear();
				}
				void on_update()
				{
					BSPrefab::on_update();
					if (!bound_obj() && flag)
					{
						set_drag(20);
						set_drag_alpha(0.1);
						set_drag_scale(0.8);
						set_ay(gs->rand_real(-0.03, -0.01));
						set_vx(gs->rand_real(-0.5, 0.5));
						flag = false;
					}
				}
				bool flag = true;
			public:
				B() : BSPrefab(BSType::star, BSColor::blue) {}
			};
			void on_create() override
			{
				ESPrefab::on_create();
				set_invincible(true, 30);
				set_max_health(30);
				set_boundary(30, 30, 30, 30);
			}
			void on_update() override
			{
				ESPrefab::on_update();	int t = this->t(); if (is_killed()) return;

				if (t == 0)
				{
					sx = tx;
					sy = -20;
				}
				if (t >= 0 && t <= 60)
				{
					move::straight(this, t, 0, 60, { sx, sy }, { tx, ty }, move::factor::ease_out);
				}
				if (t == 150)
				{
					sx = lx(-50, tx < gs->center_w);
					sy = ty;
				}
				if (t >= 150 && t <= 270)
				{
					move::straight(this, t, 150, 270, { tx, ty }, { sx, sy }, move::factor::ease_in);
				}

				if (t <= 60 && !child_list().size())
				{
					for (int i = 0; i < 5; i++)
					{
						auto b = os->create_object<B>();
						b->set_pos(0, 5);
						b->bind(this);
					}
				}
				if (t == 120)
				{
					auto l = child_list();
					for (auto b : l) 
						b->bind(nullptr);
				}
			}
			double sx, sy, tx, ty;
		public:
			E1(double tx, double ty) : tx(tx), ty(ty), ESPrefab(ESColor::blue, 0, 5) {}
		};
		class B final : public BSPrefab
		{
			void on_create()
			{
				BSPrefab::on_create();
				set_can_be_killed_by_player(false);
				set_ay(0.02);
				set_vx(gs->rand_real(-0.5, 0.5));
				set_max_v(2.5);
				set_drag(10);
				set_drag_alpha(0.1);
				set_drag_scale(0.8);
			}
		public:
			B() : BSPrefab(BSType::star, BSColor::blue) {}
		};
		class B2 final : public BSPrefab
		{
			double svx, svy;
			void on_create()
			{
				BSPrefab::on_create();
				set_can_be_killed_by_player(false);
				svx = gs->rand_real(0.0, 0.0);
				svy = gs->rand_real(2.0, 5.0);
				set_v(svx, svy);
				set_a(0.0, -0.02);
				set_min_v(0.0001);
				set_drag(20);
				set_drag_interval(5);
				set_drag_alpha(1.0);
				set_drag_scale(1.0);
			}
			void on_update()
			{
				BSPrefab::on_update();
				if (svx < 0.0 && vx() > 0.0 || svx > 0.0 && vx() < 0.0)
					set_vx(0.0), set_ax(0.0);
				if (vy() < 0.0)
					kill(false);
			}
		public:
			B2() : BSPrefab(BSType::star, BSColor::sky_blue) {}
		};

		if (t >= 60 && t <= 240 && t % 10 == 0)
		{
			os->create_object<E1>(gs->rand_int(20, gs->game_w - 20), gs->game_h * gs->rand_real(0.2, 0.4));
		}

		int interval1 = utils::lerp(1.0, 3.0, (t - 270.0) / 630.0);

		if (t >= 270 && t <= 900 && t % interval1 == 0)
		{
			os->create_object<B>()->set_pos(gs->rand_int(-20, gs->game_w + 20), -10);
		}

		int interval2 = utils::lerp(20.0, 1.0, (t - 600.0) / 900.0);

		if (t >= 600 && t <= 1500 && t % interval2 == 0)
		{
			os->create_object<B2>()->set_pos(gs->rand_int(-20, gs->game_w + 20), -10);
		}

		if (t >= 600 && t <= 1800)
		{
			bgm.bgm_1->set_vol(1.0 - (t - 600.0) / 1200.0);
		}
	}

end:
	return;

}