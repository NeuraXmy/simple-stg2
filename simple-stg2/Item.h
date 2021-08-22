#pragma once

#include "SpriteObject.h"
#include "CollideCircle.h"

class Player;
namespace Sound { class Se; }

class Item : public SpriteObject, public CollideCircle
{
public:

	Item();

	virtual ~Item() = default;


	void pick(Player* player);

	bool is_picked() const;


	void set_pick_radius(double r);

	void set_start_info(double v_0, int ani_time, bool ani_rotate, bool ani_scale);

	void set_drop_info(double v_max, double a, double buttom);

	
	Sprite& indicator();

	const Sprite& indicator() const;


	Sound::Se* get_pick_se() const;

	void set_pick_se(Sound::Se* se);


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;

	virtual void on_collide(PhysicObject* obj) override;

	
	virtual void on_pick(Player* player);


private:

	bool picked = false;

	double pick_radius = 1.0;

	double v_0 = 0.0;
	double v_max = 0.0;
	double a = 0.0;

	int ani_time = 30;
	bool ani_rotate = true;
	bool ani_scale = true;

	double boundary_buttom = 20;

	Sprite indicator_sprite;

	Sound::Se* pick_se;

};

