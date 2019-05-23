#include "imageview.h"

void ImageView::onDraw(Renderer& renderer) {
	auto b = realBounds();

	renderer.panel(b.x, b.y, b.width, b.height);
	renderer.pushClipping(
		b.x + 2, b.y + 2,
		b.width - 4, b.height - 4
	);
	renderer.rect(
		b.x + 2, b.y + 2,
		b.width - 4, b.height - 4,
		0, 0, 0, 60, true
	);
	if (m_image != nullptr) {
		const float ratio = std::min(
					float(b.width) / float(m_image->width()),
					float(b.height) / float(m_image->height())
		);
		const float w = float(m_image->width()) * ratio;
		const float h = float(m_image->height()) * ratio;
		renderer.image(
					m_image->id(),
					b.x + int(b.width / 2 - w / 2),
					b.y + int(b.height / 2 - h / 2),
					int(w), int(h),
					0, 0, m_image->width(), m_image->height()
		);
	}
	renderer.popClipping();
}
