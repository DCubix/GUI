#ifndef SYG_WIDGET_H
#define SYG_WIDGET_H

#include "../events.h"
#include "../renderer.h"

class GUI;
class Panel;
class Widget : public Element {
	friend class GUI;
	friend class Panel;
	friend class List;
	friend class ScrollView;
public:
	virtual ~Widget() = default;

	virtual void onDraw(Renderer& renderer) {}

	bool visible() const { return m_visible; }
	void visible(bool v) { m_visible = v; }

	void configure(int row, int col, int colSpan = 1, int rowSpan = 1);

	int gridRow() const { return m_gridRow; }
	int gridColumn() const { return m_gridColumn; }

	int rowSpan() const { return m_rowSpan; }
	int columnSpan() const { return m_columnSpan; }

	int layoutParam() const { return m_layoutParam; }
	void layoutParam(int v) { m_layoutParam = v; }

	bool autoSize() const { return m_autoSize; }
	void autoSize(bool v) { m_autoSize = v; }

	Panel* getValidParent();

	virtual Size preferredSize();

	std::string name() const { return m_name; }
	void name(const std::string& v) { m_name = v; }

protected:
	GUI* m_gui;
	int m_layoutParam{ 0 };

	bool m_visible{ true }, m_autoSize{ false };
	int m_gridRow{ 0 },
		m_gridColumn{ 0 },
		m_rowSpan{ 1 },
		m_columnSpan{ 1 };

	Panel* getValidParentRecursive(Widget* parent);

	std::string m_name{ "widget" };
};

#endif // SYG_WIDGET_H
