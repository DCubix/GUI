#ifndef SYG_PANEL_H
#define SYG_PANEL_H

#include <memory>

#include "widget.h"
#include "../layout.h"

class Panel : public Widget {
public:
	Panel();

	virtual void onDraw(Renderer& renderer) override;

	void add(Widget* widget);
	void remove(Widget* widget);

	int gridWidth() const { return m_gridWidth; }
	void gridWidth(int v) { m_gridWidth = v; }

	int gridHeight() const { return m_gridHeight; }
	void gridHeight(int v) { m_gridHeight = v; }

	int padding() const { return m_padding; }
	void padding(int v) { m_padding = v; }

	int spacing() const { return m_spacing; }
	void spacing(int v) { m_spacing = v; }

	bool drawBackground() const { return m_drawBackground; }
	void drawBackground(bool v) { m_drawBackground = v; }

	void setLayout(Layout* layout);

	void removeAll();
	virtual void invalidate() override;

private:
	std::unique_ptr<Layout> m_layout;
	std::vector<Widget*> m_children;
	int m_gridWidth{ 2 }, // # of columns
		m_gridHeight{ 2 }, // # of rows
		m_padding{ 5 }, // Spacing around the edges
		m_spacing{ 4 }; // Spacing between grid cells

	bool m_drawBackground{ true };

	void performLayout();
};

#endif // SYG_PANEL_H
