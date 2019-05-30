#include "gui.h"

#include <algorithm>

#include "sdl_headers.h"

#include "widgets/spinner.h"
#include "widgets/label.h"
#include "widgets/button.h"
#include "widgets/check.h"
#include "widgets/imageview.h"
#include "widgets/scroll.h"
#include "widgets/list.h"
#include "widgets/scrollview.h"

#include "localization.h"

GUI::GUI() {
	m_events = std::make_unique<EventHandler>();
	m_renderer = std::make_unique<Renderer>();

	m_root = create<Panel>();
	m_root->gridWidth(16);
	m_root->gridHeight(16);
	m_root->spacing(4);
	m_root->padding(2);

	setlocale(LC_ALL, "");
}

static void loadDefaultWidgetAttributes(Widget* widget, pugi::xml_node& node) {
	static const std::string params[] = {
		"left", "right", "bottom", "top", "center"
	};

	int x = node.attribute("x").as_int(0);
	int y = node.attribute("y").as_int(0);
	int width = node.attribute("width").as_int(22);
	int height = node.attribute("height").as_int(22);
	bool visible = node.attribute("visible").as_bool(true);
	bool enabled = node.attribute("enabled").as_bool(true);
	int row = node.attribute("row").as_int(0);
	int col = node.attribute("col").as_int(0);
	int rowSpan = node.attribute("rowSpan").as_int(1);
	int colSpan = node.attribute("colSpan").as_int(1);
	int layoutParam = -1;

	std::string lparam = node.attribute("param").as_string();
	std::transform(lparam.begin(), lparam.end(), lparam.begin(), ::tolower);

	auto it = std::find(std::begin(params), std::end(params), lparam);
	if (it != std::end(params)) {
		layoutParam = int(std::distance(std::begin(params), it));
	}

	widget->bounds().x = x;
	widget->bounds().y = y;
	widget->bounds().width = width;
	widget->bounds().height = height;
	widget->visible(visible);
	widget->enabled(enabled);
	widget->configure(row, col, colSpan, rowSpan);
	widget->layoutParam(layoutParam);
	widget->name(node.attribute("name").as_string(""));
	widget->autoSize(node.attribute("autoSize").as_bool(false));
}

static void setupPanel(Panel* panel, pugi::xml_node& node) {
	bool drawBg = node.attribute("background").as_bool(true);
	int gridW = node.attribute("gridWidth").as_int(10);
	int gridH = node.attribute("gridHeight").as_int(10);
	int pad = node.attribute("padding").as_int(4);
	int space = node.attribute("spacing").as_int(4);
	std::string layout = node.attribute("layout").as_string();
	std::transform(layout.begin(), layout.end(), layout.begin(), ::tolower);

	panel->drawBackground(drawBg);
	panel->gridWidth(gridW);
	panel->gridHeight(gridH);
	panel->padding(pad);
	panel->spacing(space);
	loadDefaultWidgetAttributes(panel, node);

	if (layout == "border") {
		panel->setLayout(new BorderLayout());
	} else if (layout == "grid") {
		panel->setLayout(new GridLayout());
	} else if (layout == "stack") {
		panel->setLayout(new StackLayout());
	} else if (layout == "flow") {
		panel->setLayout(new FlowLayout());
	}
}

static Widget* loadWidget(GUI* gui, Panel* parent, pugi::xml_node& node);
static Panel* loadPanel(GUI* gui, Panel* parent, pugi::xml_node& node) {
	Panel* panel = gui->create<Panel>();
	setupPanel(panel, node);

	for (auto& c : node.children()) {
		if (std::string(c.name()) == "panel") {
			loadPanel(gui, panel, c);
		} else {
			loadWidget(gui, panel, c);
		}
	}

	if (parent != nullptr) {
		parent->add(panel);
	}
	return panel;
}

