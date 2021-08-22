#pragma once

class GamePanel
{
public:

	virtual ~GamePanel() = default;


	bool is_visible() const;

	void set_visible(bool flag);


	void draw();

protected:

	virtual void on_draw();

private:

	bool visible = true;

};

