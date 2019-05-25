#include <iostream>

#include "application.h"
#include "widgets/button.h"

class App : public Application {
public:
	virtual void onBuild(GUI* gui) override {
		gui->load(R"(
<root layout="border">
	<list param="top" height="80" selected="2">
		<item>Apples</item>
		<item>Oranges</item>
		<item>POMEGRANATES</item>
		<item>Pineapples</item>
		<item>Guarana</item>
		<item>Guava</item>
		<item>Passionfruit</item>
		<item>Blueberries</item>
		<item>Pizza</item>
		<item>Pineapple ON PIZZA :ok_hand:</item>
		<item>Bread</item>
	</list>
</root>
)");
	}
};

int main(int argc, char** argv) {
	return App().run();
}
