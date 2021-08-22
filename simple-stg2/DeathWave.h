#pragma once

#include "Wave.h"


class DeathWave : public Wave
{
public:

	virtual ~DeathWave() = default;


	bool is_can_kill_enemy_bullet() const;

	void set_can_kill_enemy_bullet(bool flag);

	int get_damage() const;

	void set_damage(int damage);


	bool is_visible() const;

	void set_visible(bool flag);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_cover(PhysicObject* obj) override;

	virtual void on_reach_max_radius() override;

	virtual void on_start_expand() override;

private:

	bool can_kill_enemy_bullet = true;

	int damage = 0;

	bool visible = true;
	
};

