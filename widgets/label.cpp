#include "label.h"

#include "panel.h"

void Label::onDraw(Renderer& renderer) {
	m_textWidth = renderer.textWidth(m_text) + 1;
	bounds().height = 20;

	auto b = realBounds();
	int tx = 0;
	switch (m_textAlign) {
		case Alignment::Center: tx = b.width / 2 - m_textWidth / 2; break;
		case Alignment::Right: tx = b.width - m_textWidth; break;
		default: break;
	}

	renderer.pushClipping(b.x, b.y, b.width, b.height);

	Panel* parent = getValidParent();
	if (parent) {
		auto pb = parent->realBounds();
		renderer.panel(pb.x, pb.y, pb.width, pb.height);
	}

	renderer.text(b.x + 1 + tx, b.y + (b.height / 2 - 6) + 1, m_text, 0, 0, 0, 128);
	renderer.text(b.x + tx, b.y + (b.height / 2 - 6), m_text, 255, 255, 255, 180);

	renderer.popClipping();
}

Element::Size Label::preferredSize() {
	if (m_autoSize) {
		return { m_textWidth, 20 };
	}
	return Widget::preferredSize();
}
