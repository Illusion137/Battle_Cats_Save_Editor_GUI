#pragma once

#include "Python.h"

#include "ManagedItem.h"

#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;
namespace BCSFE {
	using namespace std;

	class TrackerItem {
	public:
		TrackerItem(int value, ManagedItemType managed_item_type);
	private:
		int m_value;
		ManagedItemType m_managed_item_type;
	};

	class TrackerItems {
	public:
		TrackerItems(json tracker_data);
		void to_dict();
	private:
		json __dict__;
	};

	class Tracker {
	public:
		Tracker();
		Str get_path();
		json read_tracker();
		void write_tracker();
		//void update_tracker(int amount, ManagedItemType managed_item);
		void set_tracker_current();
		bool has_data();
		vector<ManagedItemType> parse_tracker_managed();
	private:
		TrackerItems items;
	};

	//extern None __init__(dict[str, int] self, tracker_data);
	//def get_path(self)->str :
	//def read_tracker(self)->dict[str, int] :
	//def has_data(self) -> bool :
	//def parse_tracker_managed(self)->list[managed_item.ManagedItem] :
}