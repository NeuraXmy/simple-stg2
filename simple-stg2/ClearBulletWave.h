#pragma once

#include "Wave.h"


class ClearBulletWave : public Wave
{
public:

	virtual ~ClearBulletWave() = default;


	bool is_raw_kill() const;

	void set_raw_kill(bool flag);

	bool is_force_kill() const;

	void set_force_kill(bool flag);

	bool is_bullet_drop() const;

	void set_bullet_drop(bool flag);

	int get_damage() const;

	void set_damage(int damage);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_cover(PhysicObject * obj) override;

	virtual void on_reach_max_radius() override;

	virtual void on_start_expand() override;

private:

	bool raw_kill = false;

	bool bullet_drop = true;

	bool force_kill = true;

	int damage = 0;

};

