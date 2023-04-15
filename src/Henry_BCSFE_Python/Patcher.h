#pragma once

#include "Python.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <optional>
#include <tuple>

using json = nlohmann::json;
namespace BCSFE {

	extern Str get_md5_sum(Bytes data);
	extern Str get_save_data_sum(Bytes save_data , Str game_version);
	extern StrNone detect_game_version(Bytes save_data);
	extern Bytes patch_save_data(Bytes save_data , Str game_version);
}