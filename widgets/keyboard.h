#ifndef SYG_KEYBOARD_H
#define SYG_KEYBOARD_H

#include <array>

#include "widget.h"

class Keyboard : public Widget {
	using NoteCallback = std::function<void(int, float)>;
public:
	Keyboard();

	virtual void onDraw(Renderer& renderer) override;

	virtual void onPress(int button, int x, int y) override;
	virtual void onRelease(int button, int x, int y) override;
	virtual void onMove(int x, int y) override;
	virtual void onExit() override;

	virtual void onKeyTap(int key, int mod) override;
	virtual void onKeyRelease(int key, int mod) override;

	void onNoteOn(const NoteCallback& cb) { m_onNoteOn = cb; }
	void onNoteOff(const NoteCallback& cb) { m_onNoteOff = cb; }

	void noteOn(int note, float velocity = 1.0f);
	void noteOff(int note, float velocity = 1.0f);

private:
	NoteCallback m_onNoteOn, m_onNoteOff;
	bool m_drag{ false };
	std::array<bool, 88> m_keys;
	int m_currentKey{ 0 };

	void handleClick(int x, int y);
};

#endif // SYG_KEYBOARD_H
