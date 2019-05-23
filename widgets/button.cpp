#include "button.h"

Button::Button() {
	bounds().width = 100;
	bounds().height = 24;
}

void Button::onDraw(Renderer& renderer) {
	if (!enabled()) m_state = State::Disabled;

	auto b = realBounds();
	renderer.button(b.x, b.y, b.width, b.height, int(m_state));

	renderer.pushClipping(b.x, b.y, b.width, b.height);
	renderer.text(
		b.x + (b.width / 2 - renderer.textWidth(m_text) / 2) + 1, b.y + (b.height / 2 - 6) + 1,
		m_text,
		0, 0, 0, 128
	);
	renderer.text(
		b.x + (b.width / 2 - renderer.textWidth(m_text) / 2), b.y + (b.height / 2 - 6),
		m_text,
		255, 255, 255, 180
	);
	renderer.popClipping();
}

void Button::onPress(int button, int x, int y) {
	Widget::onPress(button, x, y);

	m_state = State::Click;
	invalidate();
}

void Button::onRelease(int button, int x, int y) {
	Widget::onRelease(button, x, y);

	m_state = State::Normal;
	invalidate();
}

void Button::onEnter() {
	Widget::onEnter();

	m_state = State::Hover;
	invalidate();
}

void Button::onExit() {
	Widget::onExit();

	m_state = State::Normal;
	invalidate();
}
