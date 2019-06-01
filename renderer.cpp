#include "renderer.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <sstream>

#include "skin.h"
#include "font.h"
#include "font_small.h"

#define MAX_SHADOW 16.0f

extern "C" {
	#define NANOVG_GL2_IMPLEMENTATION
	#include "nanovg/src/nanovg_gl.h"
}

Renderer::Renderer() {
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	m_ren = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

	m_skin = nvgCreateImageMem(m_ren, 0, skin_data, skin_size);
	m_font = nvgCreateImageMem(m_ren, 0, font_data, font_size);
	m_fontSmall = nvgCreateImageMem(m_ren, 0, font_small_data, font_small_size);

	nvgImageSize(m_ren, m_font, &m_fontWidth, &m_fontHeight);
	nvgImageSize(m_ren, m_fontSmall, &m_fontSmallWidth, &m_fontSmallHeight);
}

Renderer::~Renderer() {
	nvgDeleteImage(m_ren, m_skin);
	nvgDeleteImage(m_ren, m_font);
	nvgDeleteImage(m_ren, m_fontSmall);
}

void Renderer::line(int x1, int y1, int x2, int y2, int r, int g, int b, int a, int width) {
	nvgSave(m_ren);
	nvgStrokeWidth(m_ren, width);
	nvgStrokeColor(m_ren, nvgRGBA(r, g, b, a));
	nvgBeginPath(m_ren);
	nvgMoveTo(m_ren, x1, y1);
	nvgLineTo(m_ren, x2, y2);
	nvgStroke(m_ren);
	nvgRestore(m_ren);
}

void Renderer::curve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4,
					 int r, int g, int b, int a, int width) {
	nvgSave(m_ren);
	nvgStrokeWidth(m_ren, width);
	nvgStrokeColor(m_ren, nvgRGBA(r, g, b, a));
	nvgBeginPath(m_ren);
	nvgMoveTo(m_ren, x1, y1);
	nvgBezierTo(m_ren, x2, y2, x3, y3, x4, y4);
	nvgStroke(m_ren);
	nvgRestore(m_ren);
}

void Renderer::roundRect(int x, int y, int w, int h, int rad, int r, int g, int b, int a, bool fill, int width) {
	nvgSave(m_ren);
	nvgStrokeWidth(m_ren, width);
	nvgStrokeColor(m_ren, nvgRGBA(r, g, b, a));
	nvgFillColor(m_ren, nvgRGBA(r, g, b, a));
	nvgBeginPath(m_ren);
	nvgRoundedRect(m_ren, x, y, w, h, rad);
	if (fill) nvgFill(m_ren);
	else nvgStroke(m_ren);
	nvgRestore(m_ren);
}

void Renderer::rect(int x, int y, int w, int h, int r, int g, int b, int a, bool fill, int width) {
	nvgSave(m_ren);
	nvgStrokeWidth(m_ren, width);
	nvgStrokeColor(m_ren, nvgRGBA(r, g, b, a));
	nvgFillColor(m_ren, nvgRGBA(r, g, b, a));
	nvgBeginPath(m_ren);
	nvgRect(m_ren, x, y, w, h);
	if (fill) nvgFill(m_ren);
	else nvgStroke(m_ren);
	nvgRestore(m_ren);
}

void Renderer::panel(int x, int y, int w, int h, int sx, int sy, float shadow) {
	const int TS = 64;
	if (shadow > 0.0f) {
		const float f = std::clamp(shadow, 0.01f, 1.0f) * 30;
		const float hf = f / 2.0f;
		// Drop shadow
		NVGpaint shadowPaint = nvgBoxGradient(
					m_ren, x + sx, y + sy, w, h, 3, f,
					nvgRGBA(0, 0, 0, 132),
					nvgRGBA(0, 0, 0, 0)
		);
		nvgSave(m_ren);
		nvgBeginPath(m_ren);
		nvgRect(m_ren, x - hf, y - hf, w + f,h + f);
		nvgRoundedRect(m_ren, x, y, w, h, 3);
		nvgPathWinding(m_ren, NVG_HOLE);
		nvgFillPaint(m_ren, shadowPaint);
		nvgFill(m_ren);
		nvgRestore(m_ren);
	}
	patch(x, y, w, h, 0, 160, TS, TS);
}

void Renderer::button(int x, int y, int w, int h, int state) {
	const int TS = 64;
	int ox = state * TS;
	patch(x, y, w, h, ox, 96, TS, TS, 4);
}

void Renderer::check(int x, int y, bool state) {
	int ox = int(state) * 16;
	skin(x, y, 16, 16, 64 + ox, 160, 16, 16);
}

void Renderer::flatPanel(int x, int y, int w, int h, int sx, int sy, float shadow) {
	const int TS = 64;
	if (shadow > 0.0f) {
		const float f = std::clamp(shadow, 0.01f, 1.0f) * 30;
		const float hf = f / 2.0f;
		// Drop shadow
		NVGpaint shadowPaint = nvgBoxGradient(
					m_ren, x + sx, y + sy, w, h, 3, f,
					nvgRGBA(0, 0, 0, 132),
					nvgRGBA(0, 0, 0, 0)
		);
		nvgSave(m_ren);
		nvgBeginPath(m_ren);
		nvgRect(m_ren, x - hf, y - hf, w + f,h + f);
		nvgRoundedRect(m_ren, x, y, w, h, 3);
		nvgPathWinding(m_ren, NVG_HOLE);
		nvgFillPaint(m_ren, shadowPaint);
		nvgFill(m_ren);
		nvgRestore(m_ren);
	}
	patch(x, y, w, h, 128, 160, TS, TS);
}

