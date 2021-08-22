#include "Item.h"
#include "GameSystem.h"
#include "Player.h"
#include "Utils.h"
#include "DrawSystem.h"
#include "Sound.h"


Item::Item() : CollideCircle(CollideGroup::item, this)
{

}


void Item::pick(Player* player)
{
	if (!picked && !player->is_killed())
	{
		picked = true;
	}
}

bool Item::is_picked() const
{
	return picked;
}

void Item::set_pick_radius(double r)
{
	set_collider_r(r);
}

void Item::set_start_info(double v_0, int ani_time, bool ani_rotate, bool ani_scale)
{
	this->v_0 = v_0;
	this->ani_time = ani_time;
	this->ani_rotate = ani_rotate;
	this->ani_scale = ani_scale;

	set_vy(v_0);
}

void Item::set_drop_info(double v_max, double a, double buttom)
{
	this->v_max = v_max;
	this->a = a;
	this->boundary_buttom = buttom;

	set_ay(a);
}

Sprite& Item::indicator()
{
	return indicator_sprite;
}

const Sprite& Item::indicator() const
{
	return indicator_sprite;
}

Sound::Se* Item::get_pick_se() const
{
	return pick_se;
}

void Item::set_pick_se(Sound::Se* se)
{
	pick_se = se;
}



void Item::on_create()
{
	SpriteObject::on_create();

	sprite().set_level(DrawLevel::item_1);
	indicator().set_level(DrawLevel::item_2);
}

void Item::on_update()
{
	SpriteObject::on_update();

	// start ani
	if (t() <= ani_time)
	{
		double k = (double)t() / ani_time;

		if (ani_rotate)
		{
			set_rot(k * 4.0 * utils::pi);
		}
	}

	// boundary
	{
		set_x(utils::clamp<double>(x(), 0, GameSystem::game_w));
	}

	// pick
	{
		if (picked)
		{
			if (auto p = GameSystem::instance()->get_player())
			{
				if (t() >= ani_time)
				{
					set_pos_fixed(true);

					double dx = p->x() - x();
					double dy = p->y() - y();
					double rot = std::atan2(dy, dx);

					if (dx * dx + dy * dy <= 25.0)
					{
						if (pick_se)
							pick_se->play();
						on_pick(p);
						destroy();
						return;
					}

					double v = std::min(10.0, sqrt(dx * dx + dy * dy));
					set_pos(x() + v * cos(rot), y() + v * sin(rot));
				}
			}
			else
			{
				picked = false;
				set_pos_fixed(false);
			}
		}
	}

	// move
	{
		if (vy() > v_max)
		{
			set_vy(v_max);
			set_ay(0.0);
		}
	}

	// boundary
	{
		if (y() > GameSystem::game_h + boundary_buttom)
		{
			this->destroy();
		}
	}
}

void Item::on_destroy()
{

	SpriteObject::on_destroy();
}

void Item::on_draw()
{
	double ws = sprite().w_scale();
	double hs = sprite().h_scale();

	if (t() <= ani_time)
	{
		double k = (double)t() / ani_time;

		if (ani_scale)
		{
			sprite().set_scale(k * ws, k * hs);
		}
	}

	SpriteObject::on_draw();
		
	sprite().set_scale(ws, hs);


	if (y() < 0 && indicator().ready())
	{
		double alpha = indicator().alpha();
		double a = alpha * utils::clamp(-y() / 30.0, 0.0, 1.0) * 0.8;
		indicator().set_alpha(a);

		DrawSystem::instance()->draw_sprite(indicator(), x(), indicator().current_coord().height / 2);

		indicator().set_alpha(alpha);
	}
}

void Item::on_collide(PhysicObject* obj)
{
	if (!picked)
	{
		if (auto p = dynamic_cast<Player*>(obj))
		{
			pick(p);
		}
	}
}

void Item::on_pick(Player* player)
{
}