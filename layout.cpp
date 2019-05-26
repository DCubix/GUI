#include "layout.h"

#include "widgets/widget.h"
#include "widgets/panel.h"

void BorderLayout::apply(Panel* panel, std::vector<Widget*> widgets) {
	auto sz = panel->realBounds();
	int top = panel->padding();
	int bottom = sz.height - panel->padding();
	int left = panel->padding();
	int right = sz.width - panel->padding();

	for (Widget* w : widgets) {
		BorderLayoutPosition param = BorderLayoutPosition(w->layoutParam());
		if (!slots[param]) {
			slots[param] = w;
			m_slotCount++;
		}
	}

	for (Widget* w : slots) {
		if (!w) continue;
		auto& wb = w->bounds();
		BorderLayoutPosition param = BorderLayoutPosition(w->layoutParam());
		switch (param) {
			default: break;
			case BorderLayoutPosition::Top: {
				wb.x = left; wb.y = top;
				wb.width = right - left;
				top += wb.height + panel->spacing();
			} break;
			case BorderLayoutPosition::Bottom: {
				wb.x = left; wb.y = bottom - wb.height;
				wb.width = right - left;
				bottom -= wb.height + panel->spacing();
			} break;
			case BorderLayoutPosition::Left: {
				wb.x = left; wb.y = top;
				wb.height = bottom - top;
				left += wb.width + panel->spacing();
			} break;
			case BorderLayoutPosition::Right: {
				wb.x = right - wb.width; wb.y = top;
				wb.height = bottom - top;
				right -= wb.width + panel->spacing();
			} break;
			case BorderLayoutPosition::Center: {
				wb.x = left; wb.y = top;
				wb.width = right - left; wb.height = bottom - top;
			} break;
		}
	}
}

void GridLayout::apply(Panel* panel, std::vector<Widget*> widgets) {
	auto sz = panel->bounds();
	const int panelWidth = sz.width;
	const int panelHeight = sz.height;
	const int spacingWidth = (panel->gridWidth() - 1) * panel->spacing();
	const int spacingHeight = (panel->gridHeight() - 1) * panel->spacing();
	const int width = panelWidth - (spacingWidth + panel->padding() * 2);
	const int height = panelHeight - (spacingHeight + panel->padding() * 2);
	const int cellWidth = width / panel->gridWidth();
	const int cellHeight = height / panel->gridHeight();

	for (Widget* w : widgets) {
		w->bounds().x = w->gridColumn() * (cellWidth + panel->spacing()) + panel->padding();
		w->bounds().y = w->gridRow() * (cellHeight + panel->spacing()) + panel->padding();
		w->bounds().width = (cellWidth * w->columnSpan() + panel->spacing() * (w->columnSpan() - 1));
		w->bounds().height = (cellHeight * w->rowSpan() + panel->spacing() * (w->rowSpan() - 1));
	}
}

void StackLayout::apply(Panel* panel, std::vector<Widget*> widgets) {
	int y = panel->padding();
	for (Widget* w : widgets) {
		w->bounds().x = panel->padding();
		w->bounds().y = y;
		w->bounds().width = panel->bounds().width - panel->padding() * 2;
		y += w->bounds().height + panel->spacing();
	}
}

void FlowLayout::apply(Panel* panel, std::vector<Widget*> widgets) {
	int x = panel->padding();
	for (Widget* w : widgets) {
		w->bounds().x = x;
		w->bounds().y = panel->padding();
		w->bounds().height = panel->bounds().width - panel->padding() * 2;
		x += w->bounds().width + panel->spacing();
	}
}
