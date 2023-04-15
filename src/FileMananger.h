#pragma once

#include "Settings.h"

#include <nlohmann/json.hpp>

#include <boost/regex.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <filesystem>
#include <exception>

namespace BCats {
	namespace FileManager {
		extern inline std::string get_save_folder_path();
		extern inline void copy_file_to(const std::string& filePathFrom, const std::string& filePathTo);
		extern std::string find_unused_file_name_in_saves_folder();
		extern void fix_json_save_file(const std::string& filePath);

		inline std::string saveFilePath;
		inline std::string saveFileBackupPath;

		inline nlohmann::json saveData;
	}
}