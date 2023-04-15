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

	union VecVec {
		vector<vector<Any>> vec2d;
		vector<Any> vec;
	};

	extern vector<int> create_vector_separated(vector<int> data, int length);
	extern vector<int> create_vector_double(vector<float> data);
	extern vector<int> serialise_treasures(vector<int> save_data , vector<vector<int>> treasures);
	extern vector<Any> flatten_list(VecVec _2d_vector);
	extern vector<int> serialse_purchase_receipts(vector<int> save_data , json data);
	extern vector<int> serialise_outbreaks(vector<int> save_data , json outbreaks);
	extern vector<int> serialise_talent_data(vector<int> save_data , json talents);
	extern vector<int> serialise_aku(vector<int> save_data , json aku);
	extern vector<int> serialise_tower(vector<int> save_data , json tower);
	extern vector<int> exit_serialiser(vector<int> save_data , json save_stats);
	extern vector<int> serialise_medals(vector<int> save_data , json medals);
	extern vector<int> serialise_play_time(vector<int> save_data , json play_time);
	extern vector<int> serialise_mission_segment(vector<int> save_data , json data);
	extern vector<int> serialise_dojo(vector<int> save_data , json dojo_data);
	extern vector<int> write_double(vector<int> save_data , float number);
	extern Bytes start_serialize(json save_stats);
	extern vector<int> serialise_gold_pass(vector<int> save_data , json gold_pass);
	extern vector<int> write_variable_length_int(vector<int> save_data , int i);
	extern vector<int> write_dict(vector<int> save_data , json data);
	extern Bytes serialize_save(json save_stats);

}