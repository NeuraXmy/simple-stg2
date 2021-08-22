#pragma once

#include "Bullet.h"


class EnemyBullet : public Bullet
{
public:

	EnemyBullet();

	virtual ~EnemyBullet() = default;


	bool is_can_be_killed_by_player() const;

	void set_can_be_killed_by_player(bool flag);

	
	bool is_grazed() const;

	void set_grazed(bool flag);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_collide(PhysicObject* obj) override;

	virtual void on_kill() override;

	virtual void on_drop_item() override;

private:

	bool can_be_killed_by_player = true;

	bool grazed = false;



};
