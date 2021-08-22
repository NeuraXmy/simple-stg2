#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class WindowSystem final
{
public:

	static WindowSystem* instance();

	static void release_instance();


	void open(const std::string& title, int w, int h, bool full_screen = false);

	void close();
	
	void resize(int w, int h);

	void set_title(const std::string& title);

	int get_w() const;

	int get_h() const;

	void set_vsync(bool flag);

	void set_focus();


	void update();


	sf::RenderWindow* get_window();

	const sf::RenderWindow* get_window() const;


	bool is_open() const;

	bool is_closed() const;


private:

	sf::RenderWindow* window;


	WindowSystem();

	~WindowSystem();

};

