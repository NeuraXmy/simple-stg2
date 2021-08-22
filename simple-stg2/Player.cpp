#include <cmath>

#include "Player.h"
#include "GameSystem.h"
#include "DrawSystem.h"
#include "ObjectSystem.h"
#include "CollideCircle.h"
#include "Utils.h"
#include "Sound.h"

#include "EnemyBullet.h"
#include "DeathWave.h"

#include "Enemy.h"
#include "Boss.h"
#include "Laser.h"


static const double sqrthalf = std::sqrt(0.5);


Player::Player() : CollideCircle(CollideGroup::player, this)
{

}


void Player::HitPoint::show()
{
    if (!sprite().visible())
    {
        sprite().set_visible(true);
        start_show_time = t();
    }
}

void Player::HitPoint::hide()
{
    if (sprite().visible())
    {
        sprite().set_visible(false);
    }
}

void Player::HitPoint::on_create()
{
    SpriteObject::on_create();
    sprite().set_level(DrawLevel::hit_point);
    set_omega(rotating_speed);
}

void Player::HitPoint::on_update()
{
    SpriteObject::on_update();
    double k = std::min(1.0, (t() - start_show_time) / (double)ani_time);
    sprite().set_alpha(k);
    sprite().set_scale(1.0 + (1.0 - k) * scale_k, 1.0 + (1.0 - k) * scale_k);
}

void Player::HitPoint::on_draw()
{
    SpriteObject::on_draw();
    DrawSystem::instance()->draw_sprite(sprite(), x(), y(), -rot());
}



Player::GrazeCircle::GrazeCircle(Player* player) :
    CollideCircle(CollideGroup::player, this), p(player)
{
}

void Player::GrazeCircle::on_collide(PhysicObject* obj)
{
    CollideCircle::on_collide(obj);

    if (auto b = dynamic_cast<EnemyBullet*>(obj))
    {
        p->graze_bullet(b);
    }
    if (auto l = dynamic_cast<Laser*>(obj))
    {
        p->graze_laser(l);
    }
}



void Player::set_boundary(double l, double r, double t, double b)
{
    boundary_l = l;
    boundary_r = r;
    boundary_t = t;
    boundary_b = b;
}


bool Player::is_slowmode()
{
    return slowmode;
}

void Player::set_slowmode(bool flag)
{
    if (flag ^ slowmode)
    {
        slowmode = flag;
    }
}


double Player::get_speed() const
{
    return speed;
}

void Player::set_speed(double speed)
{
    this->speed = speed;
}

double Player::get_slowmode_speed() const
{
    return slowmode_speed;
}

void Player::set_slowmode_speed(double speed)
{
    this->slowmode_speed = speed;
}


bool Player::is_invincible() const
{
    return invincible;
}

void Player::set_invincible(bool flag, int len)
{
    if (flag && len > 0)
    {
        invincible = true;
        invicible_cancel_time = std::max(invicible_cancel_time, t() + len);
    }
    else
    {
        invincible = false;
        invicible_cancel_time = t() - 10;
    }
}

void Player::set_invincible_cancel_time(int time)
{
    invicible_cancel_time = time;
}


bool Player::is_lock_move() const
{
    return lock_move;
}

void Player::set_lock_move(bool flag)
{
    lock_move = flag;
}

bool Player::is_lock_attack() const
{
    return lock_attack;
}

void Player::set_lock_attack(bool flag)
{
    lock_attack = flag;
}

bool Player::is_lock_bomb() const
{
    return lock_bomb;
}

void Player::set_lock_bomb(bool flag)
{
    lock_bomb = flag;
}


void Player::kill()
{
    if (!killed && !invincible && !cheat_mode)
    {
        killed = true;
        last_killed_time = t();
    }
}

void Player::kill_raw()
{
    if (!killed)
    {
        killed = true;
        last_killed_time = t();
    }
}

void Player::rescue()
{
    if (killed)
    {
        killed = false;
        on_rescue();
    }
}

bool Player::is_killed() const
{
    return killed;
}

int Player::get_kill_bomb_time() const
{
    return killed_bomb_time;
}

