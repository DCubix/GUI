#ifndef SYG_SCROLL_H
#define SYG_SCROLL_H

#include "button.h"

class Scroll : public Widget {
	friend class List;
public:
	enum Orientation {
		Horizontal = 0,
		Vertical
	};

	virtual void onDraw(Renderer& renderer) override;
	virtual void onPress(int button, int x, int y) override;
	virtual void onRelease(int button, int x, int y) override;
	virtual void onScroll(int direction) override;
	virtual void onMove(int x, int y) override;
	virtual void onEnter() override;
	virtual void onExit() override;

	float value() const { return m_value; }
	void value(float v);

	float minimum() const { return m_min; }
	void minimum(float v) { m_min = v; }

	float maximum() const { return m_max; }
	void maximum(float v) { m_max = v; }

	float step() const { return m_step; }
	void step(float v) { m_step = v; }

	Orientation orientation() const { return m_orientation; }
	void orientation(Orientation v) { m_orientation = v; }

	void onChange(const std::function<void(float)>& cb) { m_onChange = cb; }

protected:
	std::function<void(float)> m_onChange;

	State m_state{ State::Normal };
	float m_value{ 0.0f }, m_min{ 0.0f }, m_max{ 1.0f }, m_step{ 0.1f };
	Orientation m_orientation{ Orientation::Horizontal };

	float thumbSize() const;
	float thumbPos() const;
};

#endif // SYG_SCROLL_H
