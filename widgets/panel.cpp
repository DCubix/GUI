#include "panel.h"

#include "../gui.h"

Panel::Panel() {
}

void Panel::onDraw(Renderer& renderer) {
	performLayout();

	auto b = realBounds();

	if (m_drawBackground) renderer.panel(b.x, b.y, b.width, b.height);
	renderer.pushClipping(
		b.x + m_padding, b.y + m_padding, 
		b.width - m_padding * 2, b.height - m_padding * 2
	);
	for (auto&& w : m_children) {
		if (!w) continue;
		if (!w->visible()) continue;
		w->onDraw(renderer);
	}
	renderer.popClipping();
}

void Panel::add(Widget* widget) {
	widget->m_parent = this;
	m_children.push_back(widget);
}

void Panel::remove(Widget* widget) {
	auto it = std::find(m_children.begin(), m_children.end(), widget);
	if (it != m_children.end()) {
		(*it)->m_parent = nullptr;
		m_gui->events()->unsubscribe(*it);
		m_children.erase(it);
	}
}

void Panel::removeAll() {
	for (Widget* w : m_children) {
		w->m_parent = nullptr;
		m_gui->events()->unsubscribe(w);
	}
	m_children.clear();
}

Element::Size Panel::preferredSize() {
	if (m_autoSize) {
		int contentWidth = 0;
		int contentHeight = 0;
		for (auto&& w : m_children) {
			if (!w) continue;
			if (!w->visible()) continue;
			auto b = w->bounds();
			contentWidth = std::max(contentWidth, b.x + b.width);
			contentHeight = std::max(contentHeight, b.y + b.height);
		}
		return { contentWidth, contentHeight };
	}
	return Widget::preferredSize();
}

void Panel::setLayout(Layout* layout) {
	if (m_layout && layout) {
		m_layout.reset(layout);
	} else if (layout) {
		m_layout = std::unique_ptr<Layout>(layout);
	}
}

void Panel::performLayout() {
	if (m_layout) {
		m_layout->apply(this, m_children);
	}
}
