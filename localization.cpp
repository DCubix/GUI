#include "localization.h"

#include <iostream>
#include <fstream>
#include <sstream>

Dict Locale::m_dictionary;
std::string Locale::m_locale = "C";
std::locale Locale::m_loc;

void Locale::init() {
	// Get the current locale
	setlocale(LC_ALL, "");

	lc::generator gen{};
	m_loc = std::locale(gen(""));
	std::locale::global(m_loc);

	std::cout.imbue(m_loc);

	//loc = loc.substr(0, loc.find_first_of('.'));
	auto&& fc = std::use_facet<lc::info>(m_loc);
	m_locale = fc.language() + "_" + fc.country();

	// Find the locale file and load it if it exists
	std::ifstream fp("locale/" + m_locale + ".csv");
	if (fp.good()) {
		std::string line;
		while (std::getline(fp, line)) {
			std::istringstream ss(line);
			std::string key, value;

			std::getline(ss, key, ';');
			std::getline(ss, value, ';');

			m_dictionary.insert({ key, value });
		}
		fp.close();
	} else {
		std::cerr << "A locale file for " << m_locale << " was not found." << std::endl;
	}
}

std::string Locale::get(const std::string& v) {
	auto pos = m_dictionary.find(v);
	if (pos != m_dictionary.end()) {
		return pos->second;
	}
	return v;
}
