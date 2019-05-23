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
	void text(const std::string& v) { m_text = v; invalidate(); }

	Alignment textAlign() const { return m_textAlign; }
	void textAlign(Alignment v) { m_textAlign = v; invalidate(); }

	bool autoWidth() const { return m_autoWidth; }
	void autoWidth(bool v) { m_autoWidth = v; invalidate(); }

private:
	std::string m_text{ "Label" };
	Alignment m_textAlign{ Left };
	bool m_autoWidth{ false };
};

#endif // SYG_LABEL_H