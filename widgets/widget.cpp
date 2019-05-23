#include "widget.h"

#include "panel.h"

void Widget::configure(int row, int col, int colSpan, int rowSpan) {
	m_gridRow = row;
	m_gridColumn = col;
	m_columnSpan = colSpan;
	m_rowSpan = rowSpan;
}

Panel* Widget::getValidParent() {
	return getValidParentRecursive((Widget*) m_parent);
}

Panel* Widget::getValidParentRecursive(Widget* parent) {
	if (!parent) return nullptr;

	Panel* panel = dynamic_cast<Panel*>(parent);
	if (!panel) return nullptr;

	if (panel->parent() && !panel->drawBackground())
		return getValidParentRecursive((Widget*) panel->parent());
	return panel;
}