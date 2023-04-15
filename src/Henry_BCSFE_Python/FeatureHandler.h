#pragma once

#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;
namespace BCSFE {
	using namespace std;

	typedef string Str;
	typedef void* Any;

	union StrNone {
		Str str;
		bool none;
	};

	extern json fix_elsewhere_old(json save_stats);
	extern json get_feature( Any selected_features, Str search_string, json results);
	extern json show_options(json save_stats, json features_to_use);
	extern json menu(json save_stats, StrNone path_save);
}