#include "Boss.h"
#include "PlayerBullet.h"
#include "DrawSystem.h"
#include "GameSystem.h"
#include "Player.h"
#include "Utils.h"
#include "SoundSystem.h"


Boss::Boss() : CollideCircle(CollideGroup::enemy, this)
{
}


void Boss::damage_health(int damage)
{
	if (damage > 0)
	{
		if (is_in_low_health())
		{
			if (damage_low_se)
				damage_low_se->play();
		}
		else
		{
			if (damage_se)
				damage_se->play();
		}

		if (!invincible)
		{
			if (auto a = get_current_attack())
				a->damage_health(damage);
			on_attacked();
			last_be_attacked_time = t();
		}
	}
}			

int Boss::get_last_be_attacked_time() const
{
	return last_be_attacked_time;
}

bool Boss::is_in_low_health() const
{
	if (auto a = get_current_attack())
	{
		if (!a->is_casting())
		{
			return false;
		}
		return a->get_health() < a->get_max_health() / 4;
	}
	return false;
}			


bool Boss::is_invincible() const
{
	return invincible;
}

void Boss::set_invincible(bool flag, int len)
{
	if (flag && len > 0)
	{
		invincible = true;
		invicible_cancel_time = t() + len;
	}
	else
	{
		invincible = false;
		invicible_cancel_time = t() - 10;
	}
}

void Boss::set_invincible_cancel_time(int time)
{
	invicible_cancel_time = time;
}


bool Boss::is_can_hit_player() const
{
	return can_hit_player;
}

void Boss::set_can_hit_player(bool flag)
{
	can_hit_player = flag;
}


void Boss::kill()
{
	if (!killed)
	{
		killed_time = t();
		killed = true;
		set_invincible(true);
		if (kill_se)
			kill_se->play();
		on_kill();
	}
}

void Boss::kill_raw()
{
	if (!killed)
	{
		killed_time = t();
		killed = true;
		on_kill();
		destroy();
	}
}

bool Boss::is_killed()
{
	return killed;
}

Particle::DataSet& Boss::kill_particle()
{
	return kill_particle_data;
}

const Particle::DataSet& Boss::kill_particle() const
{
	return kill_particle_data;
}

Particle::DataSet& Boss::kill_ani_particle()
{
	return kill_ani_particle_data;
}

const Particle::DataSet& Boss::kill_ani_particle() const
{
	return kill_ani_particle_data;
}

int Boss::get_kill_ani_time() const
{
	return kill_ani_time;
}

void Boss::set_kill_ani_time(int time)
{
	kill_ani_time = time;
}


void Boss::charge()
{
	ParticleSystem::instance()->create_particle(charge_particle_data, this);
	if (charge_se)
		charge_se->play();
}

Particle::DataSet& Boss::charge_particle()
{
	return charge_particle_data;
}

const Particle::DataSet& Boss::charge_particle() const
{
	return charge_particle_data;
}


void Boss::set_casting(bool flag, int len)
{
	if (flag)
	{
		casting = true;
		if (len >= 0)
		{
			cast_cancel_time = t() + len;
		}
	}
	else
	{
		casting = false;
		cast_cancel_time = t() - 10;
	}
}

bool Boss::is_casting() const
{
	return casting;
}


void Boss::set_still_frame_index(int index)
{
	set_act_frame_index((int)ActState::still, index);
}

void Boss::set_still_from_left_frame_index(int index)
{
	set_act_frame_index((int)ActState::still_from_left, index);
}

void Boss::set_still_from_right_frame_index(int index)
{
	set_act_frame_index((int)ActState::still_from_right, index);
}

void Boss::set_left_move_frame_index(int index)
{
	set_act_frame_index((int)ActState::left, index);
}

void Boss::set_right_move_frame_index(int index)
{
	set_act_frame_index((int)ActState::right, index);
}

void Boss::set_casting_frame_index(int index)
{
	set_act_frame_index((int)ActState::casting, index);
}


void Boss::clear_attacks()
{
	if (auto a = get_current_attack())
		a->end(BossAttack::State::failed_timeout);
	attacks.clear();
}

BossAttack* Boss::get_current_attack() const
{
	return current_attack >= 0 && current_attack < attacks.size() ? attacks[current_attack] : nullptr;
}

