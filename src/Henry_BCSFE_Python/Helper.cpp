#include "Helper.h"

Str BCSFE::str_to_gv(const Str& game_version)
{
	///Turn a game version with semantic versioning to integer representation
	vector<Str> split_gv = split(game_version, '.');
	if (split_gv.size() == 2)
		split_gv.push_back("0");
	Str final = "";
	for (Str split : split_gv) {
		zfill(split, 2);
		final += split;
	}
	lstrip(final, '0');
	return final;
}

Str BCSFE::gv_to_str(int game_version)
{
	///Turn a game version with integer representation to semantic versioning
	Str s_game_version = to_string(game_version);
	zfill(s_game_version, 6);
	vector<Str> split_gv;
	split_gv.reserve(3);
	split_gv.push_back( to_string( stoi( s_game_version.substr(0, 1) ) ) );
	split_gv.push_back( to_string( stoi( s_game_version.substr(2, 3) ) ) );
	split_gv.push_back( to_string( stoi( s_game_version.substr(4, 5) ) ) );

	return join(split_gv, '.');
}

Any BCSFE::load_json(Str json_path)
{
	///Load a json file
	return json::parse(read_file_string(json_path));
}

bool BCSFE::is_jp(json save_stats)
{
	///Check if the save is a Japanese save
	return save_stats["version"] == "jp";
}

bool BCSFE::check_data_is_jp(json save_stats)
{
	///Check if the save data is a Japanese save, checking the config file
	if (get_config_value_category("EDITOR", "ONLY_GET_EN_DATA"))
		return false;
	return is_jp(save_stats);
}

void BCSFE::exit_editor()
{
	exit(1);
}

BCSFE::vector<int> BCSFE::check_cat_ids(vector<int> cat_ids, json save_stats)
{
	///Check if a list of cat ids is valid
	vector<int> new_cat_ids;
	new_cat_ids.reserve(cat_ids.size());
	for (int cat_id : cat_ids) {
		if (cat_id > save_stats["cats"].size() - 1) {
			//colored_text(f"Invalid cat id {cat_id}", base = RED)
			continue;
		}
		new_cat_ids.push_back(cat_id);
	}
	return new_cat_ids;
}

int BCSFE::calculate_user_rank(json save_stats)
{
	///Calculate the user rank
	int user_rank = 0;
	
	int cat_id = 0;
	for (int cat_flag : save_stats["cats"]) {
		if (cat_flag == 0)
			continue;
		user_rank += save_stats["cat_upgrades"]["Base"][cat_id] + 1;
		user_rank += save_stats["cat_upgrades"]["Plus"][cat_id];
		cat_id++;
	}
	for (int skill_id = 0; skill_id < save_stats["blue_upgrades"]["Base"].size(); skill_id++ ) {

		if (skill_id == 1)
			continue;
		user_rank += save_stats["blue_upgrades"]["Base"][skill_id] + 1;
		user_rank += save_stats["blue_upgrades"]["Plus"][skill_id];
	}
	return user_rank;
}

Bytes BCSFE::write_save_data(Bytes save_data, Str country_code, Str path, bool prompt)
{
	///Patch the save data and write it

	save_data = patch_save_data(save_data, country_code);
	if (prompt) {
		StrNone new_path = save_file("Save File", get_save_file_filetype(), path);
		if (!new_path)
			//colored_text("Save cancelled", new = RED);
			return;
		path = new_path.value();
	}
	write_file_bytes(path, save_data);
	//colored_text(f"Saved to: &{os.path.abspath(path)}&", new=GREEN)
	return save_data;
}

Str BCSFE::select_dir(Str title, Str externault_dir)
{
	return Str();
}

Str BCSFE::get_cc(json save_stats)
{
	return Str();
}

Str BCSFE::get_lang(bool jp)
{
	return Str();
}

Str BCSFE::get_save_path_home()
{
	return Str();
}

BCSFE::optional<Str> BCSFE::save_file(Str title, vector<tuple<Str, Str>> file_types, Str path)
{
	return optional<Str>();
}

Str BCSFE::select_file(Str title, vector<tuple<Str, Str>> file_types, Str externault_dir, Str initial_file)
{
	return Str();
}

