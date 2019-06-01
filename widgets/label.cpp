#include "label.h"

#include "panel.h"

void Label::onDraw(Renderer& renderer) {
	m_textWidth = renderer.textWidth(m_text) + 1;

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

	renderer.text(b.x + tx, b.y + 1, m_text, 0, 0, 0, 128, m_wordWrap, b.x + b.width);
	renderer.text(b.x + tx, b.y, m_text, 255, 255, 255, 180, m_wordWrap, b.x + b.width);

	renderer.popClipping();

	//renderer.rect(b.x, b.y, b.width, b.height, 255, 0, 0);
}

Element::Size Label::preferredSize() {
	if (m_autoSize) {
		return { m_textWidth, bounds().height };
	}
	return Widget::preferredSize();
}
