#pragma once

#include "PhysicObject.h"
#include "Shooter.h"
#include "ObjectSystem.h"


class DroneSet : public PhysicObject
{
public:

	virtual ~DroneSet() = default;


	int get_mode() const;

	void set_mode(int mode);

	
	template<class Drone, class ... Args>
	Drone* append_drone(Args ... args)
	{
		auto d = ObjectSystem::instance()->create_object<Drone>(args...);
		drones.push_back(d);
		d->bind(this);
		return d;
	}

	int get_drone_count();

	Shooter* drone(int index);

	void remove_drone(int index);

	void remove_all();


	void stop_all();

	void start_all();

	void show_all();

	void hide_all();


	int get_last_change_mode_t() const;

	int get_change_mode_time() const;

	void set_change_mode_time(int time);


	int get_lat_show_hide_time() const;

	int get_show_hide_time() const;

	void set_show_hide_time(int time);

	bool is_all_show() const;

	bool is_all_hide() const;

	
	template<class Func>
	void foreach(Func&& func) 
	{
		for (auto d : drones)
			func(d);
	}


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;


	virtual void on_change_mode();

	virtual void on_changing_mode(double process);

	virtual void on_show_hide(double process);

	virtual void on_show();

	virtual void on_hide();

private:

	int mode = 0;

	int last_change_mode_time = 0;

	int change_mode_time = 10;

	int last_show_hide_time = 0;

	int show_hide_time = 10;

	std::vector<Shooter*> drones;

};

