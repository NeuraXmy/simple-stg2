#pragma once

#include <queue>
#include <vector>

#include "PhysicObject.h"
#include "Sprite.h"

enum class DrawLevel;
enum class BlendMode;


class SpriteObject : public PhysicObject
{
	friend class Laser;

public:

	virtual ~SpriteObject() = default;


	void set_sprite(Texture::Texture* tex, Texture::Coords* coords);

	void set_sprite(const Sprite& sprite);

	Sprite& sprite();

	const Sprite& sprite() const;


	int get_drag() const;

	void set_drag(int cnt);

	bool get_drag_fade() const;

	void set_drag_fade(bool flag);

	bool get_drag_shrink() const;

	void set_drag_shrink(bool flag);

	int get_drag_interval() const;

	void set_drag_interval(int interval);

	double get_drag_alpha() const;

	void set_drag_alpha(double a);

	double get_drag_scale() const;

	void set_drag_scale(double s);

	BlendMode get_drag_blend() const;

	void set_drag_blend(BlendMode blend);

	int get_last_drag_time() const;

	
protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;


private:

	Sprite d_sprite;


	int drag_cnt = 0;
	int drag_interval = 1;
	int last_drag_time = 0;

	bool drag_fade = true;
	bool drag_shrink = true;

	double drag_alpha = 1.0;
	double drag_scale = 1.0;

	BlendMode drag_blend = BlendMode::plus;

};
