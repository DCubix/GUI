#include "imageview.h"

void ImageView::onDraw(Renderer& renderer) {
	auto b = realBounds();
	auto sz = preferredSize();

	renderer.panel(b.x, b.y, b.width, b.height);
	if (m_image != nullptr) {
		const float ratio = std::min(
					float(sz.width) / float(m_image->width()),
					float(sz.height) / float(m_image->height())
		);
		const float w = float(m_image->width()) * ratio;
		const float h = float(m_image->height()) * ratio;
		renderer.image(
					m_image->id(),
					b.x + int(sz.width / 2 - w / 2),
					b.y + int(sz.height / 2 - h / 2),
					int(w), int(h),
					0, 0, m_image->width(), m_image->height()
		);
	}
}

Element::Size ImageView::preferredSize() {
	if (m_autoSize && m_image != nullptr) {
		return { m_image->width(), m_image->height() };
	}
	return Widget::preferredSize();
}
