#pragma once

#include <nlohmann/json.hpp>

#include <iostream>
#include <optional>
#include <tuple>

using json = nlohmann::json;
namespace BCSFE {
	using namespace std;

	typedef char* Bytes;
	typedef string Str;
	typedef void* Any;

	union StrNone {
		Str str;
		bool none;
	};
	union IntNone {
		int i;
		bool none;
	};
	extern Str get_data_path();
	extern optional<vector<Str>> get_installed_battlecats_versions();
	extern optional<Str> pull_save_data(Str game_version);
	extern bool is_ran_as_root();
	extern bool rerun_game(Str version);
}