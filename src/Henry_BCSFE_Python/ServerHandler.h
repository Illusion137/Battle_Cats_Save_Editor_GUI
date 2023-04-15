#pragma once

#include <nlohmann/json.hpp>

#include <iostream>
#include <optional>
#include <tuple>

using json = nlohmann::json;
namespace BCSFE {
	using namespace std;

	typedef optional<json> JsonNone;

	extern int get_current_time();
	extern Str random_hex_string(int length);
	extern Str random_digit_string(int length);
	extern Str get_nyanko_auth_url();
	extern Str generate_nyanko_signature(Str inquiry_code , Str data);
	extern Str generate_nyanko_signature_v1(Str inquiry_code , Str data);
	extern bool check_nyanko_signature(Str signature, Str data, Str inquiry_code);
	extern bool check_nyanko_signature_v1(Str signature, Str data, Str inquiry_code);
	extern json get_headers(Str inquiry_code , Str data);
	extern JsonNone get_password(Str inquiry_code);
	extern json get_client_info(Str country_code , Str game_version);
	extern Str get_nyanko_save_url();
	extern optional<json> get_save_key_data(Str token);
	extern optional<Str> get_save_key(Str token);
	extern Str get_nyanko_backups_url();
	extern Str get_nyanko_managed_items_url();
	extern Str get_inquiry_code();
	extern bool test_is_save_data(Bytes save_data);
	extern optional<Str> download_handler();
}