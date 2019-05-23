#include <iostream>

#include "application.h"
#include "widgets/button.h"

class App : public Application {
public:
	virtual void onBuild(GUI* gui) override {
		gui->load(R"(
<root layout="stack">
	<button name="btnHello" text="Hello" />
	<imageview src="out.png" height="320" />
</root>
		)");

		Button* btnHello = gui->get<Button>("btnHello");
		btnHello->onClick([](int button, int x, int y) {

		});
	}
};

int main(int argc, char** argv) {
	return App().run();
}
