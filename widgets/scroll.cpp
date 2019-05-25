#include "scroll.h"

static bool hits(int x, int y, int bx, int by, int bw, int bh) {
	return x > bx&&
		x < bx + bw &&
		y > by &&
		y < by + bh;
}

static bool hitsR(int x, int y,  int bx, int by, int bw, int bh) {
	return hits(x, y, bx, by, bw, bh);
}

void Scroll::onDraw(Renderer& renderer) {
	if (!enabled()) m_state = State::Disabled;

	auto b = realBounds();
	const float sz = m_orientation == Horizontal ? b.width : b.height;
	const float ts = thumbSize();
	const float thumbSizeSmall = m_orientation == Horizontal ? b.height : b.width;

	renderer.panel(b.x, b.y, b.width, b.height);
	renderer.rect(b.x+1, b.y+1, b.width-1, b.height-1, 0, 0, 0, 50, true);
//	if (ts < sz) {
		float pos = thumbPos();
		if (m_orientation == Horizontal) {
			renderer.button(b.x + pos + 2, b.y + 2, ts - 4, thumbSizeSmall - 4, m_state);
		} else {
			renderer.button(b.x + 2, b.y + pos + 2, thumbSizeSmall - 4, ts - 4, m_state);
		}
//	}
}

void Scroll::onPress(int button, int x, int y) {
	Widget::onPress(button, x, y);

	auto b = realBounds();
	const float sz = m_orientation == Horizontal ? b.width : b.height;
	const float ts = thumbSize();
	const float thumbSizeSmall = m_orientation == Horizontal ? b.height : b.width;

//	if (ts > sz) return;

	bool hitThumb = m_orientation == Horizontal ?
				hitsR(x, y, 0, 0, sz, thumbSizeSmall) :
				hitsR(x, y, 0, 0, thumbSizeSmall, sz);

	if (hitThumb) {
		m_state = State::Click;
	}
	onMove(x, y);
}

void Scroll::onRelease(int button, int x, int y) {
	Widget::onRelease(button, x, y);
	m_state = State::Normal;
}

void Scroll::onScroll(int direction) {
	Widget::onScroll(direction);
	value(value() + step() * direction * -1);
}

void Scroll::onMove(int x, int y) {
	Widget::onMove(x, y);

	auto b = realBounds();
	const float sz = m_orientation == Horizontal ? b.width : b.height;
	const float ts = thumbSize();
	const float thumbSizeSmall = m_orientation == Horizontal ? b.height : b.width;

//	if (ts > sz) return;

	bool hitThumb = m_orientation == Horizontal ?
				hitsR(x, y, 0, 0, sz, thumbSizeSmall) :
				hitsR(x, y, 0, 0, thumbSizeSmall, sz);

	if (hitThumb) {
		m_state = State::Hover;
	} else {
		m_state = State::Normal;
	}

	if (m_clicked) {
		m_state = State::Click;

		const float tsh = ts / 2.0f;
		float xnorm = (float(m_orientation == Horizontal ? x : y) - tsh) / (sz - ts);
		xnorm = std::clamp(xnorm, 0.0f, 1.0f);

		float val = m_min + xnorm * (m_max - m_min);
		val = std::floor(val / m_step) * m_step;
		value(val);
	}
}

void Scroll::onEnter() {
	Widget::onEnter();
	m_state = State::Normal;
}

void Scroll::onExit() {
	Widget::onExit();
	m_state = State::Normal;
}

void Scroll::value(float v) {
	v = std::clamp(v, m_min, m_max);
	if (m_onChange) m_onChange(v);
	m_value = v;
}

float Scroll::thumbSize() const {
	auto b = realBounds();
	const float sz = m_orientation == Horizontal ? b.width : b.height;
	const float r = sz > (m_max - m_min) ? (m_max - m_min) / sz : sz / (m_max - m_min);
	return m_orientation == Horizontal ? b.width * r : b.height * r;
	//return 16.0f;
}

float Scroll::thumbPos() const {
	auto b = realBounds();
	const float sz = m_orientation == Horizontal ? b.width : b.height;
	float space = sz - thumbSize();
	float vnorm = (m_value - m_min) / (m_max - m_min);
	return vnorm * space;
}
