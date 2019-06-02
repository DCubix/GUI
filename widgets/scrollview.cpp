#include "scrollview.h"

#include "../gui.h"

ScrollView::ScrollView() {
	m_hscroll = new Scroll();
	m_hscroll->step(2.0f);
	m_hscroll->orientation(Scroll::Horizontal);
	m_hscroll->bounds().height = 16;
	m_hscroll->visible(false);
	m_hscroll->m_parent = this;
	m_hscroll->m_gui = nullptr;

	m_vscroll = new Scroll();
	m_vscroll->step(4.0f);
	m_vscroll->orientation(Scroll::Vertical);
	m_vscroll->bounds().width = 16;
	m_vscroll->visible(false);
	m_vscroll->m_parent = this;
	m_vscroll->m_gui = nullptr;
}

ScrollView::~ScrollView() {
	if (m_vscroll) {
		if (m_gui->events()) m_gui->events()->unsubscribe(m_vscroll);
		delete m_vscroll;
		m_vscroll = nullptr;
	}
	if (m_hscroll) {
		if (m_gui->events()) m_gui->events()->unsubscribe(m_hscroll);
		delete m_hscroll;
		m_hscroll = nullptr;
	}
}

void ScrollView::onDraw(Renderer& renderer) {
	if (m_widget == nullptr) return;

	m_widget->autoSize(true);

	auto sz = m_widget->preferredSize();
	auto b = realBounds();

	renderer.panel(b.x, b.y, b.width - 16, b.height - 16);

	if (m_hscroll->m_gui == nullptr) {
		m_hscroll->m_gui = m_gui;
		m_gui->events()->subscribe(m_hscroll);
		m_vscroll->m_gui = m_gui;
		m_gui->events()->subscribe(m_vscroll);
	}

	m_hscroll->visible(sz.width > (b.width - 16));
	m_vscroll->visible(sz.height > (b.height - 16));

	if (m_hscroll->visible()) {
		m_hscroll->minimum(0.0f);
		m_hscroll->maximum(sz.width - (b.width - 16));
		m_hscroll->bounds().x = 0;
		m_hscroll->bounds().y = (b.height - 16);
		m_hscroll->bounds().width = b.width - 16;
		m_hscroll->onDraw(renderer);
		m_widget->bounds().x = -m_hscroll->value();
	}

	if (m_vscroll->visible()) {
		m_vscroll->minimum(0.0f);
		m_vscroll->maximum(sz.height - (b.height - 16));
		m_vscroll->bounds().x = (b.width - 16);
		m_vscroll->bounds().y = 0;
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
