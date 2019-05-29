#include "localization.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <locale.h>

Dict Locale::m_dictionary;
std::string Locale::m_locale = "C";

void Locale::init() {
	// Get the current locale
	setlocale(LC_ALL, "");
	std::string loc = setlocale(LC_ALL, nullptr);
	//loc = loc.substr(0, 5); // pt_BR, en_US
	m_locale = loc;

	// Find the locale file and load it if it exists
	std::ifstream fp("locale/" + loc + ".csv");
	if (fp.good()) {
		std::string line;
		while (std::getline(fp, line)) {
			std::istringstream ss(line);
			std::string key, value;

			std::getline(ss, key, ',');
			std::getline(ss, value, ',');

			m_dictionary.insert({ key, value });
		}
		fp.close();
	} else {
		std::cerr << "A locale file for " << loc << " was not found." << std::endl;
	}
}

std::string Locale::get(const std::string& v) {
	auto pos = m_dictionary.find(v);
	if (pos != m_dictionary.end()) {
		return pos->second;
	}
	return v;
}
