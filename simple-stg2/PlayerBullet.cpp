#include "PlayerBullet.h"
#include "Enemy.h"
#include "Boss.h"


PlayerBullet::PlayerBullet() : Bullet(CollideGroup::player_bullet)
{
}

int PlayerBullet::get_damage() const
{
    return damage;
}

void PlayerBullet::set_damage(int damage)
{
    this->damage = damage;
}

bool PlayerBullet::is_penetrate() const
{
    return penetrate;
}

void PlayerBullet::set_penetrate(bool flag)
{
    penetrate = flag;
}

bool PlayerBullet::is_can_kill_enemy_bullet() const
{
    return can_kill_enemy_bullet;
}

void PlayerBullet::set_can_kill_enemy_bullet(bool flag)
{
    can_kill_enemy_bullet = flag;
    set_collide_group(flag ? CollideGroup::player_bullet_bomb : CollideGroup::player_bullet);
}

int PlayerBullet::get_hit_score() const
{
    return hit_score;
}

void PlayerBullet::set_hit_score(int score)
{
    hit_score = score;
}

void PlayerBullet::on_create()
{
    Bullet::on_create();
    sprite().set_level(DrawLevel::player_bullet_1);

}

void PlayerBullet::on_update()
{
    Bullet::on_update();

}

void PlayerBullet::on_destroy()
{

    Bullet::on_destroy();
}

void PlayerBullet::on_draw()
{
    Bullet::on_draw();

}

void PlayerBullet::on_collide(PhysicObject* obj)
{
    Bullet::on_collide(obj);

    if (!penetrate)
    {
        if (auto e = dynamic_cast<Enemy*>(obj))
        {
            kill();
        }
        if (auto e = dynamic_cast<Boss*>(obj))
        {
            kill();
        }
    }
}

void PlayerBullet::on_kill()
{
    Bullet::on_kill();

}