void Player::set_kill_bomb_time(int time)
{
    killed_bomb_time = time;
}

bool Player::is_in_kill_bomb_time() const
{
    return is_killed() && t() - last_killed_time <= killed_bomb_time;
}


void Player::set_still_frame_index(int index)
{
    set_act_frame_index((int)ActState::still, index);
}

void Player::set_still_from_left_frame_index(int index)
{
    set_act_frame_index((int)ActState::still_from_left, index);
}

void Player::set_still_from_right_frame_index(int index)
{
    set_act_frame_index((int)ActState::still_from_right, index);
}

void Player::set_left_move_frame_index(int index)
{
    set_act_frame_index((int)ActState::left, index);
}

void Player::set_right_move_frame_index(int index)
{
    set_act_frame_index((int)ActState::right, index);
}


DeathWave* Player::get_death_wave() const
{
    return death_wave;
}

void Player::set_death_wave(DeathWave* wave)
{
    if (death_wave)
    {
        death_wave->destroy();
        death_wave = nullptr;
    }

    death_wave = wave;
    death_wave->set_enabled(false);
    death_wave->set_visible(false);
}


Player::HitPoint* Player::hit_point() const
{
    return hit_point_obj;
}


void Player::remove_main_gun()
{
    if (main_gun_obj)
    {
        main_gun_obj->destroy();
        main_gun_obj = nullptr;
    }
}

Shooter* Player::main_gun() const
{
    return main_gun_obj;
}


void Player::remove_drone_set_fast()
{
    if (drone_set_fast_obj)
    {
        drone_set_fast_obj->destroy();
        drone_set_fast_obj = nullptr;
    }
}

DroneSet* Player::drone_set_fast() const
{
    return drone_set_fast_obj;
}


void Player::remove_drone_set_slow()
{
    if (drone_set_slow_obj)
    {
        drone_set_slow_obj->destroy();
        drone_set_slow_obj = nullptr;
    }
}

DroneSet* Player::drone_set_slow() const
{
    return drone_set_slow_obj;
}


int Player::get_power() const
{
    return power;
}

void Player::set_power(int power)
{
    int last = this->power;

    this->power = utils::clamp(power, 0, max_power);

    if (last != this->power)
    {
        if (this->power == max_power && full_power_se)
            full_power_se->play();

        on_change_power();
        if (this->power == max_power)
        {
            last_full_power_time = t();
        }
    }
}

void Player::add_power(int power)
{
    if (this->power / power_up_interval < (this->power + power) / power_up_interval && power_up_se)
        power_up_se->play();
    set_power(this->power + power);
}

int Player::get_max_power() const
{
    return max_power;
}

void Player::set_max_power(int power)
{
    max_power = power;
    if (this->power > max_power)
        set_power(max_power);
}

bool Player::is_attacking() const
{
    return attacking && !lock_attack;
}

bool Player::is_full_power() const
{
    return power == max_power;
}


int Player::get_life_count() const
{
    return life_count;
}

void Player::set_life_count(int life)
{
    life_count = life;
}

void Player::extend_life()
{
    if (life_count < life_limit)
    {
        life_count++;
        last_extend_life_time = t();
        if (extend_life_se)
            extend_life_se->play();
    }
}

int Player::get_life_limit() const
{
    return life_limit;
}

void Player::set_life_limit(int limit)
{
    life_limit = limit;
}


int Player::get_bomb_count() const
{
    return bomb_count;
}

void Player::set_bomb_count(int bomb)
{
    bomb_count = bomb;
}

void Player::extend_bomb()
{
    if (bomb_count < bomb_limit)
    {
        bomb_count++;
        last_extend_bomb_time = t();
        if (extend_bomb_se)
            extend_bomb_se->play();
    }
}

int Player::get_bomb_limit() const
{
    return bomb_limit;
}

void Player::set_bomb_limit(int limit)
{
    bomb_limit = limit;
}

int Player::get_bomb_initial_count() const
{
    return bomb_initial_count;
}

void Player::set_bomb_initial_count(int bomb)
{
    bomb_initial_count = bomb;
}

