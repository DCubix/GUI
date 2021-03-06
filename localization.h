#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>
#include <map>
#include <locale>

#include <boost/locale.hpp>
namespace lc = boost::locale;

using Dict = std::map<std::string, std::string>;

class Locale {
public:
	static void init();
	static std::string get(const std::string& v);

private:
	static Dict m_dictionary;
	static std::string m_locale;
	static std::locale m_loc;
};

#define LL(x) Locale::get(x)

#endif // LOCALIZATION_H