Widget* loadWidget(GUI* gui, Panel* parent, pugi::xml_node& node) {
	static const std::string params[] = {
		"left", "center", "right"
	};

	Widget* w = nullptr;
	std::string name = node.name();
	if (name == "label") {
		int align = 0;
		std::string lparam = node.attribute("align").as_string();
		std::transform(lparam.begin(), lparam.end(), lparam.begin(), ::tolower);

		auto it = std::find(std::begin(params), std::end(params), lparam);
		if (it != std::end(params)) {
			align = int(std::distance(std::begin(params), it));
		}

		w = gui->create<Label>();
		((Label*) w)->text(node.attribute("text").as_string());
		((Label*) w)->textAlign(Label::Alignment(align));
	} else if (name == "button") {
		w = gui->create<Button>();
		((Button*) w)->text(node.attribute("text").as_string());
	} else if (name == "check") {
		w = gui->create<Check>();
		((Check*) w)->text(node.attribute("text").as_string());
		((Check*) w)->checked(node.attribute("checked").as_bool(false));
	} else if (name == "spinner") {
		w = gui->create<Spinner>();
		((Spinner*) w)->suffix(node.attribute("suffix").as_string());
		((Spinner*) w)->minimum(node.attribute("min").as_float());
		((Spinner*) w)->maximum(node.attribute("max").as_float(1.0f));
		((Spinner*) w)->value(node.attribute("value").as_float(((Spinner*) w)->minimum()));
		((Spinner*) w)->step(node.attribute("step").as_float(0.1f));
		((Spinner*) w)->draggable(node.attribute("draggable").as_bool(true));
	} else if (name == "imageview") {
		w = gui->create<ImageView>();
		std::string fileName = node.attribute("src").as_string();
		if (!fileName.empty()) {
			((ImageView*) w)->image(gui->createImage(fileName));
		}
	} else if (name == "scroll") {
		w = gui->create<Scroll>();

		std::string ori = node.attribute("orientation").as_string("horizontal");
		std::transform(ori.begin(), ori.end(), ori.begin(), ::tolower);

		((Scroll*) w)->minimum(node.attribute("min").as_float());
		((Scroll*) w)->maximum(node.attribute("max").as_float(100.0f));
		((Scroll*) w)->value(node.attribute("value").as_float(((Scroll*) w)->minimum()));
		((Scroll*) w)->step(node.attribute("step").as_float(1.0f));
		((Scroll*) w)->orientation(
					ori == "horizontal" ? Scroll::Horizontal :
					ori == "vertical" ? Scroll::Vertical : Scroll::Horizontal
		);
	} else if (name == "list") {
		w = gui->create<List>();
		((List*) w)->selected(node.attribute("selected").as_int(-1));

		std::vector<std::string> list;
		for (auto& n : node.children("item")) {
			list.push_back(n.first_child().value());
		}
		((List*) w)->list(list);
	} else if (name == "scrollview") {
		w = gui->create<ScrollView>();
		auto wd = node.first_child();
		if (wd) {
			Widget* widget = nullptr;
			if (std::string(wd.name()) == "panel") {
				widget = loadPanel(gui, nullptr, wd);
			} else {
				widget = loadWidget(gui, nullptr, wd);
			}
			((ScrollView*) w)->widget(widget);
		}
	}

	if (w != nullptr) {
		loadDefaultWidgetAttributes(w, node);
		if (parent != nullptr) {
			parent->add(w);
		}
	}
	return w;
}

void GUI::load(const std::string& xmlData) {
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_string(xmlData.data());
	if (!res) {
		return;
	}

	if (std::string(doc.root().first_child().name()) != "root") {
		return;
	}

	auto root = doc.root().first_child();
	loadDefaultWidgetAttributes(m_root, root);
	setupPanel(m_root, root);

	for (auto& node : root.children()) {
		if (std::string(node.name()) == "panel") {
			loadPanel(this, m_root, node);
		} else {
			loadWidget(this, m_root, node);
		}
	}
}

Image* GUI::createImage(int width, int height) {
	m_images.push_back(std::make_unique<Image>());
	m_images.back()->load(*m_renderer.get(), PixelData(width, height));
	return m_images.back().get();
}

Image* GUI::createImage(const std::string& fileName) {
	m_images.push_back(std::make_unique<Image>());
	m_images.back()->load(*m_renderer.get(), PixelData(fileName));
	return m_images.back().get();
}

Spinner* GUI::spinner(
	float* value, float vmin, float vmax,
	const std::string& suffix,
	bool drag,
	const std::function<void()> onChange,
	float step
) {
	Spinner* spn = create<Spinner>();
	spn->m_userData = value;
	spn->minimum(vmin);
	spn->maximum(vmax);
	spn->draggable(drag);
	spn->suffix(suffix);
	spn->step(step);
	spn->value(*value);
	spn->onChange([=](float val) {
		float* _value = spn->userData();
		(*_value) = val;
		if (onChange) onChange();
	});
	return spn;
}

void GUI::destroy(Widget* widget) {
	auto it = std::remove_if(
		m_widgets.begin(),
		m_widgets.end(),
		[&](std::unique_ptr<Widget> const& p) {
			return p.get() == widget;
		});
	if (it != m_widgets.end()) {
		it->reset();
		m_widgets.erase(it, m_widgets.end());
	}
	m_clear = true;
}

void GUI::render(int width, int height) {
	m_root->bounds().width = width;
	m_root->bounds().height = height;

	// Clean orphans (except main panel)
	std::vector<int> orphans;
	for (int i = 1; i < m_widgets.size(); i++) {
		if (m_widgets[i]->parent() == nullptr) {
			orphans.push_back(i);
		}
	}
	std::reverse(orphans.begin(), orphans.end());
	for (int i : orphans) {
		m_widgets.erase(m_widgets.begin() + i);
	}
	//

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_renderer->begin(width, height);

	for (auto&& widget : m_widgets) {
		if (widget->visible()) {
			widget->onDraw(*m_renderer.get());
		}
	}

	m_renderer->end();
}
