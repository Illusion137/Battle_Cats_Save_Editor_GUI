#include "CatInfo.h"

BCats::CatInfo::Filters::Filters() {}

inline BCats::CatInfo::Filters& BCats::CatInfo::Filters::get_instance()
{
	static BCats::CatInfo::Filters instance;
	return instance;
}

std::string BCats::CatInfo::get_cat_name_from_id(int id, CatForm_ form)
{
	switch (form) {
		case(CatForm_Base):
			if (catDataJson[id].at("baseForm").at("name").is_string())
				return catDataJson[id].at("baseForm").at("name");
			break;
		case(CatForm_Evolved):
			if (catDataJson[id].at("evolvedForm").at("name").is_string())
				return catDataJson[id].at("evolvedForm").at("name");
			break;
		case(CatForm_True):
			if(catDataJson[id].at("trueForm").at("name").is_string())
				return catDataJson[id].at("trueForm").at("name");
			break;
	}
	return "";
}

inline bool BCats::CatInfo::is_valid_cat_form(int id )
{
	return catDataJson[id].at("trueForm").at("name") != 0;
}

void BCats::CatInfo::update_cat_ids_from_filters() {
	const int numOfIDS = FileManager::saveData.at("cats").size();
	
	const Filters& filters = Filters::get_instance();

	catIDs.clear();

	for (int i = 0; i < numOfIDS; i++) {
		if (filters.andSearch){
			try
			{
				if (
					(!filters.owned || (filters.owned && FileManager::saveData.at("cats")[i] == 1)) &&
					(!filters.notOwned || (filters.notOwned && FileManager::saveData.at("cats")[i] == 0)) &&
					(!filters.hasTrueForm || (filters.hasTrueForm && catDataJson.at(i).at("trueForm").at("name").is_string())) &&
					(!filters.ids || (std::find(filters.ids.value().begin(), filters.ids.value().end(), i) != filters.ids.value().end())) &&
					(std::all_of(filters.rarities.begin(), filters.rarities.end(), [](bool v) { return !v; }) ||
						check_cat_rarity_deque(filters.rarities, (CatRarity_)catDataJson.at(i).at("rarity"))) &&
					(std::all_of(filters.targets.begin(), filters.targets.end(), [](bool v) { return !v; }) ||
						check_cat_target_deque(filters.targets, i)) &&
					( (!filters.names || compare_for_each_string(filters.names.value(), get_cat_name_from_id(i, CatForm_Base))) ||
						(!filters.names || compare_for_each_string(filters.names.value(), get_cat_name_from_id(i, CatForm_Evolved))) ||
						(!filters.names || compare_for_each_string(filters.names.value(), get_cat_name_from_id(i, CatForm_True))) )
					)
				{
					catIDs.push_back(i);
				}
			}
			catch (const std::exception& e)
			{
				std::cout << '\n' << e.what() << '\n';
			}
		}
		else {
			try
			{
				if (
					(filters.owned && FileManager::saveData.at("cats")[i] == 1) ||
					(filters.notOwned && FileManager::saveData.at("cats")[i] == 0) ||
					(filters.hasTrueForm && catDataJson.at(i).at("trueForm").at("name").is_string()) ||
					(!std::all_of(filters.rarities.begin(), filters.rarities.end(), [](bool v) { return !v; }) && 
						check_cat_rarity_deque(filters.rarities, (CatRarity_)catDataJson.at(i).at("rarity"))) ||
					(!std::all_of(filters.targets.begin(), filters.targets.end(), [](bool v) { return !v; }) &&
						check_cat_target_deque(filters.targets, i) ) ||
					(!filters.ids || std::find(filters.ids.value().begin(), filters.ids.value().end(), i) != filters.ids.value().end()) || 
					(!filters.names || compare_for_each_string(filters.names.value(), get_cat_name_from_id(i, CatForm_Base))) ||
					(!filters.names || compare_for_each_string(filters.names.value(), get_cat_name_from_id(i, CatForm_Evolved))) ||
					(!filters.names || compare_for_each_string(filters.names.value(), get_cat_name_from_id(i, CatForm_True)))
					) 
				{
					catIDs.push_back(i);
				}
			}
			catch (const std::exception& e)
			{
				std::cout << '\n' << e.what() << '\n';
			}
		}
	}
}

bool BCats::CatInfo::check_cat_rarity_deque(std::deque<bool> rarities, CatRarity_ catRarity)
{
	bool result = false;
	for (int i = 0; i < 6; i++) {
		if (rarities[i] && (i == catRarity)) {
			result = true;
			break;
		}
	}
	return result;
}

bool BCats::CatInfo::check_cat_target_deque(std::deque<bool> targets, int unitID)
{
	bool result = false;
	std::vector<int> baseTraits = catDataJson.at(unitID).at("baseForm").at("traits");
	std::vector<int> evolvedTraits = catDataJson.at(unitID).at("evolvedForm").at("traits");
	std::vector<int> trueTraits = catDataJson.at(unitID).at("trueForm").at("traits");
	for (int i = 0; i < 10; i++) {
		if (targets[i] && ( std::find(baseTraits.begin(), baseTraits.end(), i)
				!= baseTraits.end() ) ) {
			result = true;
			break;
		}
	}
	for (int i = 0; i < 10; i++) {
		if (targets[i] && (std::find(evolvedTraits.begin(), evolvedTraits.end(), i)
			!= evolvedTraits.end())) {
			result = true;
			break;
		}
	}
	for (int i = 0; i < 10; i++) {
		if (targets[i] && (std::find(trueTraits.begin(), trueTraits.end(), i)
			!= trueTraits.end())) {
			result = true;
			break;
		}
	}
	return result;
}

bool BCats::CatInfo::compare_for_each_string(std::vector<std::string> strings, std::string theString)
{
	bool result = false;
	std::transform(theString.begin(), theString.end(), theString.begin(),
		[](unsigned char c) { return std::tolower(c); });
	for (int i = 0; i < strings.size(); i++) {
		std::transform(strings[i].begin(), strings[i].end(), strings[i].begin(),
			[](unsigned char c) { return std::tolower(c); });
		if (theString.find(strings[i]) != std::string::npos) {
			result = true;
			break;
		}
	}
	return result;
}

bool BCats::CatInfo::compare_for_each_int(std::vector<int> ints, int theInt)
{
	bool result = false;
	for (int i = 0; i < ints.size(); i++) {
		if (theInt == ints[i]) {
			result = true;
			break;
		}
	}
	return result;
}