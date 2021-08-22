#include "Laser.h"
#include "Utils.h"
#include "DrawSystem.h"
#include "Player.h"
#include "ParticleSystem.h"
#include "GameSystem.h"
#include "PlayerBullet.h"
#include "Sound.h"


Laser::Laser() : CollidePointSet(CollideGroup::enemy_bullet, this)
{

}


bool Laser::is_head_light() const
{
    return head_light;
}

void Laser::set_head_light(bool flag)
{
    head_light = flag;
}

bool Laser::is_tail_light() const
{
    return tail_light;
}

void Laser::set_tail_light(bool flag)
{
    tail_light = flag;
}

double Laser::get_light_offset() const
{
    return light_offset;
}

void Laser::set_light_offset(double offset)
{
    light_offset = offset;
}

double Laser::get_light_omega() const
{
    return light_omega;
}

void Laser::set_light_omega(double omega)
{
    light_omega = omega;
}

Sprite& Laser::light_sprite()
{
    return light_sprite_d;
}

const Sprite& Laser::light_sprite() const
{
    return light_sprite_d;
}

Sprite& Laser::create_sprite()
{
    return create_sprite_d;
}

const Sprite& Laser::create_sprite() const
{
    return create_sprite_d;
}


double Laser::get_w() const
{
    return w;
}

void Laser::set_w(double w)
{
    this->w = w;
}

double Laser::get_len() const
{
    return len;
}

void Laser::set_len(double len)
{
    this->len = len;
    update_collider(true);
}

double Laser::get_full_len() const
{
    return full_len;
}

void Laser::set_full_len(double len)
{
    full_len = len;
}

bool Laser::is_grow_len() const
{
    return grow_len;
}

void Laser::set_grow_len(bool flag)
{
    grow_len = flag;
}

void Laser::set_two_point(double head_x, double head_y, double tail_x, double tail_y, double offset)
{
    double dx = head_x - tail_x;
    double dy = head_y - tail_y;
    double len = sqrt(dx * dx + dy * dy);
    double fx = offset / len * dx;
    double fy = offset / len * dy;
    set_len(len + offset * 2.0);
    set_pos(head_x + fx, head_y + fy);
    set_rot(atan2(head_y - tail_y, head_x - tail_x));
}

double Laser::center_x() const
{
    return x() - len * std::cos(rot()) * 0.5;
}

double Laser::center_y() const
{
    return y() - len * std::sin(rot()) * 0.5;
}

double Laser::center_x_relative() const
{
    return x_relative() - len * std::cos(rot()) * 0.5;
}

double Laser::center_y_relative() const
{
    return y_relative() - len * std::sin(rot()) * 0.5;
}

void Laser::set_tail_physic_enabled(bool flag)
{
    if (flag && !tail_physic)
    {
        tail_physic = ObjectSystem::instance()->create_object<PhysicObject>();
        tail_physic->set_pos(x_relative() - len * cos(rot()), y_relative() - len * sin(rot()));
        tail_physic->bind(bound_obj());
        tail_physic->set_v(vx(), vy());
        tail_physic->set_a(ax(), ay());
    }
    else if (!flag && tail_physic)
    {
        tail_physic->destroy();
        tail_physic = nullptr;
    }
}

bool Laser::is_tail_physic_enabled(bool flag)
{
    return tail_physic;
}

PhysicObject* Laser::get_tail_physic() const
{
    return tail_physic;
}


void Laser::turn_half_on()
{
    if (state != State::half_on)
    {
        state = State::half_on;
        set_collide(false);
    }
}

void Laser::turn_on(int time)
{
    if (state != State::on)
    {
        turn_on_ani_time = time;
        turn_on_end_time = t() + time;
        if (on_se)
            on_se->play();
        set_collide(false);
    }
}

void Laser::turn_off(int time)
{
    if (state != State::off)
    {
        turn_off_ani_time = time;
        turn_off_end_time = t() + time;
        set_collide(false);
    }
}

bool Laser::is_half_on() const
{
    return state == State::half_on;
}

bool Laser::is_on() const
{
    return state == State::on;
}

bool Laser::is_off() const
{
    return state == State::off;
}

Laser::State Laser::get_state() const
{
    return state;
}

double Laser::get_half_on_w() const
{
    return half_on_w;
}

