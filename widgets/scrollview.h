#ifndef SCROLLVIEW_H
#define SCROLLVIEW_H

#include "scroll.h"
#include <memory>

class ScrollView : public Widget {
public:
	ScrollView();
	virtual ~ScrollView() = default;

	virtual void onDraw(Renderer& renderer) override;

	Widget* widget() { return m_widget; }
	void widget(Widget* w);

private:
	std::unique_ptr<Scroll> m_hscroll, m_vscroll;
	Widget* m_widget{ nullptr };
};

#endif // SCROLLVIEW_H
