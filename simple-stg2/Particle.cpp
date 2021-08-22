#include "Particle.h"
#include "DrawSystem.h"
#include "PhysicObject.h"
#include "DrawSystem.h"

namespace Particle
{
    static const double pi = acos(-1.0);


    void Data::set_option(Options option, bool flag)
    {
        if (flag)
        {
            options |= (int)option;
        }
        else
        {
            options &= ~(int)option;
        }
    }


    void Particle::init(const Data& data, double ox, double oy)
    {
        auto ds = DrawSystem::instance();

        life_time = data.lifeTime.get_draw();

        double r = data.rand_pos_r.get_draw();
        double theta = ds->rand_real(0.0, 2.0 * pi);
        x = ox + r * cos(theta);
        y = oy + r * sin(theta);

        if (data.options & (int)Options::gather)
        {
            double v = r / life_time;
            vx = v * -cos(theta);
            vy = v * -sin(theta);
        }
        else if (data.rand_v_direction)
        {
            double v = data.v.get_draw();
            double theta = ds->rand_real(0.0, 2.0 * pi);
            vx = v * cos(theta);
            vy = v * sin(theta);
        }
        else
        {
            vx = data.vx.get_draw();
            vy = data.vy.get_draw();
        }

        rot = data.rot.get_draw();
        omega = data.omega.get_draw();

        scale = data.scale.get_draw();

        sprite = data.sprite;

        update_func = data.update_func;
        options = data.options;

        update_count = 0;
        visible = true;
        to_be_destroy = false;
    }

    bool Particle::is_to_be_destroy() const
    {
        return to_be_destroy;
    }

    void Particle::destroy()
    {
        to_be_destroy = true;
    }

    void Particle::update()
    {
        if (life_time < 0 || update_count <= life_time)
        {
            double life_fac = life_factor();

            if (options & (int)Options::slow_down)
            {
                x += vx * life_fac;
                y += vy * life_fac;
                rot += omega * life_fac;
            }
            else
            {
                x += vx;
                y += vy;
                rot += omega;
            }

            sprite.update();

            if (options & (int)Options::destroy_when_ani_end)
            {
                if (sprite.current_frame() == sprite.frame_count() - 1)
                {
                    destroy();
                }
            }

            if (update_func)
            {
                (*update_func)(this);
            }
        }

        if (life_time >= 0 && ++update_count > life_time)
        {
            destroy();
        }
    }

    void Particle::draw()
    {
        if (!visible)
        {
            return;
        }

        double life_fac = life_factor();

        double a = sprite.alpha();
        double ws = sprite.w_scale();
        double hs = sprite.h_scale();

        if (options & (int)Options::fade_out)
            sprite.set_alpha(a * life_fac);
        else if(options & (int)Options::appear)
            sprite.set_alpha(a * (1.0 - life_fac));

        if (options & (int)Options::shrink)
            sprite.set_scale(ws * life_fac * scale, hs * life_fac * scale);
        else 
            sprite.set_scale(ws * scale, hs * scale);

        DrawSystem::instance()->draw_sprite(sprite, x, y, rot);

        sprite.set_alpha(a);
        sprite.set_scale(ws, hs);
    }

    int Particle::t() const
    {
        return update_count;
    }

    double Particle::life_factor() const
    {
        return 1.0 - (double)update_count / life_time;
    }
}