void Laser::set_half_on_w(double w)
{
    half_on_w = w;
}


void Laser::set_collider_interval(double interval)
{
    collider_interval = interval;
    update_collider(true);
}

void Laser::set_head_tail_no_collider_count(int head, int tail)
{
    head_no_collider_count = head;
    tail_no_collider_count = tail;
    update_collider(true);
}

double Laser::get_collider_w() const
{
    return collider_w;
}

void Laser::set_collider_w(double w)
{
    collider_w = w;
    update_collider(true);
}


bool Laser::is_grazed() const
{
    return grazed;
}

int Laser::get_graze_interval() const
{
    return graze_interval;
}

void Laser::set_graze_interval(int interval)
{
    graze_interval = interval;
}

bool Laser::is_can_be_killed_by_player() const
{
    return can_be_killed_by_player;
}

void Laser::set_can_be_killed_by_player(bool flag)
{
    can_be_killed_by_player = flag;
}


bool Laser::is_boundary_test_enabled() const
{
    return boundary_test;
}

void Laser::set_boundary_test(bool flag)
{
    boundary_test = flag;
}

void Laser::set_boundary(double l, double r, double t, double b)
{
    boundary_l = l;
    boundary_r = r;
    boundary_t = t;
    boundary_b = b;
}

void Laser::shorten_from_head(double len, bool drop_item)
{
    if (len > 1e-8)
    {
        if (kill_se)
            kill_se->play();

        foreach_drop_point(0.0, len, [this, drop_item](double x, double y) {
            if(drop_item)
                on_drop_item(x, y);
            ParticleSystem::instance()->create_particle(kill_particle_data, x, y);
        });

        set_pos(x_relative() - len * cos(rot()), y_relative() - len * sin(rot()));

        set_len(this->len - len);
        on_shorten_from_head(len);
    }
}

void Laser::shorten_from_tail(double len, bool drop_item)
{
    if (len < this->len - 1e-8)
    {
        if (kill_se)
            kill_se->play();

        foreach_drop_point(len, this->len, [this, drop_item](double x, double y) {
            if (drop_item)
                on_drop_item(x, y);
            ParticleSystem::instance()->create_particle(kill_particle_data, x, y);
            });

        set_len(len);
        on_shorten_from_tail(len);
    }
}

void Laser::set_shorten_from(bool is_from_head)
{
    default_shorten_from_head = is_from_head;
}

bool Laser::is_shorten_from_head() const
{
    return default_shorten_from_head;
}

bool Laser::is_shorten_from_tail() const
{
    return !default_shorten_from_head;
}

void Laser::kill(bool drop_item)
{
    if (!killed)
    {
        killed = true;

        default_shorten_from_head
            ? shorten_from_head(len, drop_item)
            : shorten_from_tail(0.0, drop_item);
        
        on_kill();

        destroy();
    }
}

void Laser::kill_raw()
{
    if (!killed)
    {
        killed = true;
        on_kill();
        destroy();
    }
}

bool Laser::is_killed() const
{
    return killed;
}

bool Laser::is_kill_when_bound_enemy_killed() const
{
    return kill_when_bound_enemy_killed;
}

void Laser::set_kill_when_bound_enemy_killed(bool flag)
{
    kill_when_bound_enemy_killed = flag;
}

bool Laser::is_destroy_when_off() const
{
    return destroy_when_off;
}

void Laser::set_destroy_when_off(bool flag)
{
    destroy_when_off = flag;
}

Particle::DataSet& Laser::kill_particle()
{
    return kill_particle_data;
}

const Particle::DataSet& Laser::kill_particle() const
{
    return kill_particle_data;
}

double Laser::get_drop_interval() const
{
    return drop_interval;
}

void Laser::set_drop_interval(double interval)
{
    drop_interval = interval;
}

void Laser::graze()
{
    grazed = true;
    last_graze_time = t();
}

Sound::Se* Laser::get_on_se() const
{
    return on_se;
}

void Laser::set_on_se(Sound::Se* se)
{
    on_se = se;
}

Sound::Se* Laser::get_kill_se() const
{
    return kill_se;
}

void Laser::set_kill_se(Sound::Se* se)
{
    kill_se = se;
}

