#include "GameObject.h"

int GameObject::t() const
{
    return update_t;
}

bool GameObject::to_be_destroy() const
{
    return destroyed;
}

void GameObject::create()
{
    on_create();
}

void GameObject::destroy()
{
    if (!destroyed)
    {
        destroyed = true;

        on_destroy();
    }
}

void GameObject::update()
{
    if (update_paused)
    {
        return;
    }

    on_update();
    
    update_t++;
}

void GameObject::draw()
{
    on_draw();
}

void GameObject::draw_ui()
{
    on_draw_ui();
}

void GameObject::pause_update()
{
    update_paused = true;
}

void GameObject::resume_update()
{
    update_paused = false;
}

bool GameObject::is_update_paused() const
{
    return update_paused;
}


void GameObject::add_task(const Task& task, int delay)
{
    TaskWithTime twt;
    twt.task = task;
    twt.time = t() + delay;
    tasks.push(twt);
}

void GameObject::clear_task()
{
    while (tasks.size())
        tasks.pop();
}


void GameObject::on_create()
{

}

void GameObject::on_update()
{
    while (tasks.size())
    {
        if (t() < tasks.top().time)
        {
            break;
        }
        tasks.top().task();
        tasks.pop();
    }
}

void GameObject::on_destroy()
{

}

void GameObject::on_draw()
{

}

void GameObject::on_draw_ui()
{
}
