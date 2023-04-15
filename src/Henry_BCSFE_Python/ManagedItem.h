#pragma once

#include "Python.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <unordered_map>

using json = nlohmann::json;
namespace BCSFE {
	using namespace std;

	enum DetailType {
		DetailType_GET,
		DetailType_USE
	};
	inline const unordered_map<DetailType, Str> DetailType_ = {
		{ DetailType_GET, "get" },{ DetailType_USE, "use" }
	};

	enum ManagedItemType {
		ManagedItemType_CATFOOD,
		ManagedItemType_RARE_TICKET,
		ManagedItemType_PLATINUM_TICKET,
		ManagedItemType_LEGEND_TICKET
	};
	inline const unordered_map<ManagedItemType, Str> ManagedItemType_ = {
		{ ManagedItemType_CATFOOD, "catfood" },{ ManagedItemType_RARE_TICKET, "rareTicket" },
		{ ManagedItemType_PLATINUM_TICKET, "platinumTicket" },{ ManagedItemType_LEGEND_TICKET, "legendTicket" }
	};

	class ManagedItem {
	public:
		ManagedItem(int amount, DetailType detailType, ManagedItemType managed_item_type);
		json to_dict();
	private:
		int m_amount;
		DetailType m_detailType;
		ManagedItemType m_managed_item_type;
		Str m_detail_code;
		int m_detail_created_at;
	};
}