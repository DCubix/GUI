#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <optional>
#include <cstdint>

#include "renderer.h"

struct Color {
	float r, g, b, a;
};

class Image {
public:
	Image() = default;
	virtual ~Image() = default;

	Image(Renderer& renderer, int width, int height);
	Image(Renderer& renderer, const std::string& fileName);

	void flush(Renderer& renderer);

	std::optional<Color> get(int x, int y);
	void set(int x, int y, float r, float g, float b, float a);

	int width() const { return m_width; }
	int height() const { return m_height; }
	int id() const { return m_handle; }

private:
	int m_handle{ -1 };
	std::vector<uint8_t> m_data;
	int m_width, m_height;
};

#endif // IMAGE_H
