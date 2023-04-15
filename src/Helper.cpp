#include "Helper.h"

inline bool BCats::Helper::is_json_file(const std::string& filePath)
{
	boost::regex isJsonFileRegex(".+(\.json)");
	return boost::regex_search(filePath, isJsonFileRegex);
}

inline std::vector<std::string> BCats::Helper::split_string(const std::string& string, const char delemiter)
{
	return std::vector<std::string>();
}
