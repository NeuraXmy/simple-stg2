#include "Enemy.h"
#include "GameSystem.h"
#include "PlayerBullet.h"
#include "Sound.h"


Enemy::Enemy() : CollideCircle(CollideGroup::enemy, this)
{

}


int Enemy::get_max_health() const
{
    return max_health;
}

void Enemy::set_max_health(int hp, bool recover)
{
    max_health = hp;
    if (recover || max_health < health)
    {
        set_health(max_health);
    }
}

int Enemy::get_health() const
{
    return health;
}

void Enemy::set_health(int hp)
{
    health = hp;
}

void Enemy::damage_health(int damage)
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
            set_health(health - damage);
            on_attacked();
            last_be_attacked_time = t();
        }
    }
}

int Enemy::get_last_be_attacked_time() const
{
    return last_be_attacked_time;
}

bool Enemy::is_in_low_health() const
{
    return health <= max_health / 4;
}


bool Enemy::is_boundary_test() const
{
    return boundary_test;
}

void Enemy::set_boundary_test(bool flag)
{
    boundary_test = flag;
}

void Enemy::set_boundary(double l, double r, double t, double b)
{
    boundary_l = l;
    boundary_r = r;
    boundary_t = t;
    boundary_b = b;
}


bool Enemy::is_invincible() const
{
    return invincible;
}

void Enemy::set_invincible(bool flag, int len)
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

void Enemy::set_invincible_cancel_time(int time)
{
    invicible_cancel_time = time;
}


bool Enemy::is_can_hit_player() const
{
    return can_hit_player;
}

void Enemy::set_can_hit_player(bool flag)
{
    can_hit_player = flag;
}


void Enemy::kill(bool drop_item)
{
    if (!killed)
    {
        ParticleSystem::instance()->create_particle(kill_particle_data, this);

        killed = true; 
        if (drop_item)
        {
            on_drop_item();
        }
        if (kill_se)
            kill_se->play();

        on_kill();
        destroy();
    }
}

void Enemy::kill_raw()
{
    if (!killed)
    {
        killed = true; 
        on_kill();
        destroy();
    }
}

bool Enemy::is_killed()
{
    return killed;
}


void Enemy::set_still_frame_index(int index)
{
    set_act_frame_index((int)ActState::still, index);
}

void Enemy::set_still_from_left_frame_index(int index)
{
    set_act_frame_index((int)ActState::still_from_left, index);
}

void Enemy::set_still_from_right_frame_index(int index)
{
    set_act_frame_index((int)ActState::still_from_right, index);
}

void Enemy::set_left_move_frame_index(int index)
{
    set_act_frame_index((int)ActState::left, index);
}

void Enemy::set_right_move_frame_index(int index)
{
    set_act_frame_index((int)ActState::right, index);
}


Particle::DataSet& Enemy::kill_particle()
{
    return kill_particle_data;
}

const Particle::DataSet& Enemy::kill_particle() const
{
    return kill_particle_data;
}


void Enemy::start_all_shooter()
{
    for (auto s : shooters)
    {
        s->start();
    }
}

void Enemy::stop_all_shooter()
{
    for (auto s : shooters)
    {
        s->stop();
    }
}

void Enemy::clear_all_shooter()
{
    stop_all_shooter();
    for (auto s : shooters)
    {
        s->destroy();
    }
    shooters.clear();
}

Sound::Se* Enemy::get_kill_se() const
{
    return kill_se;
}

void Enemy::set_kill_se(Sound::Se* se)
{
    kill_se = se;
}

Sound::Se* Enemy::get_damage_se() const
{
    return damage_se;

}

void Enemy::set_damage_se(Sound::Se* se)
{
    damage_se = se;
}

Sound::Se* Enemy::get_damage_low_se() const
{
    return damage_low_se;
}

void Enemy::set_damage_low_se(Sound::Se* se)
{
    damage_low_se = se;
}


void Enemy::on_create()
{
    ActorObject::on_create();

    sprite().set_level(DrawLevel::enemy);

    for (int i = 0; i < (int)ActState::act_state_num; i++)
        create_act_state(0);
    set_act_state((int)ActState::still);

}

void Enemy::on_update()
{
    ActorObject::on_update();

    // boundary
    {
        if (boundary_test &&
              (x() < -boundary_l || x() > GameSystem::game_w + boundary_r
            || y() < -boundary_t || y() > GameSystem::game_h + boundary_b))
            kill_raw();
    }

    // invincible
    {
        if (t() == invicible_cancel_time)
        {
            invincible = false;
        }
    }

    // health
    {
        if (health <= 0)
        {
            kill();
        }
        else if (is_in_low_health())
        {
            on_low_health();
        }
    }

    // move
    {
        double dx = x() - last_x;
        double dy = y() - last_y;

        if (abs(dx) >= 1e-4 && abs(dx) > abs(dy) * 0.5)
        {
            set_act_state(dx < 0 ? (int)ActState::left : (int)ActState::right);
        }
        else
        {
            if (current_act_state() == (int)ActState::left)
                set_act_state((int)ActState::still_from_left);
            if (current_act_state() == (int)ActState::right)
                set_act_state((int)ActState::still_from_right);
        }

        last_x = x();
        last_y = y();
    }
}

void Enemy::on_destroy()
{
    clear_all_shooter();

    ActorObject::on_destroy();
}

void Enemy::on_draw()
{
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

void Enemy::on_change_act_state(int state)
{
    ActorObject::on_change_act_state(state);

}

void Enemy::on_collide(PhysicObject* obj)
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


void Enemy::on_kill()
{

}

void Enemy::on_attacked()
{
    
}

void Enemy::on_low_health()
{

}

void Enemy::on_drop_item()
{

}
