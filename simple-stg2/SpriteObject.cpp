#include "SpriteObject.h"
#include "DrawSystem.h"
#include "ParticleSystem.h"
#include "Particle.h"

void SpriteObject::set_sprite(Texture::Texture* tex, Texture::Coords* coords)
{
	d_sprite.set_texture(tex);
	d_sprite.set_coords(coords);
}

void SpriteObject::set_sprite(const Sprite& sprite)
{
	d_sprite = sprite;
}

Sprite& SpriteObject::sprite()
{
	return d_sprite;
}

const Sprite& SpriteObject::sprite() const
{
	return d_sprite;
}

int SpriteObject::get_drag() const
{
	return drag_cnt;
}

void SpriteObject::set_drag(int cnt)
{
	drag_cnt = cnt;
}

bool SpriteObject::get_drag_fade() const
{
	return drag_fade;
}

void SpriteObject::set_drag_fade(bool flag)
{
	drag_fade = flag;
}

bool SpriteObject::get_drag_shrink() const
{
	return drag_shrink;
}

void SpriteObject::set_drag_shrink(bool flag)
{
	drag_shrink = flag;
}

int SpriteObject::get_drag_interval() const
{
	return drag_interval;
}

void SpriteObject::set_drag_interval(int interval)
{
	drag_interval = interval;
}

double SpriteObject::get_drag_alpha() const
{
	return drag_alpha;
}

void SpriteObject::set_drag_alpha(double a)
{
	drag_alpha = a;
}

double SpriteObject::get_drag_scale() const
{
	return drag_scale;
}

void SpriteObject::set_drag_scale(double s)
{
	drag_scale = s;
}

BlendMode SpriteObject::get_drag_blend() const
{
	return drag_blend;
}

void SpriteObject::set_drag_blend(BlendMode blend)
{
	drag_blend = blend;
}

int SpriteObject::get_last_drag_time() const
{
	return last_drag_time;
}

void SpriteObject::on_create()
{
	PhysicObject::on_create();

}

void SpriteObject::on_update()
{
	PhysicObject::on_update();

	sprite().update();

	if (drag_cnt && t() - last_drag_time >= drag_interval)
	{
		Particle::Data p;
		p.sprite = d_sprite;
		p.sprite.set_blend(drag_blend);
		p.sprite.pause();
		p.sprite.set_alpha(p.sprite.alpha() * drag_alpha);
		p.sprite.set_scale(p.sprite.w_scale() * drag_scale, p.sprite.h_scale() * drag_scale);
		p.cnt = 1;
		p.rot = rot();
		p.lifeTime = (double)drag_interval * drag_cnt;
		if (drag_fade)		p.set_option(Particle::Options::fade_out);
		if (drag_shrink)	p.set_option(Particle::Options::shrink);
		ParticleSystem::instance()->create_particle(p, this);

		last_drag_time = t();
	}
}

void SpriteObject::on_destroy()
{

	PhysicObject::on_destroy();
}

void SpriteObject::on_draw()
{
	PhysicObject::on_draw();

	DrawSystem::instance()->draw_sprite(sprite(), x(), y(), rot());
}