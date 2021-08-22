#pragma once

#include "Bullet.h"


class PlayerBullet : public Bullet
{
public:

	PlayerBullet();

	virtual ~PlayerBullet() = default;


	int get_damage() const;

	void set_damage(int damage);

	bool is_penetrate() const;

	void set_penetrate(bool flag);

	bool is_can_kill_enemy_bullet() const;

	void set_can_kill_enemy_bullet(bool flag);

	int get_hit_score() const;

	void set_hit_score(int score);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_collide(PhysicObject* obj) override;

	virtual void on_kill() override;


private:

	int damage = 0;

	bool penetrate = false;

	bool can_kill_enemy_bullet = false;

	int hit_score = 1;

};
