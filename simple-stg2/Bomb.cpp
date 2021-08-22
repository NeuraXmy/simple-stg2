#include "Bomb.h"
#include "Player.h"
#include "GameSystem.h"
#include "DrawSystem.h"
#include "SoundSystem.h"


Player* Bomb::get_player() const
{
	return player;
}

void Bomb::set_player(Player* player)
{
	this->player = player;
	this->bind(player);
}


std::wstring Bomb::get_title() const
{
	return title;
}

void Bomb::set_title(const std::wstring& title)
{
	this->title = title;
}

std::wstring Bomb::get_subtitle() const
{
	return subtitle;
}

void Bomb::set_subtitle(const std::wstring& subtitle)
{
	this->subtitle = subtitle;
}


int Bomb::get_len() const
{
	return len;
}

void Bomb::set_len(int len)
{
	this->len = len;
}

int Bomb::get_invincible_time() const
{
	return invincible_time;
}

void Bomb::set_invincible_time(int time)
{
	invincible_time = time;
}

int Bomb::get_pick_item_time() const
{
	return pick_item_time;
}

void Bomb::set_pick_item_time(int time)
{
	pick_item_time = time;
}


int Bomb::from_start_t() const
{
	return t() - start_time;
}

void Bomb::start()
{
	if (!casting)
	{
		start_time = t();
		casting = true;

		start_all_shooter();

		if (player)
		{
			player->set_invincible(true, invincible_time);
		}

		if (start_se)
			start_se->play();

		on_start();
	}
}

void Bomb::end()
{
	if (casting)
	{
		stop_all_shooter();
		casting = false;
		on_end();
	}
}

bool Bomb::is_casting() const
{
	return casting;
}

bool Bomb::is_ended() const
{
	return !casting;
}


void Bomb::start_all_shooter()
{
	for (auto s : shooters)
	{
		s->start();
	}
}

void Bomb::stop_all_shooter()
{
	for (auto s : shooters)
	{
		s->stop();
	}
}

void Bomb::clear_all_shooter()
{
	stop_all_shooter();
	for (auto s : shooters)
	{
		s->destroy();
	}
	shooters.clear();
}


Sprite& Bomb::title_sprite()
{
	return title_sprite_d;
}

const Sprite& Bomb::title_sprite() const
{
	return title_sprite_d;
}


Background* Bomb::background() const
{
	return bg;
}

void Bomb::remove_background()
{
	if (bg)
	{
		delete bg;
		bg = nullptr;
	}
}

Sound::Se* Bomb::get_start_se() const
{
	return start_se;
}

void Bomb::set_start_se(Sound::Se* se)
{
	start_se = se;
}


void Bomb::on_create()
{
	PhysicObject::on_create();

}

void Bomb::on_update()
{
	PhysicObject::on_update();

	if (is_casting())
	{
		on_casting();

		if (t() - start_time < pick_item_time)
		{
			GameSystem::instance()->pick_all_item();
		}

		if (t() - start_time > len)
		{
			end();
		}
	}
}

void Bomb::on_destroy()
{
	remove_background();
	clear_all_shooter();

	PhysicObject::on_destroy();
}

void Bomb::on_draw()
{
	PhysicObject::on_draw();

	if (bg)
	{
		double ani_time = len * 0.1;
		double a = utils::clamp(
			std::min(
				(t() - start_time) / ani_time, 
				(start_time + len + ani_time * 2.0 - t()) / ani_time
			), 0.0, 1.0);

		bg->set_alpha(a);
		bg->draw(from_start_t());
	}
}

void Bomb::on_draw_ui()
{
	PhysicObject::on_draw_ui();

	// title
	if(is_casting())
	{
		if (title_sprite().ready())
		{
			const double w = title_sprite().current_coord().width * title_sprite().w_scale();
			const double h = title_sprite().current_coord().height * title_sprite().h_scale();

			const double x1 = -w * 0.5;
			const double x2 = w * 0.5;
			const double y1 = GameSystem::game_h * 0.8;
			const double y2 = GameSystem::game_h + h * 0.5;

			double x = 0.0, y = 0.0;

			if (is_casting())
			{
				int t = from_start_t();
				double ani_t = 30;
				if (t <= ani_t)
				{
					double k = t / ani_t;
					x = utils::lerp(x1, x2, sqrt(k));
					y = y1;
				}
				else if (t <= ani_t * 4)
				{
					x = x2;
					y = y1;
				}
				else
				{
					double k = (t - ani_t * 4) / ani_t;
					x = x2;
					y = utils::lerp(y1, y2, k);
				}
			}

			DrawSystem::instance()->draw_sprite(title_sprite(), x, y);

			{
				TextDrawcall call;
				call.wstr = subtitle + L"¡¸" + title + L"¡¹";
				call.x = x - w * 0.5 + 5, call.y = y;
				call.anchor = TextAnchor::left_top;
				call.size = 15, call.color = Color(1, 1, 1);
				call.set_edge(Color(0.2, 0.2, 0.8), 1);
				call.level = DrawLevel::top;
				DrawSystem::instance()->draw_text(call);
			}
		}
	}
}


void Bomb::on_start()
{
}

void Bomb::on_casting()
{
}

void Bomb::on_end()
{
}
