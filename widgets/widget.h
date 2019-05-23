#ifndef SYG_WIDGET_H
#define SYG_WIDGET_H

#include "../events.h"
#include "../renderer.h"

class GUI;
class Panel;
class Widget : public Element {
	friend class GUI;
	friend class Panel;
public:
	virtual ~Widget() = default;

	virtual void onDraw(Renderer& renderer) {}

	bool visible() const { return m_visible; }
	void visible(bool v) { m_visible = v; invalidate(); }

	void configure(int row, int col, int colSpan = 1, int rowSpan = 1);

	int gridRow() const { return m_gridRow; }
	int gridColumn() const { return m_gridColumn; }

	int rowSpan() const { return m_rowSpan; }
	int columnSpan() const { return m_columnSpan; }

	virtual void invalidate() { m_dirty = true; }

	virtual bool dirty() const { return m_dirty; }

	bool alwaysRedraw() const { return m_alwaysRedraw; }
	void alwaysRedraw(bool v) { m_alwaysRedraw = v; }

	int layoutParam() const { return m_layoutParam; }
	void layoutParam(int v) { m_layoutParam = v; invalidate(); }

	Panel* getValidParent();

protected:
	GUI* m_gui;
	int m_layoutParam{ 0 };

	bool m_visible{ true },
		m_dirty{ true },
		m_alwaysRedraw{ false };
	int m_gridRow{ 0 },
		m_gridColumn{ 0 },
		m_rowSpan{ 1 },
		m_columnSpan{ 1 };

	Panel* getValidParentRecursive(Widget* parent);
};

#endif // SYG_WIDGET_H