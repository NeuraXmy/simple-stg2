#include "BossAttack.h"
#include "Boss.h"
#include "GameSystem.h"
#include "DrawSystem.h"
#include "Utils.h"
#include "ClearBulletWave.h"
#include "SoundSystem.h"

using utils::pi;


void BossAttack::set_boss(Boss* boss)
{
	this->boss = boss;
}

Boss* BossAttack::get_boss() const
{
	return boss;
}


std::wstring BossAttack::get_title() const
{
	return title;
}

void BossAttack::set_title(const std::wstring& title)
{
	this->title = title;
}

std::wstring BossAttack::get_subtitle() const
{
	return subtitle;
}

void BossAttack::set_subtitle(const std::wstring& subtitle)
{
	this->subtitle = subtitle;
}


bool BossAttack::is_spellcard() const
{
	return is_sc;
}

void BossAttack::set_is_spellcard(bool flag)
{
	is_sc = flag;
}


bool BossAttack::is_time_spellcard() const
{
	return is_time_sc;
}

void BossAttack::set_is_time_spellcard(bool flag)
{
	is_time_sc = flag;
}


int BossAttack::get_time_limit() const
{
	return time_limit;
}

void BossAttack::set_time_limit(int time)
{
	time_limit = time;
}

int BossAttack::get_from_start_time() const
{
	return t() - start_time;
}


int BossAttack::get_max_health() const
{
	return max_health;
}

void BossAttack::set_max_health(int hp, bool recover)
{
	max_health = hp;
	if (recover || health > max_health)
	{
		health = max_health;
		display_health = max_health;
	}
}

int BossAttack::get_health() const
{
	return health;
}

void BossAttack::set_health(int hp)
{
	health = hp;
}

void BossAttack::damage_health(int damage)
{
	if (!is_time_sc && damage > 0 && is_casting())
	{
		set_health(health - damage);
		last_be_attacked_time = t();
		on_attacked();
	}
}

int BossAttack::get_last_be_attacked_time() const
{
	return last_be_attacked_time;
}


int BossAttack::get_max_score() const
{
	return max_score;
}

void BossAttack::set_max_score(int score, bool recover)
{
	max_score = score;

	if (recover || this->score > max_score)
	{
		this->score = max_score;
	}
}

int BossAttack::get_score() const
{
	return score;
}

void BossAttack::set_score(int score)
{
	this->score = score;
}


bool BossAttack::is_preparing() const
{
	return state == State::preparing;
}

int BossAttack::get_prepare_time() const
{
	return prepare_time;
}

void BossAttack::set_prepare_time(int time)
{
	prepare_time = time;
}


void BossAttack::start()
{
	if (state != State::casting && state != State::preparing)
	{
		start_time = t() + prepare_time;
		state = State::preparing;
		score = max_score;
		health = 1;
		player_killed = false;
		player_bombed = false;
		boss->set_invincible(true, prepare_time);
		GameSystem::instance()->set_current_boss_attack(this);
		on_prepare();

		if (start_se)
			start_se->play();
	}
}

void BossAttack::end(State state)
{
	if (this->state == State::casting && 
	    (state == State::beaten ||
		state == State::failed_lost ||
		state == State::failed_timeout))
	{
		end_time = t();
		this->state = state;
		stop_all_shooter();
		on_end();

		switch (state)
		{
		case BossAttack::State::beaten:
			on_beaten();				break;
		case BossAttack::State::failed_timeout:
			on_failed_timeout();		break;
		case BossAttack::State::failed_lost:
			on_failed_lost();			break;
		default:
			break;
		}

		if (state == BossAttack::State::beaten)
		{
			if (auto p = GameSystem::instance()->get_player())
			{
				p->add_score(score);
			}
		}
		
		if (beaten_se)
			beaten_se->play();

		if (kill_bullet_when_end)
		{
			auto w = ObjectSystem::instance()->create_object<ClearBulletWave>();
			w->set_bullet_drop(true);
			w->set_damage(0);
			w->set_pos(this);
			w->set_initial_expand_v(15.0);
			w->set_max_radius(1200.0);
			w->start_expand();
		}

		on_drop_item();

		GameSystem::instance()->set_current_boss_attack(nullptr);
	}
}