std::vector<BossAttack*> Boss::get_attacks()
{
	return attacks;
}

const std::vector<BossAttack*> Boss::get_attacks() const
{
	return attacks;
}

int Boss::get_attack_count() const
{
	return attacks.size();
}

int Boss::get_sc_attack_count() const
{
	return std::count_if(attacks.begin(), attacks.end(), [](const BossAttack* a) {
		return a->is_spellcard();
	});
}

void Boss::start_attacks(int index)
{
	if (index < 0 || index >= get_attack_count())
		return;

	if (auto a = get_current_attack())
		a->end(BossAttack::State::failed_timeout);

	current_attack = index;

	attack_start_time = t();
}

bool Boss::is_kill_when_attacks_end() const
{
	return kill_when_attacks_end;
}

void Boss::set_kill_when_attacks_end(bool flag)
{
	kill_when_attacks_end = flag;
}

int Boss::get_attack_interval() const
{
	return attack_start_interval;
}

void Boss::set_attack_interval(int interval)
{
	attack_start_interval = interval;
}


Sprite& Boss::magic_circle()
{
	return magic_circle_d;
}

const Sprite& Boss::magic_circle() const
{
	return magic_circle_d;
}

void Boss::set_magic_circle(bool enable)
{
	if (enable && !magic_circle_enabled)
	{
		magic_circle_enabled = true;
		magic_circle_enabled_time = t();
	}
	else
	{
		magic_circle_enabled = false;
	}
}


double Boss::get_game_center_x() const
{
	return center_x;
}

double Boss::get_game_center_y() const
{
	return center_y;
}

void Boss::set_game_center(double x, double y)
{
	center_x = x;
	center_y = y;
}

void Boss::move_to_center(int time, move::Factor factor)
{
	move_start_time = t();
	move_end_time = t() + time;
	move_factor = factor;

	move_s_x = x();
	move_s_y = y();
	move_t_x = center_x;
	move_t_y = center_y;
}

void Boss::move_randomly(
	int time, double probability_to_player,
	utils::Range dx, utils::Range dy,
	utils::Range x_limit, utils::Range y_limit,
	move::Factor factor
)
{
	move_start_time = t();
	move_end_time = t() + time;
	move_factor = factor;

	move_s_x = x();
	move_s_y = y();

	double delta_x = dx.get_game();
	double delta_y = dy.get_game();

	double k = GameSystem::instance()->rand_real(0.0, 1.0);
	auto p = GameSystem::instance()->get_player();
	if (p && k < probability_to_player)
	{
		if (x() > p->x())
			delta_x = -delta_x;
		if (GameSystem::instance()->rand_int(0, 1))
			delta_y = -delta_y;
	}
	else
	{
		if (GameSystem::instance()->rand_int(0, 1))
			delta_x = -delta_x;
		if (GameSystem::instance()->rand_int(0, 1))
			delta_y = -delta_y;
	}

	move_t_x = utils::clamp(move_s_x + delta_x, x_limit.l, x_limit.r);
	move_t_y = utils::clamp(move_s_y + delta_y, y_limit.l, y_limit.r);
}

bool Boss::is_attack_ended() const
{
	return current_attack == attacks.size();
}


Sound::Se* Boss::get_kill_se() const
{
	return kill_se;
}

void Boss::set_kill_se(Sound::Se* se)
{
	kill_se = se;
}

Sound::Se* Boss::get_damage_se() const
{
	return damage_se;
}

void Boss::set_damage_se(Sound::Se* se)
{
	damage_se = se;
}

Sound::Se* Boss::get_damage_low_se() const
{
	return damage_low_se;
}

void Boss::set_damage_low_se(Sound::Se* se)
{
	damage_low_se = se;
}

Sound::Se* Boss::get_charge_se() const
{
	return charge_se;
}

void Boss::set_charge_se(Sound::Se* se)
{
	charge_se = se;
}


void Boss::on_create()
{
	ActorObject::on_create();

	sprite().set_level(DrawLevel::boss);
	magic_circle().set_level(DrawLevel::boss);

	for (int i = 0; i < (int)ActState::act_state_num; i++)
		create_act_state(0);
	set_act_state((int)ActState::still);

	center_x = GameSystem::game_w * 0.5;
	center_y = GameSystem::game_h * 0.3;
}