void Laser::on_create()
{
    SpriteObject::on_create();

    sprite().set_level(DrawLevel::enemy_bullet_1);
    light_sprite().set_level(DrawLevel::enemy_bullet_1);
}

void Laser::on_update()
{
    // do not use SpriteObject::on_update() : keep drag show rightly
    {
        PhysicObject::on_update();
        
        sprite().update();

        if (drag_cnt && t() - last_drag_time >= drag_interval)
        {
            Particle::Data p;
            p.sprite = sprite();
            p.sprite.set_blend(drag_blend);
            p.sprite.pause();
            p.sprite.set_alpha(p.sprite.alpha() * drag_alpha);
            p.sprite.set_scale(p.sprite.w_scale() * drag_scale * w_scale, p.sprite.h_scale() * drag_scale * h_scale);
            p.cnt = 1;
            p.rot = rot();
            p.lifeTime = (double)drag_interval * drag_cnt;
            if (drag_fade)		p.set_option(Particle::Options::fade_out);
            if (drag_shrink)	p.set_option(Particle::Options::shrink);
            ParticleSystem::instance()->create_particle(p, center_x(), center_y());

            last_drag_time = t();
        }
    }

    // on off
    {
        if (t() == turn_on_end_time)
        {
            state = State::on;
            set_collide(true);
        }
        if (t() == turn_off_end_time)
        {
            state = State::off;

            if (destroy_when_off)
            {
                kill_raw();
            }

            set_collide(false);
        }
    }

    // len grow
    {
        if (grow_len && len < full_len)
        {
            double dlt = v();

            if (len + dlt >= full_len - 1e-8)
            {
                grow_len = false;
                len = full_len;
            }
            else
                len += dlt;

            update_collider();
        }
    }

    if (tail_physic)
    {
        set_two_point(x_relative(), y_relative(), tail_physic->x_relative(), tail_physic->y_relative());
    }

    // graze
    {
        if (t() == last_graze_time + graze_interval)
        {
            grazed = false;
        }
    }

    // boundary
    if (boundary_test)
    {
        double sx = x();
        double sy = y();
        double tx = x() - len * cos(rot());
        double ty = y() - len * sin(rot());

        if (boundary_test &&
            (tx < -boundary_l || tx > GameSystem::game_w + boundary_r
          || ty < -boundary_t || ty > GameSystem::game_h + boundary_b) &&
            (sx < -boundary_l || sx > GameSystem::game_w + boundary_r
          || sy < -boundary_t || sy > GameSystem::game_h + boundary_b))
            kill_raw();
    }
}

void Laser::on_destroy()
{

    SpriteObject::on_destroy();
}

