#pragma once

#include <functional>
#include <queue>
#include <vector>


class GameObject
{
	friend class ObjectSystem;

public:

	using Task = std::function<void()>;


	GameObject() = default;

	virtual ~GameObject() = default;


	int t() const;

	bool to_be_destroy() const;

	void create();

	void destroy();

	void update();

	void draw();

	void draw_ui();


	void pause_update();

	void resume_update();

	bool is_update_paused() const;


	void add_task(const Task& task, int delay);

	void clear_task();

protected:

	virtual void on_create();

	virtual void on_update();

	virtual void on_destroy();

	virtual void on_draw();

	virtual void on_draw_ui();

private:

	int update_t = 0;

	bool destroyed = false;

	bool update_paused = false;


	struct TaskWithTime
	{
		int time;
		Task task;
		bool operator<(const TaskWithTime& t) const 
		{
			return time > t.time;
		}
	};

	std::priority_queue<TaskWithTime, std::vector<TaskWithTime>> tasks;

};

