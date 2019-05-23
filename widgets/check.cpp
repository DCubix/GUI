#include "check.h"

void Check::onClick(int button, int x, int y) {
	m_checked = !m_checked;
	if (m_onChecked) m_onChecked(m_checked);
	invalidate();
	Widget::onClick(button, x, y);
}

void Check::onDraw(Renderer& renderer) {
	auto b = realBounds();
	renderer.pushClipping(b.x, b.y, b.width, b.height);
	renderer.check(b.x, b.y + (b.height / 2 - 8), m_checked);
	renderer.text(b.x + 19, b.y + (b.height / 2 - 6) + 1, m_text, 0, 0, 0, 128);
	renderer.text(b.x + 18, b.y + (b.height / 2 - 6), m_text, 255, 255, 255, 180);
	renderer.popClipping();
}
