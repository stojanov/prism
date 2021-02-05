#pragma once

#include <regex>
#include <string>
#include <vector>

namespace Prism::String
{
	inline std::vector<std::string> Split(const std::string& input, const std::string& regex) {
		std::regex re(regex);
		std::sregex_token_iterator
			first{ input.begin(), input.end(), re, -1 },
			last;
		return { first, last };
	}

	inline void LTrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
			}));
	}

	inline void RTrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	inline void Trim(std::string& s) {
		LTrim(s);
		RTrim(s);
	}
}
