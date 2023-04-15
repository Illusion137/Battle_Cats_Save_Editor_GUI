#pragma once

#include <nlohmann/json.hpp>

#include <iostream>
#include <optional>
#include <tuple>

using json = nlohmann::json;
namespace BCSFE {
	using namespace std;

	typedef char* Bytes;
	typedef string Str;
	typedef void* Any;

	union StrNone {
		Str str;
		bool none;
	};
	union IntNone {
		int i;
		bool none;
	};

	extern int address = 0;
	extern Bytes save_data_g = nullptr;

	//class ClearedSlots:
	//extern collections.Orderedjson re_order(json data);
	extern void set_address(int val);
	extern json next_int_len(int number);
	extern json generate_empty_len(int length);
	extern int next_int(int number);
	extern void skip(int number);
	extern int convert_little(Bytes byte_data);
	extern json get_time_data_skip(bool dst_flag);
	extern Str get_time_data(bool dst_flag);
	extern vector<int> get_length_data(int length_bytes = 4, int separator = 4, IntNone length = {});
	extern vector<float> get_length_doubles(int length_bytes = 4, IntNone length = {});
	extern vector<vector<int>> get_equip_slots();
	extern json get_main_story_levels();
	extern vector<vector<int>> get_treasures();
	extern json get_cat_upgrades();
	extern json get_blue_upgrades();
	extern Str get_utf8_string(IntNone length = {});
	extern int read_variable_length_int();
	extern tuple<json, json> load_bonus_hash();
	extern json get_event_stages_current();
	extern json get_event_stages(json lengths);
	extern vector<json> get_purchase_receipts();
	extern json get_dojo_data_maybe();
	extern vector<json> get_data_before_outbreaks();
	extern json get_outbreaks();
	extern vector<json> get_mission_data_maybe();
	extern tuple<vector< tuple<int, int> >, json> get_unlock_popups();
	extern void get_unknown_data();
	extern json get_cat_cannon_data();
	extern vector<json> get_data_near_ht();
	extern json get_ht_it_data();
	extern json get_mission_segment();
	extern json get_mission_data();
	extern vector<json> get_data_after_challenge();
	extern vector<json> get_data_after_tower();
	extern json get_event_timed_scores();
	extern json get_uncanny_progress(json lengths);
	extern json get_data_after_uncanny();
	extern json get_gold_pass_data();
	extern json get_talent_data();
	extern json get_medals();
	extern vector<json> get_data_after_medals();
	extern vector<json> get_data_after_after_leadership(bool dst);
	extern json get_legend_quest_current();
	extern void get_legend_quest_progress(json lengths);
	extern vector<json> get_data_after_leadership();
	extern json get_gauntlet_current();
	extern json get_gauntlet_progress(json lengths, bool unlock = true);
	extern json get_enigma_stages();
	extern tuple<json, vector<json>> get_cleared_slots();
	extern vector<json> get_data_after_gauntlets();
	extern vector<json> get_data_after_orbs();
	extern json get_cat_shrine_data();
	extern json get_cat_shrine_data();
	extern json get_talent_orbs(json game_version);
	extern vector<json> data_after_after_gauntlets();
	extern vector<json> get_data_near_end_after_shards();
	extern vector<json> get_data_near_end();
	extern json get_aku();
	extern vector<json> get_data_after_aku();
	extern vector<json> get_data_near_end_after_aku();
	extern json exit_parser(json save_stats);
	extern json check_gv(json save_stats, int game_version);
	extern json get_play_time();
	extern json start_parse(Bytes save_data , Str country_code);
	extern int get_game_version(Bytes save_data);
	extern bool get_dst(Bytes save_data , int offset);
	extern float get_double();
	extern vector<json> get_110800_data();
	extern vector<json> get_110800_data_2();
	extern vector<json> get_110700_data();
	extern json get_login_bonuses();
	extern vector<vector<int>> get_tower_item_obtained();
	template<typename type> extern json get_dict(type key_type , type value_type , optional<int> length = None);
	extern json parse_save(Bytes save_data , StrNone country_code , optional<bool> dst = {});
}