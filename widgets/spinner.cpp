#include "spinner.h"

#include <algorithm>
#include <sstream>

constexpr int buttonW = 16;

static bool hits(int x, int y, int bx, int by, int bw, int bh) {
	return x > bx&&
		x < bx + bw &&
		y > by &&
		y < by + bh;
}

template <typename T>
static std::string to_string_prec(const T a_value, const int n = 6) {
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

static bool hitsR(int x, int y,  int bx, int by, int bw, int bh) {
	return hits(x, y, bx, by, bw, bh);
}

Spinner::Spinner() {
	bounds().width = 100;
	bounds().height = 24;
}

void Spinner::onDraw(Renderer& renderer) {
	auto b = realBounds();

	float vnorm = (m_value - m_min) / (m_max - m_min);

	int mainW = b.width - buttonW;
	int barW = int((mainW - 4) * vnorm);

	renderer.panel(b.x, b.y, b.width, b.height);
	if (m_draggable) renderer.rect(b.x + 2, b.y + 2, barW, b.height - 4, 0, 0, 0, 80, true);

	int halfH = b.height / 2;
	renderer.button(b.x + mainW, b.y, buttonW, halfH, m_incState);
	renderer.button(b.x + mainW, b.y + halfH, buttonW, halfH, m_decState);
	renderer.text(b.x + mainW + 4, b.y + (halfH / 2 - 6), "\x1E", 0, 0, 0, 180);
	renderer.text(b.x + mainW + 4, b.y + halfH + (halfH / 2 - 6), "\x1F", 0, 0, 0, 180);

	renderer.pushClipping(b.x, b.y, mainW, b.height);
	if (!m_editing) {
		auto txt = to_string_prec(m_value, 3) + m_suffix;
		int tw = renderer.textWidth(txt);
		renderer.text(b.x + (mainW / 2 - tw / 2) + 1, b.y + (b.height / 2 - 6) + 1, txt, 0, 0, 0, 128);
		renderer.text(b.x + (mainW / 2 - tw / 2), b.y + (b.height / 2 - 6), txt, 255, 255, 255, 128);
	} else {
		renderer.text(b.x + 3, b.y + (b.height / 2 - 6) + 1, m_valText, 0, 0, 0, 128);
		renderer.text(b.x + 2, b.y + (b.height / 2 - 6), m_valText, 255, 255, 255, 128);

		int cx = m_valText.size() * 8;
		renderer.text(b.x + cx + 3, b.y + (b.height / 2 - 6) + 1, "|", 0, 0, 0, 128);
		renderer.text(b.x + cx + 2, b.y + (b.height / 2 - 6), "|", 255, 255, 255, 128);
	}
	renderer.popClipping();
}

void Spinner::onMove(int x, int y) {
	auto b = realBounds();
	int halfH = b.height / 2;
	int mainW = b.width - buttonW;
	if (hitsR(x, y, mainW, 0, buttonW, halfH)) { // INC
		m_incState = 1;
	} else {
		m_incState = 0;
	}

	if (hitsR(x, y, mainW, halfH, buttonW, halfH)) { // DEC
		m_decState = 1;
	} else {
		m_decState = 0;
	}

	if (m_decState == 0 && m_incState == 0 && m_clicked && m_draggable) {
		float xnorm = float(x) / (mainW - 2);
		xnorm = std::clamp(xnorm, 0.0f, 1.0f);

		float val = m_min + xnorm * (m_max - m_min);
			val = std::floor(val / m_step) * m_step;
		value(val);
	}

	Widget::onMove(x, y);
}

void Spinner::onClick(int button, int x, int y) {
	Widget::onClick(button, x, y);

	auto b = realBounds();
	int mainW = b.width - buttonW;
	if (hitsR(x, y, 0, 0, mainW, b.height) && m_draggable) {
		float xnorm = float(x) / (mainW - 2);
		xnorm = std::clamp(xnorm, 0.0f, 1.0f);

		float val = m_min + xnorm * (m_max - m_min);
		val = std::floor(val / m_step) * m_step;
		value(val);
	}
}

void Spinner::onDoubleClick(int button, int x, int y) {
	Widget::onDoubleClick(button, x, y);

	auto b = realBounds();
	int mainW = b.width - buttonW;
	if (hitsR(x, y, 0, 0, mainW, b.height)) {
		m_editing = true;
		m_valText = "";
	}
}

void Spinner::onPress(int button, int x, int y) {
	Widget::onPress(button, x, y);
	if (!m_editing) {
		auto b = realBounds();
		int halfH = b.height / 2;
		int mainW = b.width - buttonW;
		if (hitsR(x, y, mainW, 0, buttonW, halfH)) { // INC
			value(std::clamp(value() + step(), m_min, m_max));
			m_incState = 2;
		} else {
			m_incState = 1;
		}

		if (hitsR(x, y, mainW, halfH, buttonW, halfH)) { // DEC
			value(std::clamp(value() - step(), m_min, m_max));
			m_decState = 2;
		} else {
			m_decState = 1;
		}
	}
}

void Spinner::onRelease(int button, int x, int y) {
	Widget::onRelease(button, x, y);
	m_incState = 0;
	m_decState = 0;
}

void Spinner::onScroll(int direction) {
	Widget::onScroll(direction);
}

void Spinner::onType(char chr) {
	if (m_editing) {
		m_valText.push_back(chr);
	}
}

void Spinner::onEnter() {
	Widget::onEnter();
	m_incState = 0;
	m_decState = 0;
}

void Spinner::onExit() {
	Widget::onExit();
	m_incState = 0;
	m_decState = 0;
}

void Spinner::onBlur() {
	Widget::onBlur();
	if (m_editing) {
		m_editing = false;
		if (!m_valText.empty() &&
			std::find_if(
				m_valText.begin(), m_valText.end(),
				[](char c) { return !std::isdigit(c & 0xFF) && c != '.' && c != '-'; }
			) == m_valText.end()
			) {
			value(std::clamp(std::stof(m_valText), m_min, m_max));
		}
	}
}

void Spinner::onKeyPress(int key, int mod) {
	if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
		onBlur();
	}

	if (m_editing && key == SDLK_BACKSPACE) {
		if (!m_valText.empty()) m_valText.pop_back();
	}
}

void Spinner::value(float v) {
	if (m_onChange) m_onChange(v);
	m_value = v;
}
