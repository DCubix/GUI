#include "image.h"

#include <algorithm>

#include "stb.h"

Image::Image(Renderer& renderer, int width, int height) {
	m_width = width;
	m_height = height;
	m_data.resize(size_t(width * height * 4));

	for (size_t i = 0; i < m_data.size(); i+=4) {
		m_data[i + 0] = 0;
		m_data[i + 1] = 0;
		m_data[i + 2] = 0;
		m_data[i + 3] = 255;
	}
	m_handle = nvgCreateImageRGBA(renderer.context(), width, height, 0, m_data.data());
}

Image::Image(Renderer& renderer, const std::string& fileName) {
	int comp;
	uint8_t* data = stbi_load(fileName.c_str(), &m_width, &m_height, &comp, 4);
	if (data) {
		m_data = std::vector<uint8_t>(data, data + comp * m_width * m_height);
		m_handle = nvgCreateImageRGBA(renderer.context(), m_width, m_height, 0, m_data.data());
		stbi_image_free(data);
	}
}

void Image::flush(Renderer& renderer) {
	nvgUpdateImage(renderer.context(), m_handle, m_data.data());
}

std::optional<Color> Image::get(int x, int y) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return {};
	size_t i = (x + y * m_width) * 4;
	Color col;
	col.r = float(m_data[i + 0]) / 255.0f;
	col.g = float(m_data[i + 1]) / 255.0f;
	col.b = float(m_data[i + 2]) / 255.0f;
	col.a = float(m_data[i + 3]) / 255.0f;
	return col;
}

void Image::set(int x, int y, float r, float g, float b, float a) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;
	size_t i = (x + y * m_width) * 4;
	m_data[i + 0] = uint8_t(r * 255.0f);
	m_data[i + 1] = uint8_t(g * 255.0f);
	m_data[i + 2] = uint8_t(b * 255.0f);
	m_data[i + 3] = uint8_t(a * 255.0f);
}
