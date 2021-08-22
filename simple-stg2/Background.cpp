#include "Background.h"
#include "DrawSystem.h"

void Background::draw(int t)
{
    on_draw(t);
}

void Background::update(int t)
{
    on_update(t);
}

bool Background::is_visible() const
{
    return visible;
}

void Background::set_visible(bool flag)
{
    visible = flag;
}

double Background::get_alpha() const
{
    return alpha;
}

void Background::set_alpha(double a)
{
    alpha = a;
}

DrawLevel Background::get_level() const
{
    return level;
}

void Background::set_level(DrawLevel level)
{
    this->level = level;
}



void Background::draw_sprite(Sprite& s, double x, double y, double rot)
{
    double a = s.alpha();
    s.set_alpha(a * alpha);

    if (s.level() > DrawLevel::background_2)
        s.set_level(level);

    DrawSystem::instance()->draw_sprite(s, x, y, rot);

    s.set_alpha(a);
}

void Background::draw_sprite(Sprite& s, const Texture::Quad& q)
{
    double a = s.alpha();
    s.set_alpha(a * alpha);

    if (s.level() > DrawLevel::background_2)
        s.set_level(level);

    DrawSystem::instance()->draw_sprite(s, q);

    s.set_alpha(a);
}

void Background::on_draw(int t)
{

}

void Background::on_update(int t)
{
}
