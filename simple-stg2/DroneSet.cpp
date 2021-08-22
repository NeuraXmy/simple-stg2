#include "DroneSet.h"

int DroneSet::get_mode() const
{
    return mode;
}

void DroneSet::set_mode(int mode)
{
    int last = this->mode;
    this->mode = mode;
    if (mode != last)
    {
        last_change_mode_time = t();
        on_change_mode();
    }
}


int DroneSet::get_drone_count()
{
    return drones.size();
}

Shooter* DroneSet::drone(int index)
{
    return index >= 0 && index < drones.size() ? drones[index] : nullptr;
}

void DroneSet::remove_drone(int index)
{
    drones[index]->stop();
    drones[index]->destroy();
    drones.erase(drones.begin() + index);
}

void DroneSet::remove_all()
{
    for (auto d : drones)
    {
        d->stop();
        d->destroy();
    }
    drones.clear();
}


void DroneSet::stop_all()
{
    for (auto d : drones)
        d->stop();
}

void DroneSet::start_all()
{
    for (auto d : drones)
        d->start();
}

void DroneSet::show_all()
{
    if (!is_all_show())
    {
        for (auto d : drones)
            d->sprite().set_visible(true);
        last_show_hide_time = t();
        on_show();
        on_show_hide(0);
    }
}

void DroneSet::hide_all()
{
    if (!is_all_hide())
    {
        for (auto d : drones)
            d->sprite().set_visible(false);
        last_show_hide_time = t();
        on_hide();
        on_show_hide(0);
    }
}

int DroneSet::get_last_change_mode_t() const
{
    return last_change_mode_time;
}

int DroneSet::get_change_mode_time() const
{
    return change_mode_time;
}

void DroneSet::set_change_mode_time(int time)
{
    change_mode_time = time;
}

int DroneSet::get_lat_show_hide_time() const
{
    return last_show_hide_time;
}

int DroneSet::get_show_hide_time() const
{
    return show_hide_time;
}

void DroneSet::set_show_hide_time(int time)
{
    show_hide_time = time;
}

bool DroneSet::is_all_show() const
{
    for (auto d : drones)
        if (!d->sprite().visible())
            return false;
    return true;
}

bool DroneSet::is_all_hide() const
{
    for (auto d : drones)
        if (d->sprite().visible())
            return false;
    return true;
}


void DroneSet::on_create()
{
    PhysicObject::on_create();

}

void DroneSet::on_update()
{
    PhysicObject::on_update();

    if (t() - last_change_mode_time <= change_mode_time)
    {
        on_changing_mode(double(t() - last_change_mode_time) / change_mode_time);
    }

    if (t() - last_show_hide_time <= show_hide_time)
    {
        on_show_hide(double(t() - last_show_hide_time) / show_hide_time);
    }
}

void DroneSet::on_destroy()
{
    PhysicObject::on_destroy();

}

void DroneSet::on_draw()
{
    PhysicObject::on_draw();

}


void DroneSet::on_change_mode()
{

}

void DroneSet::on_changing_mode(double process)
{
}

void DroneSet::on_show_hide(double process)
{

}

void DroneSet::on_show()
{
}

void DroneSet::on_hide()
{
}
