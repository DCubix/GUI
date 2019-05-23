#ifndef SYG_LAYOUT_H
#define SYG_LAYOUT_H

#include <vector>
#include <array>

class Widget;
class Panel;
class Layout {
public:
	virtual void apply(Panel* parent, std::vector<Widget*> widgets) {}
};

enum BorderLayoutPosition {
	Left = 0,
	Right,
	Bottom,
	Top,
	Center,
	MaxPosition
};

class BorderLayout : public Layout {
public:
	BorderLayout() { slots.fill(nullptr); }
	virtual void apply(Panel* panel, std::vector<Widget*> widgets) override;
private:
	std::array<Widget*, MaxPosition> slots;
	int m_slotCount{ 0 };
};

class GridLayout : public Layout {
public:
	virtual void apply(Panel* panel, std::vector<Widget*> widgets) override;
};

class StackLayout : public Layout {
public:
	virtual void apply(Panel* panel, std::vector<Widget*> widgets) override;
};

class FlowLayout : public Layout {
public:
	virtual void apply(Panel* panel, std::vector<Widget*> widgets) override;
};

#endif // SYG_LAYOUT_H
