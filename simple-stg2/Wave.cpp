#include "Wave.h"
#include "ObjectSystem.h"


bool Wave::is_enabled() const
{
    return enabled;
}

void Wave::set_enabled(bool flag)
{
    enabled = flag;
}

void Wave::start_expand()
{
    radius = 0.0;
    last_update_radius = 0.0;
    expand_v = initial_v;
    enabled = true;

    on_start_expand();
}

double Wave::get_radius() const
{
    return radius;
}

void Wave::set_radius(double r)
{
    radius = r;
}

double Wave::get_max_radius() const
{
    return max_radius;
}

void Wave::set_max_radius(double r)
{
    max_radius = r;
}

double Wave::get_expand_v() const
{
    return expand_v;
}

void Wave::set_expand_v(double v)
{
    expand_v = v;
}

double Wave::get_expand_a() const
{
    return expand_a;
}

void Wave::set_expand_a(double a)
{
    expand_a = a;
}

double Wave::get_initial_expand_v() const
{
    return initial_v;
}

void Wave::set_initial_expand_v(double v)
{
    initial_v = v;
}


void Wave::on_create()
{
    PhysicObject::on_create();

}

void Wave::on_update()
{
    PhysicObject::on_update();

    if (!enabled)
    {
        return;
    }

    last_update_radius = radius;

    expand_v += expand_a;
    radius += expand_v;

    if (radius > max_radius)
    {
        on_reach_max_radius();

        radius = max_radius;
    }

    double r2 = radius * radius + 1e-9;

    ObjectSystem::instance()->foreach<PhysicObject>([this, r2](PhysicObject* obj) {
        double dx = x() - obj->x();
        double dy = y() - obj->y();
        double d = dx * dx + dy * dy;

        if (d <= r2)
        {
            on_cover(obj);
        }
    });
}

void Wave::on_destroy()
{

    PhysicObject::on_destroy();
}

void Wave::on_draw()
{
    PhysicObject::on_draw();

}

void Wave::on_cover(PhysicObject* obj)
{

}

void Wave::on_reach_max_radius()
{
    
}

void Wave::on_start_expand()
{
}