void Player::bomb()
{
    if ((!cheat_mode && bomb_count <= 0) || is_bombing() || lock_bomb)
    {
        return;
    }

    if(!cheat_mode)
        bomb_count--;
   
    if (auto a = GameSystem::instance()->get_current_boss_attack())
    {
        a->set_player_bombed(true);
    }

    if (slowmode)
    {
        if (slow_bomb)
        {
            slow_bomb->start();
        }
    }
    else
    {
        if (fast_bomb)
        {
            fast_bomb->start();
        }
    }
        
    on_bomb();
}

long long Player::get_score() const
{
    return score;
}

void Player::set_score(long long score)
{
    this->score = score;
}

void Player::add_score(long long score)
{
    set_score(this->score + score);
}

int Player::get_point_score() const
{
    return point_score;
}

void Player::set_point_score(int score)
{
    point_score = score;
}

void Player::add_point_score(int score)
{
    point_score = std::min(point_score + score, point_score_limit);
}

int Player::get_point_score_limit() const
{
    return point_score_limit;
}

void Player::set_point_score_limit(int limit)
{
    point_score_limit = limit;
}

int Player::get_graze() const
{
    return graze;
}

void Player::set_graze(int graze)
{
    this->graze = graze;
}

void Player::graze_bullet(EnemyBullet* b)
{
    if (!b->is_grazed())
    {
        b->set_grazed(true);
        graze++;
        ParticleSystem::instance()->create_particle(graze_particle_data, this);
        on_graze(b);

        if (graze_se)
            graze_se->play();
    }
}

void Player::graze_laser(Laser* l)
{
    if (!l->is_grazed())
    {
        l->graze();
        graze++;
        ParticleSystem::instance()->create_particle(graze_particle_data, this);
        on_graze(l);

        if (graze_se)
            graze_se->play();
    }
}

void Player::set_graze_circle_r(double r)
{
    graze_circle_r = r;
    graze_circle->set_collider_r(r);
}

double Player::get_graze_circle_r() const
{
    return graze_circle_r;
}

double Player::get_item_pick_line() const
{
    return item_pick_line;
}

void Player::set_item_pick_line(double y)
{
    item_pick_line = y;
}

void Player::show_item_pick_line()
{
    last_show_item_pick_line_time = t();
}

Particle::DataSet& Player::graze_particle()
{
    return graze_particle_data;
}

const Particle::DataSet& Player::graze_particle() const
{
    return graze_particle_data;
}

Particle::DataSet& Player::death_particle()
{
    return death_particle_data;
}

const Particle::DataSet& Player::death_particle() const
{
    return death_particle_data;
}

bool Player::is_cheat_mode() const
{
    return cheat_mode;
}

void Player::set_cheat_mode(bool flag)
{
    cheat_mode = flag;
}

Sound::Se* Player::get_graze_se() const
{
    return graze_se;
}

void Player::set_graze_se(Sound::Se* se)
{
    graze_se = se;
}

Sound::Se* Player::get_death_se() const
{
    return death_se;
}

void Player::set_death_se(Sound::Se* se)
{
    death_se = se;
}

Sound::Se* Player::get_extend_life_se() const
{
    return extend_life_se;
}

void Player::set_extend_life_se(Sound::Se* se)
{
    extend_life_se = se;
}

Sound::Se* Player::get_extend_bomb_se() const
{
    return extend_bomb_se;
}

void Player::set_extend_bomb_se(Sound::Se* se)
{
    extend_bomb_se = se;
}

Sound::Se* Player::get_power_up_se() const
{
    return power_up_se;
}

void Player::set_power_up_se(Sound::Se* se)
{
    power_up_se = se;
}

void Player::set_power_up_interval(int interval)
{
    power_up_interval = interval;
}

Sound::Se* Player::get_full_power_se() const
{
    return full_power_se;
}

void Player::set_full_power_se(Sound::Se* se)
{
    full_power_se = se;
}


Bomb* Player::get_fast_bomb() const
{
    return fast_bomb;
}

void Player::remove_fast_bomb() 
{
    if (fast_bomb)
    {
        fast_bomb->end();
        fast_bomb->set_player(nullptr);
        fast_bomb->destroy();
        fast_bomb = nullptr;
    }
}

