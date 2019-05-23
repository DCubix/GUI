#ifndef SYG_CHECK_H
#define SYG_CHECK_H

#include "widget.h"

class Check : public Widget {
public:

	virtual void onClick(int button, int x, int y) override;
	virtual void onDraw(Renderer& renderer) override;

	std::string text() const { return m_text; }
	void text(const std::string& v) { m_text = v; invalidate(); }

	bool checked() const { return m_checked; }
	void checked(bool v) { m_checked = v; invalidate(); }
	
	void onChecked(const std::function<void(bool)>& cb) { m_onChecked = cb; }

private:
	std::function<void(bool)> m_onChecked;
	std::string m_text{ "Check" };
	bool m_checked;
};

#endif // SYG_CHECK_H