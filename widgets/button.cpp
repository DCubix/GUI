#include "button.h"

Button::Button() {
	bounds().width = 100;
	bounds().height = 24;
}

void Button::onDraw(Renderer& renderer) {
	if (!enabled()) m_state = State::Disabled;

	m_textWidth = renderer.textWidth(m_text) + 1;

	auto b = realBounds();
	renderer.button(b.x, b.y, b.width, b.height, int(m_state));

	renderer.pushClipping(b.x, b.y, b.width, b.height);
	renderer.text(
		b.x + (b.width / 2 - m_textWidth / 2) + 1, b.y + (b.height / 2 - 6) + 1,
		m_text,
		0, 0, 0, 128
	);
	renderer.text(
		b.x + (b.width / 2 - m_textWidth / 2), b.y + (b.height / 2 - 6),
		m_text,
		255, 255, 255, 180
	);
	renderer.popClipping();
}

void Button::onPress(int button, int x, int y) {
	Widget::onPress(button, x, y);
	m_state = State::Click;
}

void Button::onRelease(int button, int x, int y) {
	Widget::onRelease(button, x, y);
	m_state = State::Normal;
}

void Button::onEnter() {
	Widget::onEnter();
	m_state = State::Hover;
}

void Button::onExit() {
	Widget::onExit();
	m_state = State::Normal;
}

Element::Size Button::preferredSize() {
	if (m_autoSize) {
		return { m_textWidth + 12, m_bounds.height };
	}
	return Widget::preferredSize();
}
