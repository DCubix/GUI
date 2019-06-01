#include "splitview.h"

static bool hitsR(int x, int y, int bx, int by, int bw, int bh) {
	return x > bx&&
		x < bx + bw &&
		y > by &&
		y < by + bh;
}

void SplitView::onDraw(Renderer& renderer) {

	auto b = realBounds();
	if (m_orientation == Horizontal) {
		if (m_moving) {
			renderer.rect(b.x, b.y + m_dividerLocation - DividerSize/2, b.width, DividerSize, 0, 0, 0, 128, true);
		}
	} else {
		if (m_moving) {
			renderer.rect(b.x + m_dividerLocation - DividerSize/2, b.y, DividerSize, b.height, 0, 0, 0, 128, true);
		}
	}

	if (m_first != nullptr) {
		auto&& wb = m_first->bounds();

		if (m_orientation == Horizontal) {
			wb.x = 0;
			wb.y = 0;
			wb.width = b.width;
			wb.height = m_dividerLocation - DividerSize/2;
		} else {
			wb.x = 0;
			wb.y = 0;
			wb.width = m_dividerLocation - DividerSize/2;
			wb.height = b.height;
		}

		if (m_first->visible()) {
			renderer.pushClipping(wb.x-1, wb.y-1, wb.width+2, wb.height+2);
			m_first->onDraw(renderer);
			renderer.popClipping();
		}
	}

	if (m_second != nullptr) {
		auto&& wb = m_second->bounds();

		if (m_orientation == Horizontal) {
			wb.x = 0;
			wb.y = m_dividerLocation + DividerSize/2;
			wb.width = b.width;
			wb.height = (b.height - m_dividerLocation) - DividerSize/2;
		} else {
			wb.x = m_dividerLocation + DividerSize/2;
			wb.y = 0;
			wb.width = (b.width - m_dividerLocation) - DividerSize/2;
			wb.height = b.height;
		}

		if (m_second->visible()) {
			renderer.pushClipping(wb.x-1, wb.y-1, wb.width+2, wb.height+2);
			m_second->onDraw(renderer);
			renderer.popClipping();
		}
	}
}

void SplitView::onPress(int button, int x, int y) {
	auto b = realBounds();
	if (m_orientation == Horizontal) {
		if (hitsR(x, y, 0, m_dividerLocation - DividerSize/2, b.width, DividerSize)) {
			m_moving = true;
		}
	} else {
		if (hitsR(x, y, m_dividerLocation - DividerSize/2, 0, DividerSize, b.height)) {
			m_moving = true;
		}
	}
	Widget::onPress(button, x, y);
}

void SplitView::onRelease(int button, int x, int y) {
	m_moving = false;
	Widget::onRelease(button, x, y);
}

void SplitView::onMove(int x, int y) {
	auto b = realBounds();
	if (m_moving) {
		m_dividerLocation = m_orientation == Horizontal ? y : x;

		if (m_orientation == Vertical) {
			m_dividerLocation = std::clamp(m_dividerLocation, DividerSize/2, b.width - DividerSize/2);
		} else {
			m_dividerLocation = std::clamp(m_dividerLocation, DividerSize/2, b.height - DividerSize/2);
		}
	}
	Widget::onMove(x, y);
}

void SplitView::first(Widget* w) {
	if (w != nullptr) {
		w->m_parent = this;
		m_first = w;
	}
}

void SplitView::second(Widget* w) {
	if (w != nullptr) {
		w->m_parent = this;
		m_second = w;
	}
}
