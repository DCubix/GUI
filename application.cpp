#include "application.h"

int Application::run(const std::string& title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("%s", SDL_GetError());
		return 1;
	}

	m_window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
	);
	if (m_window == nullptr) {
		printf("%s", SDL_GetError());
		return 2;
	}

	m_context = SDL_GL_CreateContext(m_window);
	if (m_context == nullptr) {
		printf("%s", SDL_GetError());
		return 3;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetSwapInterval(0);

	m_gui = std::make_unique<GUI>();
	onBuild(m_gui.get());

	const double timeStep = 1.0 / 30.0;
	double lastTime = double(SDL_GetTicks()) / 1000.0;
	double accum = 0.0;

	SDL_StartTextInput();

	m_running = true;
	while (m_running) {
		bool canRender = false;
		double current = double(SDL_GetTicks()) / 1000.0;
		double delta = current - lastTime;
		lastTime = current;
		accum += delta;

		switch (m_gui->events()->poll()) {
			case EventHandler::Status::Quit: onExit(m_gui.get()); break;
			case EventHandler::Status::Resize: m_gui->clear(); break;
			default: break;
		}

		while (accum > timeStep) {
			accum -= timeStep;
			canRender = true;
		}

		if (canRender) {
			int w, h;
			SDL_GetWindowSize(m_window, &w, &h);
			m_gui->render(w, h);
			SDL_GL_SwapWindow(m_window);
		}
	}

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

	return 0;
}
