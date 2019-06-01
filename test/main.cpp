#include <iostream>

#include "application.h"
#include "widgets/button.h"

class App : public Application {
public:
	virtual void onBuild(GUI* gui) override {
		gui->load(R"(
<root layout="stack">
	<list height="70" selected="2" width="240">
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
	<splitview height="120">
		<label text="This is supposed to be an extremely long string, to test word wrapping capabilities. Enjoy!!!" wordWrap="true" />
		<splitview orientation="horizontal">
			<label text="This is supposed to be an extremely long string, to test word wrapping capabilities. Enjoy!!!" wordWrap="true" />
			<button text="Test" />
		</splitview>
	</splitview>
	<button text="Button" height="20" width="120" />
	<check text="Check" selected="false" height="20" width="120" />
	<spinner suffix=" Value" value="0.5" height="20" width="120" />
	<scrollview height="180">
		<imageview src="out.png" />
	</scrollview>
</root>
)");
	}
};

int main(int argc, char** argv) {
	return App().run();
}