void Laser::on_draw()
{
    // laser
    {
        double ws = sprite().w_scale();
        double hs = sprite().h_scale();

        if (sprite().ready())
        {
            double current_w = w;
            if (t() <= turn_on_end_time)
            {
                double k = 1.0 - double(turn_on_end_time - t()) / turn_on_ani_time;
                if (state == State::off)
                    current_w = utils::lerp(0.0, w, k);
                if (state == State::half_on)
                    current_w = utils::lerp(half_on_w, w, k);
            }
            else if (t() <= turn_off_end_time)
            {
                double k = 1.0 - double(turn_off_end_time - t()) / turn_off_ani_time;
                if (state == State::on)
                    current_w = utils::lerp(w, 0.0, k);
                if (state == State::half_on)
                    current_w = utils::lerp(half_on_w, 0.0, k);
            }
            else if (state == State::half_on)
            {
                current_w *= half_on_w;
            }
            else if (state == State::off)
            {
                current_w = 0.0;
            }

            double cws = ws * len       / sprite().current_coord().width;
            double chs = hs * current_w / sprite().current_coord().height;

            w_scale = cws / sprite().w_scale();
            h_scale = chs / sprite().h_scale();

            sprite().set_scale(cws, chs);
        }

        double center_x = x() - len * std::cos(rot()) * 0.5;
        double center_y = y() - len * std::sin(rot()) * 0.5;

        DrawSystem::instance()->draw_sprite(sprite(), center_x, center_y, rot());

        sprite().set_scale(ws, hs);
    }

    // head light
    if(head_light && light_sprite().ready() && len > 1e-2)
    {
        light_rot += light_omega;

        double a = light_sprite().alpha();
        double ca = a;

        if (t() <= turn_on_end_time)
        {
            double k = 1.0 - double(turn_on_end_time - t()) / turn_on_ani_time;
            ca = utils::lerp(0.0, a, k);
        }
        else if (t() <= turn_off_end_time)
        {
            double k = 1.0 - double(turn_off_end_time - t()) / turn_off_ani_time;
            ca = utils::lerp(a, 0.0, k);
        }
        else if (state != State::on)
        {
            ca = 0.0;
        }

        light_sprite().set_alpha(ca);

        double off_x = light_offset * std::cos(rot());
        double off_y = light_offset * std::sin(rot());

        DrawSystem::instance()->draw_sprite(light_sprite(),
            x() + off_x, y() + off_y, light_rot + rot());

        light_sprite().set_alpha(a);
    }

    // tail light 
    if (tail_light && light_sprite().ready() && len > 1e-2)
    {
        light_rot += light_omega;

        double a = light_sprite().alpha();
        double ca = a;

        if (t() <= turn_on_end_time)
        {
            double k = 1.0 - double(turn_on_end_time - t()) / turn_on_ani_time;
            ca = utils::lerp(0.0, a, k);
        }
        else if (t() <= turn_off_end_time)
        {
            double k = 1.0 - double(turn_off_end_time - t()) / turn_off_ani_time;
            ca = utils::lerp(a, 0.0, k);
        }
        else if (state != State::on)
        {
            ca = 0.0;
        }

        light_sprite().set_alpha(ca);

        double off_x = -(light_offset + len) * std::cos(rot());
        double off_y = -(light_offset + len) * std::sin(rot());

        DrawSystem::instance()->draw_sprite(light_sprite(),
            x() + off_x, y() + off_y, light_rot + rot());

        light_sprite().set_alpha(a);
    }

    // create sprite
    if (create_sprite_d.ready())
    {
        if(grow_len && len < full_len - 0.001)
            create_sprite().set_alpha(1.0);
        else
        {
            double a = create_sprite_d.alpha();
            create_sprite_d.set_alpha(std::max(0.0, a - 1.0 / 5.0));
        }

        if (create_sprite_d.alpha() >= 0.001)
        {
            light_rot += light_omega;
            double off_x = -(light_offset + len) * std::cos(rot());
            double off_y = -(light_offset + len) * std::sin(rot());
            DrawSystem::instance()->draw_sprite(create_sprite(),
                x() + off_x, y() + off_y, light_rot + rot());
        }
    }
}

void Laser::on_collide(PhysicObject* obj)
{
    if (can_be_killed_by_player)
    {
        if (auto p = dynamic_cast<Player*>(obj))
        {
            double len = -get_collided_point().x;
            default_shorten_from_head
                ? shorten_from_head(len, false)
                : shorten_from_tail(len, false);
            set_grow_len(false);
            if (this->len < 10.0)
                kill(false);
        }
        if (auto b = dynamic_cast<PlayerBullet*>(obj))
        {
            if (b->is_can_kill_enemy_bullet())
            {
                double len = -get_collided_point().x;
                default_shorten_from_head
                    ? shorten_from_head(len, true)
                    : shorten_from_tail(len, true);
                set_grow_len(false);
                if (this->len < 10.0)
                    kill(true);
            }
        }
    }
}

void Laser::on_drop_item(double x, double y)
{
}

void Laser::on_kill()
{
}

void Laser::on_shorten_from_head(double len)
{
}

void Laser::on_shorten_from_tail(double len)
{
}

void Laser::on_bound_obj_destroyed(PhysicObject* bound_obj)
{
    SpriteObject::on_bound_obj_destroyed(bound_obj);

    if (kill_when_bound_enemy_killed)
    {
        kill(true);
    }
}


void Laser::update_collider(bool clear)
{
    auto& s = get_collide_points();

    if (clear)
        s.clear();

    double x = s.size() ? -s.back().x + collider_interval : head_no_collider_count * collider_interval;
    
    while (x <= len - tail_no_collider_count * collider_interval + 1e-8)
    {
        CollidePointSet::Point p;
        p.x = -x;
        p.y = 0.0;
        p.r = collider_w;
        p.enable = true;
        s.push_back(p);
        x += collider_interval;
    }
}
