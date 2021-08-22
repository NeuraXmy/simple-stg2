#include "EnemyBullet.h"
#include "PlayerBullet.h"
#include "Player.h"

EnemyBullet::EnemyBullet() : Bullet(CollideGroup::enemy_bullet)
{

}

bool EnemyBullet::is_can_be_killed_by_player() const
{
    return can_be_killed_by_player;
}

void EnemyBullet::set_can_be_killed_by_player(bool flag)
{
    can_be_killed_by_player = flag;
}

bool EnemyBullet::is_grazed() const
{
    return grazed;
}

void EnemyBullet::set_grazed(bool flag)
{
    grazed = flag;
}

void EnemyBullet::on_create()
{
    Bullet::on_create();
    sprite().set_level(DrawLevel::enemy_bullet_1);
}

void EnemyBullet::on_update()
{
    Bullet::on_update();

}

void EnemyBullet::on_destroy()
{
    
    Bullet::on_destroy();
}

void EnemyBullet::on_draw()
{
    Bullet::on_draw();

}

void EnemyBullet::on_collide(PhysicObject* obj)
{
    Bullet::on_collide(obj);

    if (auto p = dynamic_cast<Player*>(obj))
    {
        if (this->can_be_killed_by_player)
        {
            kill(false);
        }
    }
    if (auto b = dynamic_cast<PlayerBullet*>(obj))
    {
        if (b->is_can_kill_enemy_bullet() && this->can_be_killed_by_player)
        {
            kill(true);
        }
    }
}

void EnemyBullet::on_kill()
{
    Bullet::on_kill();

}

void EnemyBullet::on_drop_item()
{
    Bullet::on_drop_item();
}
