#include "DeathWave.h"
#include "EnemyBullet.h"
#include "DrawSystem.h"
#include "ObjectSystem.h"

#include "ClearBulletWave.h"


bool DeathWave::is_can_kill_enemy_bullet() const
{
    return can_kill_enemy_bullet;
}

void DeathWave::set_can_kill_enemy_bullet(bool flag)
{
    can_kill_enemy_bullet = flag;   
}

int DeathWave::get_damage() const
{
    return damage;
}

void DeathWave::set_damage(int damage)
{
    this->damage = damage;
}

bool DeathWave::is_visible() const
{
    return visible;
}

void DeathWave::set_visible(bool flag)
{
    visible = flag;
}


void DeathWave::on_create()
{
    Wave::on_create();

    set_expand_a(1.0);
    set_initial_expand_v(15.0);
    set_max_radius(4000.0);
    set_damage(3);
}

void DeathWave::on_update()
{
    Wave::on_update();

}

void DeathWave::on_destroy()
{

    Wave::on_destroy();
}

void DeathWave::on_draw()
{
    Wave::on_draw();

    if (!visible)
    {
        return;
    }


    double r = get_radius();
    double x = this->x();
    double y = this->y();

    auto draw = [r, x, y](sf::RenderTexture* target) mutable {

        constexpr double offset = 20.0;
        auto& mode = sf_blendmodes[(int)BlendMode::negative];

        sf::CircleShape cir;
        cir.setPointCount(256);
        cir.setRadius(r), cir.setOrigin(r, r);
        cir.setPosition(x, y), target->draw(cir, mode);

        r *= 0.4;
        cir.setRadius(r), cir.setOrigin(r, r);
        cir.setPosition(x + offset, y), target->draw(cir, mode);
        cir.setPosition(x - offset, y), target->draw(cir, mode);
        cir.setPosition(x, y - offset), target->draw(cir, mode);
        cir.setPosition(x, y + offset), target->draw(cir, mode);

        r *= 0.5;
        cir.setRadius(r),  cir.setOrigin(r, r);
        cir.setPosition(x, y), target->draw(cir, mode);
    };
    DrawSystem::instance()->draw_func(std::move(draw), DrawLevel::top);
}

void DeathWave::on_cover(PhysicObject* obj)
{
    Wave::on_cover(obj);
}

void DeathWave::on_reach_max_radius()
{
    Wave::on_reach_max_radius();

    set_enabled(false);
    set_visible(false);
}

void DeathWave::on_start_expand()
{
    Wave::on_start_expand();

    set_visible(true);

    if (can_kill_enemy_bullet)
    {
        add_task([this]() {
            auto w = ObjectSystem::instance()->create_object<ClearBulletWave>();
            w->set_bullet_drop(false);
            w->set_damage(damage);
            w->set_pos(this);
            w->set_initial_expand_v(15.0);
            w->set_max_radius(1200.0);
            w->set_force_kill(false);
            w->start_expand();
        }, 45);
    }
}