Bomb* Player::get_slow_bomb() const
{
    return slow_bomb;
}

void Player::remove_slow_bomb()
{
    if (slow_bomb)
    {
        slow_bomb->end();
        slow_bomb->set_player(nullptr);
        slow_bomb->destroy();
        slow_bomb = nullptr;
    }
}

bool Player::is_bombing() const
{
    if (slow_bomb && slow_bomb->is_casting())
        return true;
    if (fast_bomb && fast_bomb->is_casting())
        return true;
    return false;
}

void Player::on_create()
{
    ActorObject::on_create();

    sprite().set_level(DrawLevel::player);

    for (int i = 0; i < (int)ActState::act_state_num; i++)
        create_act_state(0);
    set_act_state((int)ActState::still);

    hit_point_obj = ObjectSystem::instance()->create_object<HitPoint>();
    hit_point_obj->bind(this);

    set_collider_r(2.5);

    auto wave = ObjectSystem::instance()->create_object<DeathWave>();
    wave->set_enabled(false);
    wave->set_visible(false);
    set_death_wave(wave);

    graze_circle = ObjectSystem::instance()->create_object<GrazeCircle>(this);
    graze_circle->bind(this);
    graze_circle->set_collider_r(graze_circle_r);
}

void Player::on_update()
{
    ActorObject::on_update();

    // death
    {
        if (is_killed())
        {
            int time = t() - last_killed_time;

            // bomb rescuing time
            if (time < killed_bomb_time)
            {
                if (time == 0)
                {
                    if(death_se)
                        death_se->play();
                    ParticleSystem::instance()->create_particle(death_particle_data, this);
                }

                set_lock_move(true);
                set_lock_attack(true);
                sprite().set_visible(false);
                set_collide(false);

                if (is_bombing())
                {
                    rescue();
                    set_collide(true);
                    set_lock_move(false);
                    set_lock_attack(false);
                    sprite().set_visible(true);
                }
                if(time == killed_bomb_time - 1)
                    set_lock_bomb(true);
            }
            // kill
            else if (time == killed_bomb_time)
            {
                life_count--;
                if (life_count == -1)
                {
                    // ...
                }
                bomb_count = 3;

                on_kill();
                set_lock_bomb(true);
                death_wave->set_pos(this);
                death_wave->start_expand();
                set_pos(GameSystem::game_w * 0.5, GameSystem::game_h * 1.45);
                set_invincible(true);
                sprite().set_visible(true);
                set_collide(true);

                if (auto a = GameSystem::instance()->get_current_boss_attack())
                    a->set_player_killed(true);
            }
            // move from buttom
            else if (time < killed_bomb_time + killed_ani_time)
            {
                double factor = double(time - killed_bomb_time + 1) / killed_ani_time;
                set_pos(GameSystem::game_w * 0.5, GameSystem::game_h * (1.45 - 0.6 * factor));
            }
            // respawn
            else if (time == killed_bomb_time + killed_ani_time)
            {
                set_lock_move(false);
                set_lock_attack(false);
                set_lock_bomb(false);
                set_invincible(true, killed_invincible_time);
                killed = false;
            }
        }
    }

    // invincible
    {
        if (t() == invicible_cancel_time)
        {
            invincible = false;
        }

        if (invincible)
        {
            sprite().set_alpha(t() % 4 < 2 ? 1.0 : 0.0);
        }
        else
        {
            sprite().set_alpha(1.0);
        }
    }

    // fire
    {
        is_attacking() ? on_attack() : on_silent();

        if (auto m = main_gun())
        {
            is_attacking() ? m->start() : m->stop();
        }

        if (auto f = drone_set_fast())
        {
            if (slowmode)
                f->hide_all(), f->stop_all();
            else if (!is_attacking())
                f->show_all(), f->stop_all();
            else 
                f->show_all(), f->start_all();
        }

        if (auto s = drone_set_slow())
        {
            if (!slowmode)
                s->hide_all(), s->stop_all();
            else if (!is_attacking())
                s->show_all(), s->stop_all();
            else
                s->show_all(), s->start_all();
        }
    }

    // movement
    {
        double v = slowmode ? slowmode_speed : speed;
        double k = (move_left || move_right) && (move_up || move_down) ? sqrthalf : 1.0;
        double dx = (move_left ? -v : 0.0) + (move_right ? v : 0.0);
        double dy = (move_up ? -v : 0.0) + (move_down ? v : 0.0);
        dx *= k, dy *= k;

        if (lock_move)
            dx = 0.0, dy = 0.0;

        double x = this->x() + dx;
        double y = this->y() + dy;
 
        if (!killed)
        {
            x = std::max(x, -boundary_l);
            y = std::max(y, -boundary_t);
            x = std::min(x, GameSystem::game_w + boundary_r);
            y = std::min(y, GameSystem::game_h + boundary_b);
        }
        set_pos(x, y);

        if (lock_move)
            set_act_state((int)ActState::still);
        else if(move_left && !move_right)
            set_act_state((int)ActState::left);
        else if (!move_left && move_right)
            set_act_state((int)ActState::right);
        else
        {
            if (current_act_state() == (int)ActState::left)
                set_act_state((int)ActState::still_from_left);
            if (current_act_state() == (int)ActState::right)
                set_act_state((int)ActState::still_from_right);
        }

        slowmode ? on_slow_mode() : on_fast_mode();

        bool moved = false;
        if (move_left && !move_right)
            on_move_left(), moved = true;
        if (!move_left && move_right)
            on_move_right(), moved = true;
        if (move_up && !move_down)
            on_move_up(), moved = true;
        if (!move_up && move_down)
            on_move_down(), moved = true;
        if (!moved)
            on_stand_still();
    }

    // item pick line
    {
        if (y() <= item_pick_line && !is_killed())
        {
            GameSystem::instance()->pick_all_item();
        }
    }
}

