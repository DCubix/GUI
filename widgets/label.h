#ifndef SYG_LABEL_H
#define SYG_LABEL_H

#include "widget.h"

class Label : public Widget {
public:
	enum Alignment {
		Left = 0, Center, Right
	};

	virtual void onDraw(Renderer& renderer) override;

	std::string text() const { return m_text; }
	void text(const std::string& v) { m_text = v; }

	Alignment textAlign() const { return m_textAlign; }
	void textAlign(Alignment v) { m_textAlign = v; }

	bool wordWrap() const { return m_wordWrap; }
	void wordWrap(bool ww) { m_wordWrap = ww; }

	virtual Size preferredSize() override;

private:
	std::string m_text{ "Label" };
	Alignment m_textAlign{ Left };
	int m_textWidth;
	bool m_wordWrap{ false };
};

#endif // SYG_LABEL_H
