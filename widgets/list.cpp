#include "list.h"

#include "../gui.h"

constexpr int ItemHeight = 16;

static bool hits(int x, int y, int bx, int by, int bw, int bh) {
	return x > bx&&
		x < bx + bw &&
		y > by &&
		y < by + bh;
}

static bool hitsR(int x, int y,  int bx, int by, int bw, int bh) {
	return hits(x, y, bx, by, bw, bh);
}

List::List() {
	m_scroll = std::make_unique<Scroll>();
	m_scroll->step(4.0f);
	m_scroll->orientation(Scroll::Vertical);
	m_scroll->bounds().width = 16;
	m_scroll->m_gui = nullptr;
}

void List::onDraw(Renderer& renderer) {
	if (m_scroll->m_gui == nullptr) {
		m_scroll->m_gui = m_gui;
		m_gui->events()->subscribe(m_scroll.get());
	}

	auto b = realBounds();
	int w = b.width - m_scroll->bounds().width;

	renderer.panel(b.x, b.y, w, b.height);

	renderer.pushClipping(b.x + 1, b.y + 1, b.width - 2, b.height - 2);
	renderer.rect(b.x, b.y, w, b.height, 0, 0, 0, 50, true);

	int y = 0;
	int i = 0;
	int iy = (ItemHeight / 2 - 6) + 1;
	int offy = int(m_scroll->value());
	for (auto& item : m_list) {
		if (y + 2 - offy > b.height) break;
		if (i == m_selected) {
			renderer.rect(b.x + 2, b.y + y + 2 - offy, w - 4, ItemHeight-1, 255, 255, 255, 180, true);
			renderer.text(b.x + 8, b.y + y + iy - offy, item, 0, 0, 0, 180);
		} else {
			renderer.rect(b.x + 2, b.y + y + 2 - offy, w - 4, ItemHeight-1, 0, 0, 0, 50, true);
			renderer.text(b.x + 9, b.y + y + iy + 1 - offy, item, 0, 0, 0, 255);
			renderer.text(b.x + 8, b.y + y + iy - offy, item, 255, 255, 255, 180);
		}
		y += ItemHeight;
		i++;
	}
	renderer.popClipping();

	m_scroll->minimum(0.0f);
	m_scroll->maximum(int(ItemHeight * (m_list.size() + 1)) - b.height);

	m_scroll->bounds().x = b.x + w - 1;
	m_scroll->bounds().y = b.y;
	m_scroll->bounds().height = b.height;
	m_scroll->onDraw(renderer);
}

void List::onPress(int button, int x, int y) {
	Widget::onPress(button, x, y);

	auto b = realBounds();
	int w = b.width - m_scroll->bounds().width;
	int ky = 0;
	int offy = int(m_scroll->value());
	for (int i = 0; i < m_list.size(); i++) {
		if (hitsR(x, y, 0, ky - offy, w - 4, ItemHeight)) {
			selected(i);
			break;
		}
		ky += ItemHeight;
	}
}

void List::onScroll(int direction) {
	m_scroll->onScroll(direction);
}

Element::Size List::preferredSize() {
	if (m_autoSize) {
		return { m_bounds.width, int(ItemHeight * m_list.size() + 4) };
	}
	return Widget::preferredSize();
}
