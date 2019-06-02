#ifndef LIST_H
#define LIST_H

#include "scroll.h"
#include <memory>

class List : public Widget {
public:
	List();
	virtual ~List() override;

	virtual void onDraw(Renderer& renderer) override;
	virtual void onPress(int button, int x, int y) override;
	virtual void onScroll(int direction) override;

	std::vector<std::string> list() const { return m_list; }
	void list(const std::vector<std::string>& v);

	int selected() const { return m_selected; }
	void selected(int v) { m_selected = v; if (m_onSelected) m_onSelected(v); }

	void onSelected(const std::function<void(int)>& cb) { m_onSelected = cb; }

	virtual Size preferredSize() override;

private:
	std::function<void(int)> m_onSelected;
	Scroll* m_scroll{ nullptr };
	std::vector<std::string> m_list;
	int m_selected{ -1 };
};

#endif // LIST_H