void Player::on_destroy()
{
    hit_point()->destroy();
    graze_circle->destroy();

    remove_main_gun();
    remove_drone_set_fast();
    remove_drone_set_slow();

    remove_fast_bomb();
    remove_slow_bomb();

    if (GameSystem::instance()->get_player() == this)
    {
        GameSystem::instance()->remove_player();
    }

    ActorObject::on_destroy();
}

void Player::on_draw()
{
    ActorObject::on_draw();
}

void Player::on_draw_ui()
{
    ActorObject::on_draw_ui();

    if (cheat_mode)
    {
        TextDrawcall call;
        call.str = "CHEAT";
        call.x = GameSystem::game_w - 5;
        call.y = GameSystem::game_h - 5;
        call.anchor = TextAnchor::right_buttom;
        call.size = 14;
        call.color = Color(1, 1, 1);
        call.set_edge(Color(1.0, 0.2, 0.2), 1);
        call.level = DrawLevel::top;
        DrawSystem::instance()->draw_text(call);
    }

    if (t() - last_show_item_pick_line_time <= 120)
    {
        double a = utils::clamp(2.0 - (t() - last_show_item_pick_line_time) / 60.0, 0.0, 1.0);
        if ((t() / 2) & 1)
            a *= 0.2;

        TextDrawcall call;
        call.str = "Item Pickup Line";
        call.x = GameSystem::game_w * 0.5;
        call.y = item_pick_line - 10;
        call.anchor = TextAnchor::center;
        call.size = 14;
        call.color = Color(1, 1, 1, a);
        call.set_edge(Color(0.4, 0.8, 1.0, a), 1);
        call.level = DrawLevel::top;
        DrawSystem::instance()->draw_text(call);

        DrawSystem::instance()->draw_func([a, this](sf::RenderTarget* target) {
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(GameSystem::game_w, 1));
            line.setFillColor(Color(0.4, 0.8, 1.0, a));
            line.setPosition(0, this->get_item_pick_line());
            target->draw(line);
            });
    }

    if (t() - last_full_power_time <= 120)
    {
        double a = utils::clamp(2.0 - (t() - last_full_power_time) / 60.0, 0.0, 1.0);
        TextDrawcall call;
        call.str = "Full Power";
        call.x = GameSystem::game_w * 0.5;
        call.y = GameSystem::game_h * 0.2 + 20;
        call.anchor = TextAnchor::center;
        call.size = 17;
        call.color = Color(1, 1, 1, a);
        call.set_edge(Color(1.0, 0.2, 0.2, a), 1);
        call.level = DrawLevel::top;
        DrawSystem::instance()->draw_text(call);
    }

    if (t() - last_extend_life_time <= 120)
    {
        double a = utils::clamp(2.0 - (t() - last_extend_life_time) / 60.0, 0.0, 1.0);
        TextDrawcall call;
        call.str = "Extended!";
        call.x = GameSystem::game_w * 0.5;
        call.y = GameSystem::game_h * 0.2;
        call.anchor = TextAnchor::center;
        call.size = 17;
        call.color = Color(1, 1, 1, a);
        call.set_edge(Color(1.0, 0.2, 0.5, a), 1);
        call.level = DrawLevel::top;
        DrawSystem::instance()->draw_text(call);
    }

    if (t() - last_extend_bomb_time <= 120)
    {
        double a = utils::clamp(2.0 - (t() - last_extend_bomb_time) / 60.0, 0.0, 1.0);
        TextDrawcall call;
        call.str = "Extended!";
        call.x = GameSystem::game_w * 0.5;
        call.y = GameSystem::game_h * 0.2;
        call.anchor = TextAnchor::center;
        call.size = 17;
        call.color = Color(1, 1, 1, a);
        call.set_edge(Color(0.2, 1.0, 0.2, a), 1);
        call.level = DrawLevel::top;
        DrawSystem::instance()->draw_text(call);
    }
}

