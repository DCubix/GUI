#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "widget.h"
#include "image.h"

class ColorPicker : public Widget {
public:
	virtual void onDraw(Renderer& renderer) override;
	virtual void onPress(int button, int x, int y) override;
	virtual void onRelease(int button, int x, int y) override;
	virtual void onMove(int x, int y) override;

	Color color() const;
	void color(const Color& c);

	float& hue() { return m_hue; }
	float& saturation() { return m_saturation; }
	float& value() { return m_value; }

	void hue(float v) { m_hue = v; }
	void saturation(float v) { m_saturation = v; }
	void value(float v) { m_value = v; }

private:
	float m_hue{ 0.0f }, m_saturation{ 1.0f }, m_value{ 1.0f };
	bool m_moving{ false };

	void pick(int x, int y);
};

#endif // COLORPICKER_H
