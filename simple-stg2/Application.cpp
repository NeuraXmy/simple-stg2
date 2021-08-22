#include <chrono>
#include <ratio>
#include <ctime>
#include <Windows.h>
#include <iostream>

#include "Application.h"


namespace
{
	using nano = long long;

	double to_sec(nano x) 
	{
		return double(x) / 1e9;
	}
	double to_msec(nano x)
	{
		return double(x) / 1e6;
	}
	nano to_nano(double sec)
	{
		return sec * 1e9;
	}

	nano get_time()
	{
		static nano frequency = 0;
		if (!frequency)
		{
			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			frequency = freq.QuadPart;
		}
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return time.QuadPart * to_nano(1) / frequency;

		// return std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}
}


Application::Application(
	int frame_rate_limit,
	int lost_tolerance,
	int frame_rate_when_lost) :
	paused(),
	exited(),
	frame_count(),
	next_update_time(),
	last_update_time(),
	lost_time(),
	frame_time(),
	start_time()
{
	set_frame_rate_limit(frame_rate_limit);
	set_lost_tolerance(lost_tolerance);
	set_frame_rate_when_lost(frame_rate_when_lost);
}


int Application::get_frame_rate_limit() const
{
	return to_nano(1) / update_period;
}

void Application::set_frame_rate_limit(int limit)
{
	update_period = limit ? to_nano(1) / limit : 0;
}

int Application::get_lost_tolerance() const
{
	return lost_tolerance;
}

void Application::set_lost_tolerance(int frame)
{
	lost_tolerance = frame;
}

int Application::get_frame_rate_when_lost() const
{
	return to_nano(1) / update_period_when_lost;
}

void Application::set_frame_rate_when_lost(int limit)
{
	update_period_when_lost = limit ? to_nano(1) / limit : 0;
}


double Application::get_lost_time() const
{
	return to_sec(lost_time);
}

double Application::get_frame_rate() const
{
	return to_nano(1) / double(frame_time);
}

int Application::get_frame_count() const
{
	return frame_count;
}


void Application::start()
{
	exited = paused = false;
	frame_time = 0;
	frame_count = 0;

	on_start();

	nano current = get_time();
	start_time = last_update_time = current;
	next_update_time = current + update_period;

	while (!exited)
	{
		if (paused)
		{
			continue;
		}
	
		on_update();

		frame_count++;

		current = get_time();

		if (!update_period)
		{
			next_update_time = current + update_period;
		}
		else
		{
			if (current >= next_update_time && lost_tolerance)
			{
				lost_time = current - next_update_time;
				
				if (current - next_update_time > lost_tolerance)
				{
					next_update_time = current + update_period;
				}
				else
				{
					current = get_time();
					while (current < last_update_time + update_period_when_lost)
						current = get_time();

					next_update_time += update_period;
				}
			}
			else
			{
				lost_time = 0;

				current = get_time();
				while (current < next_update_time)
					current = get_time();

				next_update_time += update_period;
			}
		}

		frame_time = current - last_update_time;
		last_update_time = current;
	}

	on_exit();
}

void Application::pause()
{
	paused = true;
}

void Application::resume()
{
	paused = false;
}

void Application::exit()
{
	exited = true;
}

bool Application::is_paused() const
{
	return paused;
}


void Application::on_start()
{

}

void Application::on_update()
{

}

void Application::on_exit()
{

}


