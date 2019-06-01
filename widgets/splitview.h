#ifndef SPLITVIEW_H
#define SPLITVIEW_H

#include "widget.h"

constexpr int DividerSize = 6;

class SplitView : public Widget {
public:
	virtual ~SplitView() = default;

	enum Orientation {
		Horizontal = 0,
		Vertical
	};

	virtual void onDraw(Renderer& renderer) override;
	virtual void onPress(int button, int x, int y) override;
	virtual void onRelease(int button, int x, int y) override;
	virtual void onMove(int x, int y) override;

	Widget* first() { return m_first; }
	void first(Widget* w);

	Widget* second() { return m_second; }
	void second(Widget* w);

	int dividerLocation() const { return m_dividerLocation; }
	void dividerLocation(int v) { m_dividerLocation = v; }

	Orientation orientation() const { return m_orientation; }
	void orientation(Orientation v) { m_orientation = v; }

private:
	Orientation m_orientation{ Vertical };
	Widget *m_first{ nullptr }, *m_second{ nullptr };
	int m_dividerLocation{ 100 };
	bool m_moving{ false };
};

#endif // SPLITVIEW_H
