#ifndef SYG_APPLICATION_H
#define SYG_APPLICATION_H

#include "sdl_headers.h"
#include "gui.h"

class Application {
public:
	Application() = default;
	virtual ~Application() = default;

	int run(const std::string& title = "Application", int width = 640, int height = 480);

	virtual void onBuild(GUI* gui) {}
	virtual void onTick(GUI* gui, float ts) {}
	virtual void onExit(GUI* gui) { quit(); }

	void quit() { m_running = false; }

private:
	bool m_running;
	SDL_Window* m_window;
	SDL_GLContext m_context;

	std::unique_ptr<GUI> m_gui;
};

#endif // SYG_APPLICATION_H
