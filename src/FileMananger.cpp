#include "FileMananger.h"

inline std::string BCats::FileManager::get_save_folder_path()
{
	return std::filesystem::current_path().string() + "\\SAVES\\";
}

inline void BCats::FileManager::copy_file_to(const std::string& filePathFrom, const std::string& filePathTo)
{
    std::filesystem::copy_file(filePathFrom, filePathTo, std::filesystem::copy_options::overwrite_existing);
}

std::string BCats::FileManager::find_unused_file_name_in_saves_folder()
{
    std::ifstream searchFile;
    int counter = 0;
    searchFile.open(get_save_folder_path() + "SAVE-" + std::to_string(counter) + ".json");
    
    while (searchFile.good()) {
        searchFile.close();
        counter++;
        if (counter >= MAX_SAVES_BEFORE_OVERWRITE)
            break;
        searchFile.open(get_save_folder_path() + "SAVE-" + std::to_string(counter) + ".json");
    }
    
    searchFile.close();

    return get_save_folder_path() + "SAVE-" + std::to_string(counter) + ".json";
}

void BCats::FileManager::fix_json_save_file(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (file.good()) {
        std::string fileString ((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        file.close();

        ///SLOWER BUT KEEP IN CASE
        //boost::regex fixJsonExtraData("(\\d{20,})");
        //std::string fixedString = regex_replace(fileString, fixJsonExtraData, "\"$1\"");

        boost::regex fixJsonExtraData("(?s)(extra_data\":(.+?):(.+?))(\\d+)");
        std::string fixedString = regex_replace(fileString, fixJsonExtraData, "$1\"ILLUSION\"");

        std::ofstream clearAndFixFile(filePath, std::ios::out | std::ios::trunc);
        clearAndFixFile << fixedString;
        clearAndFixFile.close();
    }
    else
        file.close();
}