void Renderer::pushClipping(int x, int y, int w, int h) {
	// Fix cliprect
	if (w < 0) {
		w = -w;
		x -= w;
	}
	if (h < 0) {
		h = -h;
		y -= h;
	}

	if (m_clipRects.empty()) {
		if (w < 1 || h < 1) return;
		//nvgSave(m_ren);
	} else {
		// Merge
		SDL_Rect parent = m_clipRects.top();
		float minX = std::max(parent.x, x);
		float maxX = std::min(parent.x + parent.w, x + w);
		if (maxX - minX < 1) return;

		float minY = std::max(parent.y, y);
		float maxY = std::min(parent.y + parent.h, y + h);
		if (maxY - minY < 1) return;

		x = minX;
		y = minY;
		w = int(maxX - minX);
		h = int(std::max(1.0f, maxY - minY));
	}

	SDL_Rect cr = { x, y, w, h };
	m_clipRects.push(cr);

	nvgScissor(m_ren, x, y, w, h);
}

void Renderer::popClipping() {
	if (!m_clipRects.empty()) {
		m_clipRects.pop();
	}
	if (m_clipRects.empty()) {
		//nvgRestore(m_ren);
	} else {
		SDL_Rect cr = m_clipRects.top();
		nvgScissor(m_ren, cr.x, cr.y, cr.w, cr.h);
	}
}

void Renderer::text(int x, int y, const std::string& str, int r, int g, int b, int a, bool ww, int w) {
	textGen(m_font, m_fontWidth, m_fontHeight, x, y, str, r, g, b, a, ww, w);
}

void Renderer::textSmall(int x, int y, const std::string& str, int r, int g, int b, int a, bool ww, int w) {
	textGen(m_fontSmall, m_fontSmallWidth, m_fontSmallHeight, x, y, str, r, g, b, a, ww, w);
}

int Renderer::textWidth(const std::string& str) const {
	return str.size() * 8;
}

void Renderer::skin(int x, int y, int w, int h, int sx, int sy, int sw, int sh) {
	image(m_skin, x, y, w, h, sx, sy, sw, sh);
}

void Renderer::image(int image, int x, int y, int w, int h, int sx, int sy, int sw, int sh, int r, int g, int b, int a) {
	if (sx < 0 || sy < 0 || sw < 0 || sh < 0) {
		sx = x;
		sy = y;
		sw = w;
		sh = h;
	}

	float ax, ay;
	int iw, ih;
	NVGpaint img;

	nvgImageSize(m_ren, image, &iw, &ih);

	ax = float(w) / float(sw);
	ay = float(h) / float(sh);

	img = nvgImagePattern(m_ren,
			x - float(sx) * ax,
			y - float(sy) * ay,
			float(iw) * ax,
			float(ih) * ay, 0,
			image,
			float(a) / 255.0f
	);
	img.innerColor = nvgRGBA(r, g, b, a);

	nvgSave(m_ren);
	nvgBeginPath(m_ren);
	nvgRect(m_ren, x, y, w, h);
	nvgFillPaint(m_ren, img);
	nvgFill(m_ren);
	nvgRestore(m_ren);
}

void Renderer::patch(int x, int y, int w, int h, int sx, int sy, int sw, int sh, int pad, int tpad) {
	tpad = tpad < 0 ? pad : tpad;

	// Left
	skin(x, y, pad, pad, sx, sy, tpad, tpad); // T
	skin(x, y + pad, pad, h - pad*2, sx, sy + tpad, tpad, sh - tpad *2); // M
	skin(x, y + (h - pad), pad, pad, sx, sy + (sh - tpad), tpad, tpad); // B

	// Center
	int cw = w - pad * 2;
	int tw = sw - tpad * 2;
	skin(x + pad, y, cw, pad, sx + tpad, sy, tw, tpad); // T
	skin(x + pad, y + pad, cw, h - pad * 2, sx + tpad, sy + tpad, tw, sh - tpad * 2); // M
	skin(x + pad, y + (h - pad), cw, pad, sx + tpad, sy + (sh - tpad), tw, tpad); // B

	// Right
	skin(x + (w - pad), y, pad, pad, sx + (sw - tpad), sy, tpad, tpad); // T
	skin(x + (w - pad), y + pad, pad, h - pad * 2, sx + (sw - tpad), sy + tpad, tpad, sh - tpad * 2); // M
	skin(x + (w - pad), y + (h - pad), pad, pad, sx + (sw - tpad), sy + (sh - tpad), tpad, tpad); // B

	//rect(x, y, w, h, 255, 0, 0);
}

void Renderer::begin(int w, int h) {
	nvgBeginFrame(m_ren, w, h, float(w) / float(h));
}

void Renderer::end() {
	nvgEndFrame(m_ren);
}

void Renderer::textGen(int font, int fw, int fh, int x, int y, const std::string& str, int r, int g, int b, int a, bool ww, int w) {
	int tx = x, ty = y;

	std::stringstream ss(str);
	std::string item;

	while (std::getline(ss, item, ' ')) {
		if (ww && tx + textWidth(item) > w) {
			tx = x;
			ty += 13;
		}
		for (size_t i = 0; i < item.length(); i++) {
			uint8_t c = uint8_t(item.at(i));
			if (c == '\n') {
				tx = x;
				ty += 13;
			} else {
				if (c != ' ') {
					putChar(font, fw, fh, tx, ty, c, r, g, b, a);
				}
			}
			tx += 8;
		}
		tx += 8;
	}
}

void Renderer::putChar(int font, int fw, int fh, int x, int y, uint8_t c, int r, int g, int b, int a) {
	const int cw = fw / 16;
	const int ch = fh / 16;

	const int sx = int(c) % 16 * cw;
	const int sy = int(c) / 16 * ch;

	image(font, x, y, cw, ch, sx, sy, cw, ch, r, g, b, a);
}
