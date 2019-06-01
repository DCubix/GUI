#ifndef SCROLLVIEW_H
#define SCROLLVIEW_H

#include "scroll.h"
#include <memory>

class ScrollView : public Widget {
public:
	virtual void onDraw(Renderer& renderer) override;

	Widget* widget() { return m_widget; }
	void widget(Widget* w);

private:
	Scroll *m_hscroll{ nullptr }, *m_vscroll{ nullptr };
	Widget* m_widget{ nullptr };
};

#endif // SCROLLVIEW_H