void Boss::on_update()
{
	ActorObject::on_update();

	// death
	{
		if (killed)
		{
			if (t() - killed_time >= kill_ani_time)
			{
				ParticleSystem::instance()->create_particle(kill_particle_data, this);
				destroy();
				return;
			}
			if ((t() - killed_time) % kill_particle_interval == 0)
			{
				ParticleSystem::instance()->create_particle(kill_ani_particle_data, this);
			}
		}
	}

	// invincible
	{
		if (t() == invicible_cancel_time)
		{
			invincible = false;
		}
	}

	// cast
	{
		if (t() == cast_cancel_time)
		{
			casting = false;
		}
	}

	// move
	{
		if (t() <= move_end_time)
		{
			move::straight(this, t(), move_start_time, move_end_time,
				{ move_s_x, move_s_y }, { move_t_x, move_t_y },
				move_factor);
		}
	}

	// act
	{
		double dx = x() - last_x;
		double dy = y() - last_y;

		if (abs(dx) >= 1e-4 || abs(dy) >= 1e-4)
		{
			set_act_state(dx < 0 ? (int)ActState::left : (int)ActState::right);
		}
		else
		{
			if (casting)
				set_act_state((int)ActState::casting);
			else
			{
				if (current_act_state() == (int)ActState::casting)
					set_act_state((int)ActState::still);
				if (current_act_state() == (int)ActState::left)
					set_act_state((int)ActState::still_from_left);
				if (current_act_state() == (int)ActState::right)
					set_act_state((int)ActState::still_from_right);
			}
		}

		last_x = x();
		last_y = y();
	}

	// sc
	{
		if (auto a = get_current_attack())
		{
			if (a->is_ended())
			{
				current_attack++;
				attack_start_time = t();

				if (current_attack == attacks.size())
				{
					on_attack_end();
				}
			}
		}
		if (auto a = get_current_attack())
		{
			if (a->is_spellcard() && a->is_charge_before_start() && charge_particle().size()
				&& attack_start_time + attack_start_interval - t() == charge_particle().at(0).lifeTime.get_avg())
			{
				charge();
			}

			if (!a->is_preparing() && !a->is_casting()
				&& t() >= attack_start_time + attack_start_interval)
			{
				a->start();
			}
		}
	}	
}

void Boss::on_destroy()
{
	clear_attacks();
	
	ActorObject::on_destroy();
}

void Boss::on_draw()
{
	if (magic_circle_enabled && magic_circle().ready())
	{
		double k1 = std::min(1.0, (t() - magic_circle_enabled_time) / 45.0);
		double k2 = sin(t() / 30.0);
		double ws = magic_circle().w_scale();
		double hs = magic_circle().w_scale();
		double a = magic_circle().alpha();

		magic_circle().set_scale(ws * (k2 * 0.1 + 0.9) * k1, hs * (k2 * 0.1 + 0.9) * k1);
		magic_circle().set_alpha(a * (k2 * 0.2 + 0.5) * 0.4);
		DrawSystem::instance()->draw_sprite(magic_circle(), x(), y(), t() / 20.0);
		magic_circle().set_alpha(a);
		magic_circle().set_scale(ws, hs);
	}

	double last = sprite().alpha();
	double a = last;

	if (t() - last_be_attacked_time <= 4)
		a *= (t() / 2) & 1 ? 1.0 : 0.5;
	if (is_in_low_health())
		a *= (t() / 8) & 1 ? 1.0 : 0.5;
	sprite().set_alpha(a);

	ActorObject::on_draw();

	sprite().set_alpha(last);
}

void Boss::on_change_act_state(int state)
{
	ActorObject::on_change_act_state(state);

}

void Boss::on_collide(PhysicObject* obj)
{
	CollideCircle::on_collide(obj);

	if (auto b = dynamic_cast<PlayerBullet*>(obj))
	{
		damage_health(b->get_damage());
		if (auto p = GameSystem::instance()->get_player())
		{
			p->add_score(b->get_hit_score());
		}
	}
}


void Boss::on_kill()
{
	
}

void Boss::on_attacked()
{

}

void Boss::on_low_health()
{
}

void Boss::on_attack_end()
{
}