BossAttack::State BossAttack::get_state() const
{
	return state;
}

bool BossAttack::is_casting() const
{
	return state == State::casting;
}

bool BossAttack::is_ended() const
{
	return state == State::beaten ||
		state == State::failed_lost ||
		state == State::failed_timeout;
}

int BossAttack::get_start_time() const
{
	return start_time;
}

int BossAttack::get_end_time() const
{
	return end_time;
}


void BossAttack::set_player_bombed(bool flag)
{
	player_bombed = flag;
}

bool BossAttack::is_player_bombed() const
{
	return player_bombed;
}

void BossAttack::set_player_killed(bool flag)
{
	player_killed = flag;
}

bool BossAttack::is_player_killed() const
{
	return player_killed;
}


void BossAttack::start_all_shooter()
{
	for (auto s : shooters)
	{
		s->start();
	}
}

void BossAttack::stop_all_shooter()
{
	for (auto s : shooters)
	{
		s->stop();
	}
}

void BossAttack::clear_all_shooter()
{
	stop_all_shooter();
	for (auto s : shooters)
	{
		s->destroy();
	}
	shooters.clear();
}


bool BossAttack::is_kill_bullet_when_end() const
{
	return kill_bullet_when_end;
}

void BossAttack::set_kill_bullet_when_end(bool flag)
{
	kill_bullet_when_end = flag;
}


Sprite& BossAttack::title_sprite()
{
	return title_sprite_d;
}

const Sprite& BossAttack::title_sprite() const
{
	return title_sprite_d;
}

Sprite& BossAttack::spellcard_stripe()
{
	return spellcard_stripe_d;
}

const Sprite& BossAttack::spellcard_stripe() const
{
	return spellcard_stripe_d;
}


bool BossAttack::is_charge_before_start() const
{
	return charge_before_start;
}

void BossAttack::set_charge_before_start(bool flag)
{
	charge_before_start = flag;
}


Background* BossAttack::background() const
{
	return bg;
}

void BossAttack::remove_background()
{
	if (bg)
	{
		delete bg;
		bg = nullptr;
	}
}

Sound::Se* BossAttack::get_start_se() const
{
	return start_se;
}

void BossAttack::set_start_se(Sound::Se* se)
{
	start_se = se;
}

Sound::Se* BossAttack::get_beaten_se() const
{
	return beaten_se;
}

void BossAttack::set_beaten_se(Sound::Se* se)
{
	beaten_se = se;
}

Sound::Se* BossAttack::get_failed_se() const
{
	return failed_se;
}

void BossAttack::set_failed_se(Sound::Se* se)
{
	failed_se = se;
}

Sound::Se* BossAttack::get_timeout_se() const
{
	return timeout_se;
}

void BossAttack::set_timeout_se(Sound::Se* se)
{
	timeout_se = se;
}


void BossAttack::on_create()
{
	PhysicObject::on_create();

	title_sprite().set_level(DrawLevel::top);
	spellcard_stripe().set_level(DrawLevel::background_2);
	spellcard_stripe().set_repeated(true);
}

