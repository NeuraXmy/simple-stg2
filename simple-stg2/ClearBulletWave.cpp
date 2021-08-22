#include "ClearBulletWave.h"

#include "EnemyBullet.h"
#include "Enemy.h"
#include "Boss.h"
#include "Laser.h"


bool ClearBulletWave::is_raw_kill() const
{
    return raw_kill;
}

void ClearBulletWave::set_raw_kill(bool flag)
{
    raw_kill = flag;
}

bool ClearBulletWave::is_force_kill() const
{
    return force_kill;
}

void ClearBulletWave::set_force_kill(bool flag)
{
    force_kill = flag;
}

bool ClearBulletWave::is_bullet_drop() const
{
    return bullet_drop;
}

void ClearBulletWave::set_bullet_drop(bool flag)
{
    bullet_drop = flag;
}

int ClearBulletWave::get_damage() const
{
    return damage;
}

void ClearBulletWave::set_damage(int damage)
{
    this->damage = damage;
}

void ClearBulletWave::on_create()
{
    Wave::on_create();

}

void ClearBulletWave::on_update()
{
    Wave::on_update();

}

void ClearBulletWave::on_destroy()
{

    Wave::on_destroy();
}

void ClearBulletWave::on_draw()
{
    Wave::on_draw();

}

void ClearBulletWave::on_cover(PhysicObject* obj)
{
    Wave::on_cover(obj);

    if(auto b = dynamic_cast<EnemyBullet*>(obj))
    {
        if (b->is_can_be_killed_by_player() || force_kill)
        {
            if (raw_kill)
            {
                b->kill_raw();
            }
            else
            {
                b->kill(bullet_drop);
            }
        }
    }
    if (auto l = dynamic_cast<Laser*>(obj))
    {
        if (l->is_can_be_killed_by_player() || force_kill)
        {
            if (raw_kill)
            {
                l->kill_raw();
            }
            else
            {
                l->kill(bullet_drop);
            }
        }
    }

    if (auto e = dynamic_cast<Enemy*>(obj))
    {
        e->damage_health(damage);
    }
    if (auto b = dynamic_cast<Boss*>(obj))
    {
        b->damage_health(damage);
    }
}

void ClearBulletWave::on_reach_max_radius()
{
    Wave::on_reach_max_radius();

    destroy();
}

void ClearBulletWave::on_start_expand()
{
    Wave::on_start_expand();

}
