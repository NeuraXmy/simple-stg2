#pragma once


/**
 * 应用程序类型 接口时间单位为秒
 * frame_rate_limit 为 0 则不限制帧数 
 * lost_tolerance 为 0 则关闭帧追击机制
 */
class Application
{
public:

	Application(
		int frame_rate_limit,
		int lost_tolerance,
		int frame_rate_when_lost
	);

	virtual ~Application() = default;


	int get_frame_rate_limit() const;

	void set_frame_rate_limit(int limit);

	int get_lost_tolerance() const;

	void set_lost_tolerance(int frame);

	int get_frame_rate_when_lost() const;

	void set_frame_rate_when_lost(int limit);


	double get_lost_time() const;

	double get_frame_rate() const;

	int get_frame_count() const;


	void start();

	void pause();

	void resume();

	void exit();

	bool is_paused() const;

protected:

	virtual void on_start();

	virtual void on_update();

	virtual void on_exit();

private:

	using nano = long long;

	nano update_period;
	int lost_tolerance;
	nano update_period_when_lost;

	nano next_update_time;
	nano last_update_time;

	nano frame_time;
	nano start_time;
	nano lost_time;

	int frame_count;

	bool paused;
	bool exited;

};