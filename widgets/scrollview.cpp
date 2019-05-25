#include "scrollview.h"

#include "../gui.h"

ScrollView::ScrollView() {
	m_hscroll = std::make_unique<Scroll>();
	m_hscroll->step(2.0f);
	m_hscroll->orientation(Scroll::Horizontal);
	m_hscroll->bounds().height = 16;
	m_hscroll->m_gui = nullptr;
	m_hscroll->visible(false);

	m_vscroll = std::make_unique<Scroll>();
	m_vscroll->step(4.0f);
	m_vscroll->orientation(Scroll::Vertical);
	m_vscroll->bounds().width = 16;
	m_vscroll->m_gui = nullptr;
	m_vscroll->visible(false);
}

void ScrollView::onDraw(Renderer& renderer) {
	if (m_widget == nullptr) return;

	m_widget->autoSize(true);

	auto sz = m_widget->preferredSize();
	auto b = realBounds();
	m_hscroll->visible(sz.width > (b.width - 16));
	m_vscroll->visible(sz.height > (b.height - 16));

	renderer.panel(b.x, b.y, b.width - 16, b.height - 16);

	if (m_hscroll->m_gui == nullptr) {
		m_hscroll->m_gui = m_gui;
		m_vscroll->m_gui = m_gui;
		m_gui->events()->subscribe(m_hscroll.get());
		m_gui->events()->subscribe(m_vscroll.get());
	}

	if (m_hscroll->visible()) {
		m_hscroll->minimum(0.0f);
		m_hscroll->maximum(sz.width - (b.width - 16));
		m_hscroll->bounds().x = b.x;
		m_hscroll->bounds().y = b.y + (b.height - 16);
		m_hscroll->bounds().width = b.width - 16;
		m_hscroll->onDraw(renderer);
		m_widget->bounds().x = -m_hscroll->value();
	}

	if (m_vscroll->visible()) {
		m_vscroll->minimum(0.0f);
		m_vscroll->maximum(sz.height - (b.height - 16));
		m_vscroll->bounds().x = b.x + (b.width - 16);
		m_vscroll->bounds().y = b.y;
		m_vscroll->bounds().height = b.height - 16;
		m_vscroll->onDraw(renderer);
		m_widget->bounds().y = -m_vscroll->value();
	}

	renderer.pushClipping(b.x, b.y, b.width-16, b.height-16);
	if (m_widget->visible()) {
		m_widget->onDraw(renderer);
	}
	renderer.popClipping();
}

void ScrollView::widget(Widget* w) {
	if (w != nullptr) {
		w->m_parent = this;
		m_widget = w;
	}
}