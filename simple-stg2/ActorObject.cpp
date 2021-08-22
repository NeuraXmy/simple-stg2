#include "ActorObject.h"

int ActorObject::current_act_state() const
{
    return current_state;
}

void ActorObject::set_act_state(int state)
{
    if (!is_vaild_act_state(state))
    {
        // todo...
        return;
    }

    if (current_state == state)
    {
        return;
    }

    current_state = state;
    sprite().set_frame(state_mapping[state]);
    last_change_time = t();
    on_change_act_state(state);
}

int ActorObject::create_act_state(int frame_index)
{
    state_mapping.push_back(frame_index);
    return state_mapping.size() - 1;
}

int ActorObject::get_act_frame_index(int state) const
{
    return is_vaild_act_state(state) ? state_mapping[state] : -1;
}

void ActorObject::set_act_frame_index(int state, int index)
{
    if (!is_vaild_act_state(state))
    {
        // todo...
        return;
    }

    state_mapping[state] = index;

    if (current_act_state() == state)
    {
        sprite().set_frame(state_mapping[state]);
    }
}

bool ActorObject::is_vaild_act_state(int state) const
{
    return state < state_mapping.size() && state >= 0;
}

void ActorObject::clear_act_state()
{
    state_mapping.clear();
}

int ActorObject::last_change_act_state_time() const
{
    return last_change_time;
}


void ActorObject::on_create()
{
    SpriteObject::on_create();

}

void ActorObject::on_update()
{
    SpriteObject::on_update();

}

void ActorObject::on_destroy()
{

    SpriteObject::on_destroy();
}

void ActorObject::on_draw()
{
    SpriteObject::on_draw();

}

void ActorObject::on_change_act_state(int state)
{

}
