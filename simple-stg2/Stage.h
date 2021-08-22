#pragma once

#include "GameObject.h"

class Background;


class Stage : public GameObject
{
public:

	virtual ~Stage() = default;


	void start();

	bool is_started() const;

	void end();

	bool is_ended() const;


	template<class Bg, class ... Args>
	Background* set_background(Args ... args)
	{
		remove_background();
		bg = new Bg(args...);
		return bg;
	}

	Background* background() const;

	void remove_background();


protected:

	virtual void on_create() override;

	virtual void on_update() override;

	virtual void on_destroy() override;

	virtual void on_draw() override;


	virtual void on_start();

	virtual void on_end();


private:

	bool started = false;

	bool ended = false;

	Background* bg = nullptr;

};