Str BCSFE::get_home_path()
{
	return Str();
}

Str BCSFE::get_default_save_name()
{
return Str();
}

json BCSFE::load_save_file(Str path)
{
	return json();
}

Str BCSFE::get_country_code(Bytes save_data)
{
	return Str();
}

void BCSFE::ask_cc()
{
}

bool BCSFE::export_json(json save_stats, Str path)
{
	return false;
}

StrNone BCSFE::load_json_handler(Str json_path)
{
	return StrNone();
}

int BCSFE::get_time()
{
	///Get current time in seconds
	time_t seconds;
	seconds = time(NULL);
	return seconds;
}

Str BCSFE::get_iso_time()
{
	///Get the current time in iso format
	time_t     now = time(0);
	struct tm  tstruct;
	char       buffer[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", &tstruct);

	return Str(buffer);
}

BCSFE::vector<Str> BCSFE::get_dirs(Str path)
{
	///Get all directories in a path
	vector<Str> filePathsInDir;
	for (const auto& entry : fs::directory_iterator(path))
		if (fs::is_directory(entry))
			filePathsInDir.push_back((entry.path().string()));
	return filePathsInDir;
}

void BCSFE::delete_dir(Str path)
{
	///Delete a directory and all of its contents
	fs::remove_all(fs::path(path));
}

void BCSFE::create_dirs(Str path)
{
	///Create directories if they don't exist
	fs::create_directory(fs::path(path));
}

BCSFE::vector<int> BCSFE::offset_list(vector<int> lst, int offset)
{
	///Offset each value in an list by a certain amount
	vector<int> new_list = {};
	for (int item : lst)
		new_list.push_back(item + offset);
	return new_list;
}

BCSFE::vector<Any> BCSFE::copy_first_n(vector<Any> lst, int number)
{
	///Get the nth item in a list of lists
	vector<Any> new_list = {};
	for (Any item : lst)
		new_list.push_back(item[number]);
	return new_list;
}

Str BCSFE::get_file(Str file_name)
{
	///Get file in files folder
	Str file_path = get_local_files_path() + "\\" + file_name;
	return file_path;
}

BCSFE::vector<Str> BCSFE::get_files_in_dir(Str dir_path)
{
	///Get all files in a directory
	vector<Str> files = {};
	for (const auto& file : fs::directory_iterator(dir_path)) {
		Str file_path = dir_path + "\\" + file.path().string();
		if (fs::exists(file_path)){
			files.push_back(file_path);
		}
	}
	return files;
}

BCSFE::vector<Str> BCSFE::find_files_in_dir(Str dir_path, Str file_name)
{
	///Find all files in a directory with a certain name
	vector<Str> files = {};
	for (const auto& file : fs::directory_iterator(dir_path)) {
		if (file.path().string().find(file_name) != Str::npos)
			files.push_back(file.path().string());
	}
	return files;
}

Str BCSFE::get_local_files_path()
{
	///Get the local files path
	fs::path localFilePath = fs::current_path();
	Str dir_path = localFilePath.string() + "\\files";
	return dir_path;
}

WStr BCSFE::read_file_string(Str file_path)
{
	///Reads a file and returns its contents as a string
	try
	{
		WStr file = read_file_utf8(file_path.c_str());
		return file;
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

json BCSFE::frames_to_time(int frames)
{
	///Turn frames into hours, minutes, seconds, frames
	frames = clamp_int(frames);
	int seconds = frames / 30;
	frames = frames % 30;
	int minutes = seconds / 60;
	seconds = seconds % 60;
	int hours = minutes / 60;
	minutes = minutes % 60;
	return { {"hh", hours}, {"mm" , minutes}, {"ss" , seconds}, {"frames" , frames} };
}

int BCSFE::clamp_int(int value)
{
	///Clamp an integer to the range of a signed 32 bit integer
	///Args:
	///		value(int) : The value to clamp
	///Returns:
	///		int: The clamped value
	return clamp(value, 0, INT_MAX);
}

Bytes BCSFE::num_to_bytes(int num, int length)
{
	///Turn number into little endian bytes
	return to_bytes(num, length, true);
}

json BCSFE::seconds_to_time(int seconds)
{
	///Turn seconds into hours, minutes, seconds
	int minutes = seconds / 60; 
	seconds = seconds % 60;
	int hours = minutes / 60;
	minutes = minutes % 60;
	return { {"hh", hours}, {"mm", minutes}, {"ss", seconds} };
}

int BCSFE::time_to_seconds(json time)
{
	///Turn hours, minutes, seconds into seconds
	int seconds = time["ss"];
	seconds += time["mm"] * 60;
	seconds += time["hh"] * 60 * 60;
	return seconds;
}

int BCSFE::time_to_frames(json time)
{
	///Turn hours, minutes, seconds, frames into frames
	int total_frames = time["frames"];
	total_frames += time["ss"] * 30;
	total_frames += time["mm"] * 60 * 30;
	total_frames += time["hh"] * 60 * 60 * 30;
	total_frames = clamp_int(total_frames);
	return total_frames;
}

IntNone BCSFE::check_int(Str value)
{
	///Check if a string is an integer
	trim(value);
	std::string::const_iterator it = value.begin();
	while (it != value.end() && std::isdigit(*it)) ++it;
	bool is_int = !value.empty() && it == value.end();
	if (is_int)
		return stoi(value);
	return {};
}

BCSFE::optional<int> BCSFE::check_int_max(Str value, optional<int> max_value)
{
	IntNone val = check_int(value);
	if (!val)
		return {};
	if (max_value)
		return clamp(val.value(), 0, max_value.value());
	return clamp_int(val.value());
}

BCSFE::vector<Str> BCSFE::int_to_str_ls(vector<int> int_list)
{
	///Turn list of ints to list of strings
	vector<Str> str_list = {};
	for (int i : int_list)
		str_list.push_back(to_string(i));
	return str_list;
}

BCSFE::vector<int> BCSFE::parse_int_list(vector<Str> lst, int offset)
{
	///Turn string list to int list
	vector<int> new_list = {};
	for (Str item : lst) {
		try {
			new_list.push_back(stoi(item) + offset);
		}
		catch (const std::exception&){
			continue;
		}
	}
	return new_list;
}

int BCSFE::clamp(int value, int min_value, int max_value)
{
	///Clamp a value between two values
	return max(min(value, max_value), min_value);
}

Bytes BCSFE::write_file_bytes(Str file_path, Bytes data)
{
	///Write file as bytes
	try {
		ofstream file(file_path, ios::out | ios::binary);
		file.write(reinterpret_cast<const char*>(data.data()), data.size());
	}
	catch (const std::exception& e) {
		throw e;
		//Exception("Permission denied: " + file_path) from err
	}
	return data;
}

char BCSFE::get_text_splitter(bool isjp)
{
	///Get the text splitter for the current save stats
	if (isjp)
		return ',';
	return '|';
}

BCSFE::vector<BCSFE::tuple<Str, Str>> BCSFE::get_save_file_filetype()
{
	///Get the file types for the save file
	return { {"Battle Cats Save Files", "*SAVE_DATA*"}, {"All Files", "*.*"} };
}

Bytes BCSFE::read_file_bytes(Str file_path)
{
	///Read file as bytes
	ifstream file(file_path, ios::in | ios::binary);
	file.unsetf(std::ios::skipws);

	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	Bytes vec;
	vec.reserve(fileSize);

	vec.insert(vec.begin(),
		std::istream_iterator<Byte>(file),
		std::istream_iterator<Byte>());

	return vec;
}

void BCSFE::write_file_string(Str file_path, Str data)
{

}

BCSFE::vector<int> BCSFE::check_clamp(Any values, int max_value, int min_value, int offset)
{
	return vector<int>();
}

json BCSFE::encode_ls(vector<int> lst)
{
	return json();
}

BCSFE::vector<BCSFE::vector<Any>> BCSFE::parse_int_list_list(vector<vector<Str>> list_of_lists)
{
	return vector<vector<Any>>();
}

StrNone BCSFE::check_hex(Str value)
{
	return StrNone();
}

StrNone BCSFE::check_dec(Str value) {
	trim(value);
	int parsed = 0;
	std::istringstream(value) >> parsed;
	if (std::to_string(parsed) == value)
		return value;
	return { };
}