void BossAttack::on_update()
{
	PhysicObject::on_update();

	if (bg)
	{
		bg->update(get_from_start_time());
	}

	if (is_preparing())
	{
		if (t() < start_time)
		{
			double k = 5.0 / prepare_time;
			health += std::max<double>(utils::lerp(health, max_health, k) - health, std::ceil(max_health / prepare_time));
			if (health > max_health)
				health = max_health;
			display_health = health;
		}
		if (t() >= start_time)
		{
			health = display_health = max_health;
			state = State::casting;
			start_all_shooter();
			on_start();
		}
	}

	if (is_casting())
	{
		on_casting();

		// score
		if (player_bombed || player_killed)
		{
			score = 0;
		}
		else
		{
			if(!is_time_sc && is_sc)
				score = max_score * (1.0 - (double)get_from_start_time() / time_limit);
		}

		// timeout
		int remain_t = time_limit - get_from_start_time();
		if (remain_t <= 300)
		{
			if ((remain_t % 60 == 0 || remain_t % 60 == 45) && timeout_se)
				timeout_se->play();
		}
		else if (remain_t <= 600)
		{
			if (remain_t % 60 == 0 && timeout_se)
				timeout_se->play();
		}

		// end
		if (is_time_sc)
		{
			if (get_from_start_time() > time_limit)
			{
				if (player_bombed || player_killed)
					end(State::failed_lost);
				else
					end(State::beaten);
			}
		}
		else
		{
			if (health <= 0)
			{
				if (player_bombed || player_killed)
					end(State::failed_lost);
				else
					end(State::beaten);
			}
			else if (get_from_start_time() > time_limit)
			{
				if (player_bombed || player_killed)
					end(State::failed_lost);
				else
					end(State::failed_timeout);
			}

		}
	}
}

void BossAttack::on_destroy()
{
	end(State::failed_timeout);

	remove_background();
	clear_all_shooter();

	PhysicObject::on_destroy();
}

void BossAttack::on_draw()
{
	PhysicObject::on_draw();

	if (bg)
	{
		if (is_preparing() || is_casting())
		{
			int t = prepare_time - (start_time - this->t());
			double a = std::min(1.0, (double)t / prepare_time);
			bg->set_alpha(a);
			bg->draw(this->t());
		}
		else if(t() - end_time < 15)
		{
			int t = this->t() - end_time;
			double a = std::max(0.0, 1.0 - double(t) / 15.0);
			bg->set_alpha(a);
			bg->draw(this->t());
		}
	}

	// stripe
	{
		if (is_preparing() && is_spellcard() && spellcard_stripe().ready())
		{
			double a = 1.0 * (start_time - t()) / prepare_time;
			double x = GameSystem::game_w * 0.5;
			double y = GameSystem::game_h;

			spellcard_stripe().set_alpha(std::min(1.0, a * 2.0) * 0.6);
			spellcard_stripe().set_repeated(true);

			DrawSystem::instance()->draw_sprite(spellcard_stripe(),
				x - a * 100.0, y * 0.1 + a * 100.0, -pi * 0.25);
			DrawSystem::instance()->draw_sprite(spellcard_stripe(),
				x + a * 100.0, y * 0.2 - a * 100.0, -pi * 0.25);
			DrawSystem::instance()->draw_sprite(spellcard_stripe(),
				x - a * 100.0, y * 0.3 + a * 100.0, -pi * 0.25);

			DrawSystem::instance()->draw_sprite(spellcard_stripe(),
				x + a * 100.0, y * 0.7 - a * 100.0, -pi * 0.25);
			DrawSystem::instance()->draw_sprite(spellcard_stripe(),
				x - a * 100.0, y * 0.8 + a * 100.0, -pi * 0.25);
			DrawSystem::instance()->draw_sprite(spellcard_stripe(),
				x + a * 100.0, y * 0.9 - a * 100.0, -pi * 0.25);
		}
	}	
}

