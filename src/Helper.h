#pragma once

#include <boost/regex.hpp>

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

namespace BCats {
	namespace Helper {
		extern inline bool is_json_file(const std::string& filePath);
		extern inline std::vector<std::string> split_string(const std::string& string, const char delemiter);
	}
}