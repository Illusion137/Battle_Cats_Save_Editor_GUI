#pragma once

#include "Python.h"

#include "ConfigManager.h"
#include "Tracker.h"
#include "ServerHandler.h"
#include "Patcher.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <optional>
#include <tuple>
#include <filesystem>

//REGEX FOR REMOVE TYPING :.+?(?=[,)])
/*
REGEX PARSE PYTHON
(def )(.+?(?=[(]))\(( ?(.+?)(?=:): (.+?)(?=[,)])(,?))?( ?(.+?)(?=:): (.+?)(?=[,)])(,?))?( ?(.+?)(?=:): (.+?)(?=[,)])(,?))?( ?(.+?)(?=:): (.+?)(?=[,)])(,?))?( ?(.+?)(?=:): (.+?)(?=[,)])(,?))?\) ?-> ?(.+?)(?=[ :]) ?:
CAPTURE GROUP REPLACE
extern $23 $2($5 $4$6 $9 $8$10 $13 $12$14 $17 $16$18 $21 $20$22);
*/
/*
REGEX GET ALL FUNCTIONS
def .+?->.+?:
*/
using json = nlohmann::json;
namespace BCSFE {
	using namespace std;
	namespace fs = std::filesystem;

	extern int                      get_time();
	extern Str                      get_iso_time();
	extern vector<Str>              get_dirs(Str path);
	extern void                     delete_dir(Str path);
	extern void                     create_dirs(Str path);
	extern vector<int>              offset_list(vector<int> lst, int offset);
	extern vector<Any>              copy_first_n(vector<Any> lst, int number);
	extern Str                      get_file(Str file_name);
	extern vector<Str>              get_files_in_dir(Str dir_path);
	extern vector<Str>              find_files_in_dir(Str dir_path, Str file_name);
	extern Str                      get_local_files_path();
	extern WStr                     read_file_string(Str file_path);
	//extern Generator[Any, Any, Any] chunks( lst, chunk_len );
	extern json                     frames_to_time(int frames);
	extern int                      clamp_int(int value);
	extern Bytes                    num_to_bytes(int num, int length);
	extern json                     seconds_to_time(int seconds);
	extern int                      time_to_seconds(json time);
	extern int                      time_to_frames(json time);
	extern IntNone                  check_int(Str value);
	extern optional<int>            check_int_max(Str value, optional<int> max_value = {} );
	extern vector<Str>              int_to_str_ls(vector<int> int_list);
	extern vector<int>              parse_int_list(vector<Str> lst, int offset);
	extern int                      clamp(int value, int min_value, int  max_value);
	extern Bytes                    write_file_bytes(Str file_path, Bytes data);
	extern char                     get_text_splitter(bool isjp);
	extern vector<tuple<Str, Str>>  get_save_file_filetype();
	extern Bytes                    read_file_bytes(Str file_path);
	extern void                     write_file_string(Str file_path, Str data);
	extern vector<int>              check_clamp(Any values, int max_value, int min_value = 0, int offset = -1);
	extern json                     encode_ls(vector<int> lst);
	extern vector<vector<Any>>      parse_int_list_list(vector<vector<Str>>list_of_lists);
	extern StrNone                  check_hex(Str value);
	extern StrNone                  check_dec(Str value);
	extern Str                      str_to_gv(const Str& game_version);
	extern Str                      gv_to_str(int game_version);
	extern Any                      load_json(Str json_path);
	extern bool                     is_jp(json save_stats);
	extern bool                     check_data_is_jp(json save_stats);
	extern void                     exit_editor();
	extern vector<int>              check_cat_ids(vector<int> cat_ids, json save_stats);
	extern int                      calculate_user_rank(json save_stats);
	extern Bytes                     write_save_data(Bytes save_data, Str country_code, Str path, bool prompt);
	extern Str                      select_dir(Str title, Str externault_dir);
	//extern bool                   run_in_parallel(fns);
	//extern bool                   run_in_background(func, Any]);
	extern Str                      get_cc(json save_stats);
	extern Str                      get_lang(bool jp);
	extern Str                      get_save_path_home();
	extern optional<Str>            save_file( Str title, vector<tuple<Str, Str>> file_types, Str path);
	extern Str                      select_file( Str title, vector<tuple<Str, Str>> file_types, Str externault_dir = "" , Str initial_file = "");
	extern Str                      get_home_path();
	extern Str                      get_default_save_name();
	extern json                     load_save_file(Str path);
	extern Str                      get_country_code(Bytes save_data);
	extern void                     ask_cc();
	extern bool                     export_json(json save_stats, Str path);
	extern StrNone                  load_json_handler(Str json_path);
}