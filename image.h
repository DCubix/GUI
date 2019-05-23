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

class PixelData {
public:
	PixelData() = default;
	virtual ~PixelData() = default;

	PixelData(int width, int height);
	PixelData(const std::string& fileName);

	std::optional<Color> get(int x, int y);
	void set(int x, int y, float r, float g, float b, float a);

	int width() const { return m_width; }
	int height() const { return m_height; }

	std::vector<uint8_t> data() const { return m_data; }

private:
	std::vector<uint8_t> m_data;
	int m_width, m_height;
};

class Image {
public:
	Image() = default;
	virtual ~Image() = default;

	void load(Renderer& renderer, const PixelData& data);

	int width() const { return m_width; }
	int height() const { return m_height; }
	int id() const { return m_handle; }

private:
	int m_handle{ -1 };
	int m_width, m_height;
};

#endif // IMAGE_H
