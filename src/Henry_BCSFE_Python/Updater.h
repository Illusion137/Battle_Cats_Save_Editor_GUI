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
	//DONT NEED FOR A WHILE

	extern bool update(Str latest_version, Str command = "py" );
	extern Str get_local_version();
	extern optional<tuple<Str, Str>> get_version_info();
	extern Str get_pypi_version(json data );
	extern Str get_latest_prerelease_version(json data);
	extern bool pypi_is_newer(Str local_version, Str pypi_version , bool remove_b = true);
	extern tuple<bool, Str> check_update(tuple<Str, Str> version_info);
}