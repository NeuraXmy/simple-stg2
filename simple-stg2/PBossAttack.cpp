#include "PBossAttack.h"
#include "PResource.h"

void SpellCard::on_create()
{
	BossAttack::on_create();
	set_is_spellcard(true);
	set_max_health(2000);
	set_time_limit(600);
	set_prepare_time(120);

	set_beaten_se(se.enemy_kill_big);
	set_start_se(se.cast);
	set_timeout_se(se.timeout);

	spellcard_stripe().set_tex_and_coords(tex.sc_stripe, coords.sc_stripe);
	spellcard_stripe().set_scale(10.0, 0.5);
	spellcard_stripe().set_tex_scale(20.0, 1.0);

	title_sprite().set_tex_and_coords(tex.sc_title, coords.sc_title);
	title_sprite().set_scale(0.6, 0.6);

	set_charge_before_start(true);
}

void NormalAttack::on_create()
{
	BossAttack::on_create();
	set_is_spellcard(false);
	set_max_health(2000);
	set_time_limit(600);
	set_prepare_time(60);

	set_beaten_se(se.enemy_kill_big);
	set_timeout_se(se.timeout);

	spellcard_stripe().set_tex_and_coords(tex.sc_stripe, coords.sc_stripe);
	spellcard_stripe().set_scale(10.0, 0.5);
	spellcard_stripe().set_tex_scale(20.0, 1.0);

	title_sprite().set_tex_and_coords(tex.sc_title, coords.sc_title);
	title_sprite().set_scale(0.6, 0.6);

	set_charge_before_start(false);
}