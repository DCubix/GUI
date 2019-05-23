#include "gui.h"

#include <algorithm>

#include "sdl_headers.h"
#include "widgets/spinner.h"

GUI::GUI() {
	m_events = std::make_unique<EventHandler>();
	m_renderer = std::make_unique<Renderer>();

	m_root = create<Panel>();
	m_root->gridWidth(16);
	m_root->gridHeight(16);
	m_root->spacing(4);
	m_root->padding(2);
}

Spinner* GUI::spinner(
	float* value, float vmin, float vmax,
	const std::string& suffix,
	bool drag,
	const std::function<void()> onChange,
	float step
) {
	Spinner* spn = create<Spinner>();
	spn->m_userData = value;
	spn->minimum(vmin);
	spn->maximum(vmax);
	spn->draggable(drag);
	spn->suffix(suffix);
	spn->step(step);
	spn->value(*value);
	spn->onChange([=](float val) {
		float* _value = spn->userData();
		(*_value) = val;
		if (onChange) onChange();
	});
	return spn;
}

void GUI::destroy(Widget* widget) {
	auto it = std::remove_if(
		m_widgets.begin(),
		m_widgets.end(),
		[&](std::unique_ptr<Widget> const& p) {
			return p.get() == widget;
		});
	if (it != m_widgets.end()) {
		it->reset();
		m_widgets.erase(it, m_widgets.end());
	}

	for (auto&& widget : m_widgets) {
		widget->invalidate();
	}
	m_clear = true;
}

void GUI::render(int width, int height) {
	m_root->bounds().width = width;
	m_root->bounds().height = height;

	// Clean orphans (except main panel)
	std::vector<int> orphans;
	for (int i = 1; i < m_widgets.size(); i++) {
		if (m_widgets[i]->parent() == nullptr) {
			orphans.push_back(i);
		}
	}
	std::reverse(orphans.begin(), orphans.end());
	for (int i : orphans) {
		m_widgets.erase(m_widgets.begin() + i);
	}
	//

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_renderer->begin(width, height);

	for (auto&& widget : m_widgets) {
		auto b = widget->bounds();
		if (widget->parent() == nullptr) {
			m_renderer->pushClipping(b.x, b.y, b.width, b.height);
		}

		if (widget->visible()) {
			widget->onDraw(*m_renderer.get());
		}
		if (widget->parent() == nullptr) {
			m_renderer->popClipping();
		}
	}

	m_renderer->end();
}
