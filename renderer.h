#ifndef SYG_RENDERER_H
#define SYG_RENDERER_H

#include <string>
#include <stack>
#include <vector>

#include "sdl_headers.h"

extern "C" {
	#include "glad/glad.h"
	#include "nanovg/src/nanovg.h"
}

class Renderer {
public:
	Renderer();
	~Renderer();

	void line(int x1, int y1, int x2, int y2, int r, int g, int b, int a = 255, int width = 1);
	void curve(
		int x1, int y1,
		int x2, int y2,
		int x3, int y3,
		int x4, int y4,
		int r, int g, int b, int a = 255, int width = 1
	);
	void roundRect(
		int x, int y, int w, int h, int rad, int r, int g, int b, int a = 255, bool fill = false, int width = 1
	);

	void rect(int x, int y, int w, int h, int r, int g, int b, int a = 255, bool fill = false, int width = 1);

	void panel(int x, int y, int w, int h, int sx = 0, int sy = 0, float shadow = 0.0f);
	void button(int x, int y, int w, int h, int state = 0);
	void check(int x, int y, bool state);
	void flatPanel(int x, int y, int w, int h, int sx = 0, int sy = 0, float shadow = 0.0f);

	void pushClipping(int x, int y, int w, int h);
	void popClipping();

	void text(int x, int y, const std::string& str, int r, int g, int b, int a = 255);
	void textSmall(int x, int y, const std::string& str, int r, int g, int b, int a = 255);
	int textWidth(const std::string& str) const;

	void patch(int x, int y, int w, int h, int sx, int sy, int sw, int sh, int pad = 5, int tpad = -1);

	void begin(int w, int h);
	void end();

	void image(
		int image,
		int x, int y, int w, int h,
		int sx, int sy, int sw, int sh,
		int r=255, int g=255, int b=255, int a=255
	);

	NVGcontext* context() { return m_ren; }

private:
	int m_skin, m_font, m_fontSmall;
	int m_fontWidth, m_fontHeight;
	int m_fontSmallWidth, m_fontSmallHeight;

	std::stack<SDL_Rect> m_clipRects;

	NVGcontext* m_ren;

	void skin(
		int x, int y, int w, int h,
		int sx, int sy, int sw, int sh
	);

	void textGen(int font, int fw, int fh, int x, int y, const std::string& str, int r, int g, int b, int a);
	void putChar(int font, int fw, int fh, int x, int y, uint8_t c, int r, int g, int b, int a = 0xFF);
};

#endif // SYG_RENDERER_H