void BossAttack::on_draw_ui()
{
	PhysicObject::on_draw_ui();

	// clear info
	{
		if (is_ended() && is_spellcard())
		{
			int t = this->t() - end_time;

			double a = 0.8;
			if (t > 120)
				a = std::max(0.0, 1.0 - (t - 120) / 120.0) * 0.8;

			if (state == State::beaten)
			{
				double start = GameSystem::game_h * 0.3;
				double flash = ((this->t() / 2) & 1) ? 1.0 : 0.6;

				TextDrawcall call;
				call.str = "Get Spellcard Bonus!";
				call.x = GameSystem::game_w * 0.5;
				call.y = start;
				call.anchor = TextAnchor::center;
				call.size = 20;
				call.color = Color(1, 1, 1, a * flash);
				call.set_edge(Color(0, 0, 0, a * flash), 1);
				call.level = DrawLevel::top;
				DrawSystem::instance()->draw_text(call);

				int len = (end_time - start_time) / 60.0 * 100.0;
				call.str = "Time: " + std::to_string(len / 100)
					+ "." + std::to_string(len / 10 % 10) + std::to_string(len % 10);
				call.y = start + 25;
				call.size = 15;
				call.color = Color(1, 1, 1, a);
				call.set_edge(Color(0, 0, 0, a), 1);
				DrawSystem::instance()->draw_text(call);

				call.str = "Score: " + std::to_string(score);
				call.y = start + 45;
				call.size = 15;
				call.color = Color(1, 1, 1, a);
				call.set_edge(Color(0, 0, 0, a), 1);
				DrawSystem::instance()->draw_text(call);
			}
			else
			{
				double start = GameSystem::game_h * 0.3;

				TextDrawcall call;
				call.str = "Bonus Failed";
				call.x = GameSystem::game_w * 0.5;
				call.y = start;
				call.anchor = TextAnchor::center;
				call.size = 16;
				call.color = Color(1, 1, 1, a);
				call.set_edge(Color(0, 0, 0, a), 1);
				call.level = DrawLevel::top;
				DrawSystem::instance()->draw_text(call);

				int len = (end_time - start_time) / 60.0 * 100.0;
				call.str = "Time: " + std::to_string(len / 100)
					+ "." + std::to_string(len / 10 % 10) + std::to_string(len % 10);
				call.str = "Time: " + std::to_string(len / 100) + "." + std::to_string(len % 100);
				call.y = start + 25;
				call.size = 15;
				DrawSystem::instance()->draw_text(call);
			}
		}
	}

	// title
	if (is_spellcard())
	{
		if (title_sprite().ready())
		{
			const double w = title_sprite().current_coord().width * title_sprite().w_scale();
			const double h = title_sprite().current_coord().height * title_sprite().h_scale();

			const double x1 = GameSystem::game_w + w * 0.5;
			const double x2 = GameSystem::game_w - w * 0.5;
			const double y1 = GameSystem::game_h * 0.7;
			const double y2 = GameSystem::game_h + h * 0.5;
			const double y3 = -h * 0.5;
			const double y4 = 35;

			double x = 0.0, y = 0.0;
			double a = 0.0;

			if (is_preparing() || is_casting())
			{
				a = 1.0;
				int t = prepare_time - (start_time - this->t());
				double ani_t = prepare_time * 0.2;
				if (t <= ani_t)
				{
					double k = t / ani_t;
					x = utils::lerp(x1, x2, sqrt(k));
					y = y1;
				}
				else if (t <= ani_t * 2)
				{
					x = x2;
					y = y1;
				}
				else if (t <= ani_t * 3)
				{
					double k = (t - ani_t * 2) / ani_t;
					x = x2;
					y = utils::lerp(y1, y2, k);
				}
				else if (t <= ani_t * 4)
				{
					double k = (t - ani_t * 3) / ani_t;
					x = x2;
					y = utils::lerp(y3, y4, sqrt(k));
				}
				else
				{
					a = 1.0;
					x = x2, y = y4;
				}
			}
			else if (is_ended())
			{
				a = std::max(0.0, (30 - (t() - end_time)) / 30.0);
				x = x2, y = y4;
			}

			title_sprite().set_alpha(a);
			DrawSystem::instance()->draw_sprite(title_sprite(), x, y);

			// title
			{
				TextDrawcall call;
				call.wstr = subtitle + L"¡¸" + title + L"¡¹";
				call.x = x + w * 0.5 - 5, call.y = y;
				call.anchor = TextAnchor::right_top;
				call.size = 15, call.color = Color(1, 1, 1, a);
				call.set_edge(Color(0.8, 0.2, 0.2, a), 1);
				call.level = DrawLevel::top;
				DrawSystem::instance()->draw_text(call);
			}

			// score
			{
				TextDrawcall call;
				call.str = "Bonus " + (is_player_bombed() || is_player_killed() ? "Failed" : std::to_string(score));
				call.x = x + w * 0.5 - 8, call.y = y + 20;
				call.anchor = TextAnchor::right_top;
				call.size = 10, call.color = Color(1, 1, 1, a);
				call.set_edge(Color(0.4, 0.0, 0.0, a), 1);
				call.level = DrawLevel::top;
				DrawSystem::instance()->draw_text(call);
			}
		}
	}

	// time
	{
		if (is_preparing() || is_casting())
		{
			int sec = std::min<int>(9999, std::min(time_limit, time_limit - get_from_start_time()) / 60.0 * 100.0);

			Color c = Color(0.8, 0.6, 0.0);
			int e = 1;

			if (sec <= 1000 && ((t() / 30) & 1))
				c = Color(1.0, 0.2, 0.2), e = 2;

			TextDrawcall call;
			call.str = std::to_string(sec / 1000 % 10) + std::to_string(sec / 100 % 10);
			call.x = 5, call.y = 2;
			call.anchor = TextAnchor::left_top;
			call.size = 30, call.color = Color(1, 1, 1);
			call.set_edge(c, e);
			call.level = DrawLevel::top;
			DrawSystem::instance()->draw_text(call);

			call.str = "." + std::to_string(sec / 10 % 10) + std::to_string(sec % 10);
			call.x = 42, call.y = 4;
			call.anchor = TextAnchor::left_top;
			call.size = 20, call.color = Color(1, 1, 1);
			call.set_edge(c, e);
			call.level = DrawLevel::top;
			DrawSystem::instance()->draw_text(call);
		}
	}

	// health
	{
		if (is_preparing() || is_casting())
		{
			double k1 = 1.0, k2 = 1.0;
			Color c = Color();

			if (is_sc && is_time_sc)
			{
				k2 = k1 = 1.0 - (double)get_from_start_time() / time_limit;
				c = Color(0.4, 0.8, 1.0);
			}
			else
			{
				k1 = (double)health / max_health;
				k2 = (double)display_health / max_health;
				display_health = utils::lerp(display_health, health, 0.01);
				c = is_sc ? Color(0.2, 1.0, 0.2) : Color(0.9, 0.9, 0.9);
			}

			k1 = utils::clamp(k1, 0.0, 1.0);
			k2 = utils::clamp(k2, 0.0, 1.0);

			DrawSystem::instance()->draw_func([k1, k2, c](sf::RenderTarget* target) {

				int len = GameSystem::game_w - 100;
				int x = 90;
				int y = 15;

				sf::RectangleShape hp;
				hp.setSize(sf::Vector2f(len * k2, 10));
				hp.setPosition(sf::Vector2f(x, y));
				hp.setFillColor(Color(0.8, 0.1, 0.1));
				target->draw(hp);

				hp.setSize(sf::Vector2f(len * k1, 10));
				hp.setFillColor(c);
				target->draw(hp);

				sf::RectangleShape border;
				border.setSize(sf::Vector2f(len, 10));
				border.setPosition(sf::Vector2f(x, y));
				border.setFillColor(Color(0, 0, 0, 0));
				border.setOutlineColor(Color(0.8, 0.6, 0.0, 1.0));
				border.setOutlineThickness(2);
				target->draw(border);

				}, DrawLevel::top);
		}
	}

}


void BossAttack::on_attacked()
{

}

void BossAttack::on_prepare()
{
}

void BossAttack::on_start()
{
}

void BossAttack::on_casting()
{
}

void BossAttack::on_end()
{
}

void BossAttack::on_beaten()
{
}

void BossAttack::on_failed_lost()
{
}

void BossAttack::on_failed_timeout()
{
}

void BossAttack::on_drop_item()
{
	
}
