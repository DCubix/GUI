#include "colorpicker.h"

#include <algorithm>
#include <cmath>
#include <sstream>

inline static void RGB2HSV(float r, float g, float b, float &h, float &s, float &v) {
	float K = 0.f;

	if (g < b) {
		std::swap(g, b);
		K = -1.f;
	}

	if (r < g) {
		std::swap(r, g);
		K = -2.f / 6.f - K;
	}

	float chroma = r - std::min(g, b);
	h = std::fabs(K + (g - b) / (6.0f * chroma + 1e-20f));
	s = chroma / (r + 1e-20f);
	v = r;
}

inline static void HSV2RGB(float& fR, float& fG, float& fB, float fH, float fS, float fV) {
	float fC = fV * fS; // Chroma
	float fHPrime = std::fmod(fH / 60.0f, 6.0f);
	float fX = fC * (1 - std::fabs(std::fmod(fHPrime, 2.0f) - 1.0f));
	float fM = fV - fC;

	if (0 <= fHPrime && fHPrime < 1) {
		fR = fC;
		fG = fX;
		fB = 0;
	} else if (1 <= fHPrime && fHPrime < 2) {
		fR = fX;
		fG = fC;
		fB = 0;
	} else if (2 <= fHPrime && fHPrime < 3) {
		fR = 0;
		fG = fC;
		fB = fX;
	} else if (3 <= fHPrime && fHPrime < 4) {
		fR = 0;
		fG = fX;
		fB = fC;
	} else if (4 <= fHPrime && fHPrime < 5) {
		fR = fX;
		fG = 0;
		fB = fC;
	} else if (5 <= fHPrime && fHPrime < 6) {
		fR = fC;
		fG = 0;
		fB = fX;
	} else {
		fR = 0;
		fG = 0;
		fB = 0;
	}

	fR += fM;
	fG += fM;
	fB += fM;
}

static std::string utoh(uint8_t v) {
	static const std::string NBS = "0123456789ABCDEF";
	std::string num = "";
	num += NBS[(v & 0xF0) >> 4];
	num += NBS[(v & 0x0F)];
	return num;
}

void ColorPicker::onDraw(Renderer& renderer) {
	auto b = realBounds();

	renderer.panel(b.x, b.y, b.width, b.height);
	renderer.colorWheel(b.x, b.y, b.width, b.height, m_value);

	float cx = b.x + b.width / 2;
	float cy = b.y + b.height / 2;
	float rad = (b.width < b.height ? b.width : b.height) / 2 - 5.0f;

	float ha = ((m_hue / 360.0f) * 2.0f - 1.0f) * NVG_PI + NVG_PI;
	float sr = rad * m_saturation;
	float px = std::cos(ha) * sr, py = std::sin(ha) * sr;

	renderer.text(cx + px - 4, cy + py - 6, "\x7", 255, 255, 255, 128);
	renderer.text(cx + px - 4, cy + py - 6, "\x9", 0, 0, 0, 128);

	if (m_moving) {
		float r, g, b;
		HSV2RGB(r, g, b, m_hue, m_saturation, m_value);

		uint8_t br = uint8_t(r * 255.0f);
		uint8_t bg = uint8_t(g * 255.0f);
		uint8_t bb = uint8_t(b * 255.0f);
		std::string rgb = "rgb(" +
				std::to_string(br) +
				", " +
				std::to_string(bg) +
				", " +
				std::to_string(bb) +
		")";
		std::string hex = "#" + utoh(br) + utoh(bg) + utoh(bb);

		int w = renderer.textWidth(rgb);
		int hw = renderer.textWidth(hex);
		int bx = cx + px + 4;
		int by = cy + py - 28;
		renderer.rect(bx, by, w + 4, 28, 0, 0, 0, 255, true);
		renderer.text(bx + 2, by + 2, rgb, 255, 255, 255, 255);
		renderer.text(bx + 2, by + 14, hex, 255, 255, 255, 255);

		renderer.roundRect(bx + 2 + hw + 2, by + 14, 12, 12, 2, br, bg, bb, 255, true);
		renderer.roundRect(bx + 2 + hw + 2, by + 14, 12, 12, 2, 255, 255, 255);
	}
}

void ColorPicker::onPress(int button, int x, int y) {
	Widget::onPress(button, x, y);
	m_moving = true;
	pick(x, y);
}

void ColorPicker::onRelease(int button, int x, int y) {
	Widget::onRelease(button, x, y);
	m_moving = false;
}

void ColorPicker::onMove(int x, int y) {
	Widget::onMove(x, y);
	if (m_moving) {
		pick(x, y);
	}
}

Color ColorPicker::color() const {
	float r, g, b;
	HSV2RGB(r, g, b, m_hue, m_saturation, m_value);
	return { r, g, b, 1.0f };
}

void ColorPicker::color(const Color& c) {
	RGB2HSV(c.r, c.g, c.b, m_hue, m_saturation, m_value);
}

void ColorPicker::pick(int x, int y) {
	auto b = realBounds();
	float cx = b.width / 2;
	float cy = b.height / 2;
	float rad = (b.width < b.height ? b.width : b.height) / 2 - 5.0f;

	float dx = cx - x;
	float dy = cy - y;
	float dist = std::sqrt(dx * dx + dy * dy);
	if (dist < rad) {
		float a = ((std::atan2(dy, dx) + NVG_PI) / (NVG_PI * 2.0f));
		m_hue = std::clamp(a * 360.0f, 0.0f, 360.0f);
		m_saturation = std::clamp(dist / rad, 0.0f, 1.0f);
	}
}