void Player::on_change_act_state(int state)
{
    ActorObject::on_change_act_state(state);

}

void Player::on_collide(PhysicObject* obj)
{
    CollideCircle::on_collide(obj);

    if (auto b = dynamic_cast<EnemyBullet*>(obj))
    {
        kill();
    }
    if (auto l = dynamic_cast<Laser*>(obj))
    {
        kill();
    }

    if (auto e = dynamic_cast<Enemy*>(obj))
    {
        if (e->is_can_hit_player())
        {
            kill();
        }
    }
    if (auto b = dynamic_cast<Boss*>(obj))
    {
        if (b->is_can_hit_player())
        {
            kill();
        }
    }
}


void Player::on_kill()
{
    
}

void Player::on_rescue()
{

}

void Player::on_slow_mode()
{
    if (lock_move)
    {
        hit_point()->hide();
    }
    else
    {
        hit_point()->show();
    }
}

void Player::on_fast_mode()
{
    hit_point()->hide();
}

void Player::on_move_left()
{
}

void Player::on_move_right()
{
}

void Player::on_move_up()
{
}

void Player::on_move_down()
{
}

void Player::on_stand_still()
{
}

void Player::on_change_power()
{

}

void Player::on_attack()
{

}

void Player::on_silent()
{
}

void Player::on_bomb()
{
}

void Player::on_graze(EnemyBullet* b)
{
}

void Player::on_graze(Laser* l)
{
}


void Player::on_key_press(Key key)
{
    switch (key)
    {
    case Key::shoot:
        attacking = true;  break;
    case Key::bomb:
        bomb();  break;
    case Key::special:
        break;
    case Key::up:
        move_up = true;  break;
    case Key::down:
        move_down = true;  break;
    case Key::left:
        move_left = true;  break;
    case Key::right:
        move_right = true;  break;
    case Key::slow:
        set_slowmode(true); break;
    case Key::debug:
        set_cheat_mode(cheat_mode ^ 1);  break;
    default:
        break;
    }
}

void Player::on_key_release(Key key)
{
    switch (key)
    {
    case Key::shoot:
        attacking = false;  break;
    case Key::bomb:
        break;
    case Key::special:
        break;
    case Key::up:
        move_up = false;  break;
    case Key::down:
        move_down = false;  break;
    case Key::left:
        move_left = false;  break;
    case Key::right:
        move_right = false;  break;
    case Key::slow:
        set_slowmode(false); break;
    case Key::debug:
        break;
    default:
        break;
    }
}


