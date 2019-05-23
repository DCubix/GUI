#ifndef SYG_GUI_H
#define SYG_GUI_H

#include <memory>
#include <unordered_map>

#include "events.h"
#include "image.h"
#include "renderer.h"

#include "widgets/panel.h"

#include "pugixml/pugixml.hpp"

class Spinner;
class GUI {
public:
	GUI();

	void load(const std::string& xmlData);

	template <typename T, typename... Args>
	T* create(Args&&... args) {
		T* widget = new T(std::forward<Args>(args)...);
		widget->m_gui = this;
		m_widgets.push_back(std::unique_ptr<T>(widget));
		m_events->subscribe(m_widgets.back().get());
		return dynamic_cast<T*>(m_widgets.back().get());
	}

	Image* createImage(int width, int height);
	Image* createImage(const std::string& fileName);

	Spinner* spinner(
		float* value, float vmin, float vmax,
		const std::string& suffix = "", 
		bool drag = true,
		const std::function<void()> onChange = nullptr,
		float step = 0.1f
	);

	template <class T>
	T* get(const std::string& name) {
		for (auto& w : m_widgets) {
			if (w->name() == name) {
				return dynamic_cast<T*>(w.get());
			}
		}
		return nullptr;
	}

	void destroy(Widget* widget);

	EventHandler* events() { return m_events.get(); }
	Renderer& renderer() { return *m_renderer.get(); }

	Panel* root() { return m_root; }

	void render(int width, int height);
	void clear() { m_clear = true; }

private:
	Panel* m_root;
	std::vector<std::unique_ptr<Widget>> m_widgets;
	std::unique_ptr<EventHandler> m_events;
	std::unique_ptr<Renderer> m_renderer;
	std::vector<std::unique_ptr<Image>> m_images;
	bool m_clear{ true };
};

#endif // SYG_GUI_H
