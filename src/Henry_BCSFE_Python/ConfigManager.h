#pragma once

#include "Python.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <optional>

using json = nlohmann::json;
namespace BCSFE {
	using namespace std;

	extern Any get_config_value_category(Str category, Str key);
	extern json get_config_file();
	extern string get_config_path();
	extern void set_config_path(Str path);
	extern Any get_config_value(Str str);
	extern void set_config_setting_category(Str category, Str key, Any value);
	extern void set_config_setting(Str setting, Any value);
	extern void create_config_file(optional<Str> config_path = {});
	extern void edit_default_gv();
	extern void edit_default_save_file_path();
	extern void edit_fixed_save_path();
	extern void edit_locale();
	extern void edit_editor_settings();
	extern void edit_start_up_settings();
	extern void edit_save_changes_settings();
	extern void edit_server_settings();
	extern void edit_config_path();
}