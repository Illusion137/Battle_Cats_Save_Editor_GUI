#pragma once

#include "FileMananger.h"
#include "Widgets.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <optional>
#include <deque>

namespace BCats {
	namespace CatInfo {
		typedef unsigned int CatID;

		enum CatForm_ : char {
			CatForm_Base = '1',
			CatForm_Evolved = '2',
			CatForm_True = '3',
		};
		enum CatRarity_ {
			CatRarity_Normal,
			CatRarity_Special,
			CatRarity_Rare,
			CatRarity_SuperRare,
			CatRarity_UberSuperRare,
			CatRarity_LegendRare,
		};
		enum CatTarget_ {
			CatTarget_Red = 0,
			CatTarget_Black = 1,
			CatTarget_Floating = 2,
			CatTarget_Metal = 3,
			CatTarget_Angel = 4,
			CatTarget_Alien = 5,
			CatTarget_Zombie = 6,
			CatTarget_Aku = 7,
			CatTarget_Relic = 8,
			CatTarget_Traitless = 9,
			//Other Traits?
			CatTarget_Witch = 10,
			CatTarget_Apostle = 11,
			CatTarget_SuperlifeForm = 12
		};
		enum CatEffectAbility_ {
			CatEffectAbility_SingleTarget,
			CatEffectAbility_AreaAttack,
			CatEffectAbility_MultiHit,
			CatEffectAbility_LongDistance,
			CatEffectAbility_OmniStrike,
			CatEffectAbility_Weaken,
			CatEffectAbility_Freeze,
			CatEffectAbility_Slow,
			CatEffectAbility_AttacksOnly,
			CatEffectAbility_StrongAgainst,
			CatEffectAbility_Resistant,
			CatEffectAbility_InsanelyTough,
			CatEffectAbility_MassiveDamage,
			CatEffectAbility_InsaneDamage,
			CatEffectAbility_Knockback,
			CatEffectAbility_Warp,
			CatEffectAbility_Curse,
			CatEffectAbility_DodgeAttack,
			CatEffectAbility_Toxic,
			CatEffectAbility_Strengthen,
			CatEffectAbility_Survive,
			CatEffectAbility_BaseDestroyer,
			CatEffectAbility_Critical,
			CatEffectAbility_Burrow,
			CatEffectAbility_Revive,
			CatEffectAbility_Metal,
			CatEffectAbility_ZombieKiller,
			CatEffectAbility_Soulstrike,
			CatEffectAbility_Barrier,
			CatEffectAbility_Shield,
			CatEffectAbility_BarrierBreaker,
			CatEffectAbility_ShieldPiercing,
			CatEffectAbility_SavageBlow,
			CatEffectAbility_ExtraMoney,
			CatEffectAbility_WaveAttack,
			CatEffectAbility_MiniWave,
			CatEffectAbility_SurgeAttacks,
			CatEffectAbility_Aftermath,
			CatEffectAbility_WaveShield,
			CatEffectAbility_ColossusSlayer,
			CatEffectAbility_BehemothSlayer,
			CatEffectAbility_Kamikaze,
			CatEffectAbility_Immunities,
			CatEffectAbility_ImmuneToWeaken,
			CatEffectAbility_ImmuneToFreeze,
			CatEffectAbility_ImmuneToSlow,
			CatEffectAbility_ImmuneToKnockback,
			CatEffectAbility_ImmuneToWaves,
			CatEffectAbility_EvadeSurge,
			CatEffectAbility_WarpBlocker,
			CatEffectAbility_ImmuneToCurse,
			CatEffectAbility_ImmuneToToxic,
			CatEffectAbility_WitchKiller,
			CatEffectAbility_EvaAngelKiller
		};

		struct SaveData {
			std::string countryCode = "en";
			std::string version = "";
			std::string transferCode = "";
			std::string confirmationCode = "";
		};

		struct Cat {
			int level;
			int plusLevel;
			bool unlockedValue;
			CatForm_ form;
			CatForm_ maxFormBegin;
			std::string names[3];
			Widgets::Animation::ImageButtonAnimation rotateAnim;
			Widgets::Animation::ButtonAnimation unlockedAnim;
		};

		class Filters {
			Filters(Filters&) = delete;
			Filters();
		public:
			inline static Filters& get_instance();
			bool andSearch;
			bool owned;
			bool notOwned;
			bool hasTrueForm;
			std::optional<std::pair<int, int>> levelRange;
			std::optional<std::pair<int, int>> plusLevelRange;
			std::optional<std::vector<std::string>> names;
			std::optional<std::vector<int>> ids;
			std::deque<bool> rarities;
			std::deque<bool> targets;
			std::deque<bool> effectAbilities;

		private:
		};

		inline std::vector<int> catIDs = { };
		inline nlohmann::json catDataJson;

		extern inline bool is_valid_cat_form(int id);
		extern std::string get_cat_name_from_id(int id, CatForm_ form);
		extern void update_cat_ids_from_filters();
		extern bool check_cat_rarity_deque(std::deque<bool> rarities, CatRarity_ catRarity);
		extern bool check_cat_target_deque(std::deque<bool> targets, int unitID);
		extern bool compare_for_each_string(std::vector<std::string> strings, std::string theString);
		extern bool compare_for_each_int(std::vector<int> ints, int theInt);
	}
}