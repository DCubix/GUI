#ifndef SYG_SPINNER_H
#define SYG_SPINNER_H

#include "widget.h"

class Spinner : public Widget {
	friend class GUI;
public:
	Spinner();

	virtual void onDraw(Renderer& renderer) override;
	virtual void onMove(int x, int y) override;
	virtual void onClick(int button, int x, int y) override;
	virtual void onDoubleClick(int button, int x, int y) override;
	virtual void onPress(int button, int x, int y) override;
	virtual void onRelease(int button, int x, int y) override;
	virtual void onScroll(int direction) override;
	virtual void onType(char chr) override;

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void onBlur() override;
	virtual void onKeyPress(int key, int mod) override;

	float value() const { return m_value; }
	void value(float v);

	float minimum() const { return m_min; }
	void minimum(float v) { m_min = v; }

	float maximum() const { return m_max; }
	void maximum(float v) { m_max = v; }

	float step() const { return m_step; }
	void step(float v) { m_step = v; }

	std::string suffix() const { return m_suffix; }
	void suffix(const std::string& v) { m_suffix = LL(v); }

	bool draggable() const { return m_draggable; }
	void draggable(bool v) { m_draggable = v; }

	void onChange(const std::function<void(float)>& cb) { m_onChange = cb; }

	float* userData() { return m_userData; }

protected:
	std::function<void(float)> m_onChange;

	float* m_userData{ nullptr };
	float m_value{ 0.0f }, m_min{ 0.0f }, m_max{ 1.0f }, m_step{ 0.1f };
	int m_decState{ 0 }, m_incState{ 0 };

	std::string m_suffix{ };
	int m_mx{ 0 };

	bool m_editing{ false }, m_draggable{ true };
	std::string m_valText{};
};

#endif // SYG_SPINNER_H
