#pragma once

#include <nlohmann/json.hpp>

#include <iostream>
#include <vector>
#include <optional>

using json = nlohmann::json;
namespace BCSFE {
    #define URL "https://raw.githubusercontent.com/fieryhenry/BCData/master/"

    using namespace std;

    typedef char* Bytes;
    typedef string Str;

    extern Bytes download_file(Str game_version, Str pack_name, Str file_name, bool get_data = true, bool print_progress = true);
    extern optional<vector<Str>> get_latest_versions();
    extern optional<Str> get_latest_version(bool is_jp);
    extern optional<Bytes> get_file_latest(Str pack_name, Str file_name, bool is_jp);
    extern optional<Bytes> get_file_latest_path(Str path, bool is_jp);
    extern optional<Str> get_path(Str pack_name, Str file_name, bool is_jp);
    extern void check_remove(Str new_version, bool is_jp);
    extern void check_remove_handler();
}