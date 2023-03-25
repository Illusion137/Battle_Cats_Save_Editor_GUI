#pragma once
#include <iostream>

namespace BCats {
	namespace Data {
		enum CountryCode { 
			CountryCode_NONE,
			CountryCode_EN,
			CountryCode_JP
		};
		enum Version { 
			Version_NONE, 
			Version_12_1_0, 
			Version_1_0_0 
		};
		struct SaveData {
			CountryCode countryCode = CountryCode_NONE;
			Version version = Version_NONE;
			std::string transferCode = "";
			std::string confirmationCode = "";
		};
		enum CatRarity { 
			CatRarity_NORMAL      = 0x000001,
			CatRarity_SPECIAL     = 0x000010,
			CatRarity_RARE        = 0x000100,
			CatRarity_SUPER_RARE  = 0x001000,
			CatRarity_UBER_RARE   = 0x010000,
			CatRarity_LEGEND_RARE = 0x100000,
			CatRarity_ALL         = 0x111111
		};
		struct Cat {
			CatRarity rarity;
		};
	}
}