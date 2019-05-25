#ifndef SYG_BUTTON_H
#define SYG_BUTTON_H

#include "widget.h"

enum State {
	Normal = 0,
	Hover,
	Click,
	Disabled
};

class Button : public Widget {
public:
	Button();

	virtual void onDraw(Renderer& renderer) override;
	virtual void onPress(int button, int x, int y) override;
	virtual void onRelease(int button, int x, int y) override;
	virtual void onEnter() override;
	virtual void onExit() override;

	std::string text() const { return m_text; }
	void text(const std::string& v) { m_text = v; }

	virtual Size preferredSize() override;

private:
	State m_state{ State::Normal };
	std::string m_text{ "Button" };
	int m_textWidth;
};

#endif // SYG_BUTTON_H
