//#include "../headers/Parser.h"
//
//int BCats::Parser::address;
//Bytes BCats::Parser::globalSaveDataBytes;
//
//nlohmann::json BCats::Parser::next_int_len(int number) {
//    if (number < 0) {
//        //raise Exception("Invalid number")
//    }
//    if (globalSaveDataBytes.size() <= 1) {
//        //raise Exception("Invalid save data")
//    }
//    if (number > sizeof(globalSaveDataBytes)) {
//        //raise Exception("Byte length is greater than the length of the save data")
//    }
//    int val = convert_little(save_data_g[address:address + number]);
//    nlohmann::json data;
//    address += number;
//    data["Value"] = val;
//    data["Length"] = number;
//    return data;
//}
//int BCats::Parser::next_int(int number) {
//    return next_int_length(number)["Value"];
//}
//
//std::string BCats::Parser::get_md5_sum(std::string data) {
//    const char* rawData = data.c_str();
//    MD5 md5;
//    md5.add(rawData, sizeof(rawData));
//    return md5.getHash();
//}
//
//std::string BCats::Parser::get_save_data_sum(const Bytes& saveDataBytes, std::string code) {
//    if ( code == "jp" || code == "ja")
//        code = "";
//
//    //std::string salt = ("battlecats" + countryCodeMap[countryCode]).encode("utf-8");
//    std::string salt = ("battlecats" + code);
//    const int saltSize = salt.size();
//    for (int i = 0; i < saltSize; i++)
//        salt[i] = (uint8_t)salt[i];
//    std::string slicedSavedData(saveDataBytes.begin(), saveDataBytes.end() - 32);
//    std::string dataToHash = salt + slicedSavedData;
//
//    return get_md5_sum(dataToHash);
//}
//
//int BCats::Parser::find_date() {
//    for (int i = 0; i < 100; i++) {
//        int val = next_int(4);
//        if (val >= 2000 and val <= 3000)
//            return address - 4;
//        //raise Exception("Could not find date")
//    }
//}
//
//void BCats::Parser::parse_save(nlohmann::json& saveStats, Bytes saveDataBytes, Data::CountryCode countryCode, bool dst = NULL) {
//    address = 0;
//    globalSaveDataBytes = saveDataBytes;
//    
//    nlohmann::json saveStats;
//    saveStats["editor_version"] = EDITOR_VERSION;
//
//    saveStats["game_version"] = next_int_len(4);
//    saveStats["version"] = countryCode;
//
//    saveStats["unknown_1"] = next_int_len(1);
//
//    saveStats["mute_music"] = next_int_len(1);
//    saveStats["mute_sound_effects"] = next_int_len(1);
//
//    saveStats["cat_food"] = next_int_len(4);
//    saveStats["current_energy"] = next_int_len(4);
//
//    int old_address = address;
//    int new_address = find_date();
//    address = old_address;
//    int extra = new_address - old_address;
//    saveStats["extra_time_data"] = next_int_len(extra);
//
//    if (dst == NULL) {
//        dst = get_dst(saveDataBytes, address + 118);
//    }
//    saveStats["dst"] = dst
//        if (
//            saveStats["version"] == "jp"
//            && dst
//            || saveStats["version"] != "jp"
//            && !dst
//            ) {
//            //helper.colored_text(
//            //    "Warning: DST detected is not correct for this save version, this may cause issues with save parsing.",
//            //    helper.RED,
//            //    )
//    }
//
//    nlohmann::json data = get_time_data_skip(saveStats["dst"]);
//
//    saveStats["time"] = data["time"];
//    saveStats["dst_val"] = data["dst"];
//    saveStats["time_stamp"] = data["time_stamp"];
//    saveStats["duplicate_time"] = data["duplicate"];
//
//    saveStats["unknown_flags_1"] = get_length_data(length = 3);
//    saveStats["upgrade_state"] = next_int_len(4);
//    saveStats["xp"] = next_int_len(4);
//
//    saveStats["tutorial_cleared"] = next_int_len(4);
//    saveStats["unknown_flags_2"] = get_length_data(length = 12);
//    saveStats["unknown_flag_1"] = next_int_len(1);
//    saveStats["slots"] = get_equip_slots();
//
//    saveStats["cat_stamp_current"] = next_int_len(4);
//
//    saveStats["cat_stamp_collected"] = get_length_data(length = 30);
//    saveStats["unknown_2"] = next_int_len(4);
//    saveStats["daily_reward_flag"] = next_int_len(4);
//    saveStats["unknown_116"] = get_length_data(length = 10);
//
//    saveStats["story_chapters"] = get_main_story_levels();
//    saveStats["treasures"] = get_treasures();
//    
//    try {
//        saveStats["enemy_guide"] = get_length_data();
//    }
//    catch (const std::exception&) {
//        parse_save(saveStats, saveDataBytes, countryCode, !dst);
//    }
//    if (len(saveStats["enemy_guide"]) == 0)
//        return parse_save(saveStats, saveDataBytes, countryCode, !dst);
//    saveStats["cats"] = get_length_data();
//    saveStats["cat_upgrades"] = get_cat_upgrades();
//    saveStats["current_forms"] = get_length_data();
//
//    saveStats["blue_upgrades"] = get_blue_upgrades();
//
//    saveStats["menu_unlocks"] = get_length_data();
//    saveStats["new_dialogs_1"] = get_length_data();
//
//    saveStats["battle_items"] = get_length_data(4, 4, 6);
//    saveStats["new_dialogs_2"] = get_length_data();
//    saveStats["unknown_6"] = next_int_len(4);
//    saveStats["unknown_7"] = get_length_data(length = 21);
//
//    saveStats["lock_item"] = next_int_len(1);
//    saveStats["locked_items"] = get_length_data(1, 1, 6);
//    saveStats["second_time"] = get_time_data(saveStats["dst"]);
//
//    saveStats["unknown_8"] = get_length_data(length = 50);
//    saveStats["third_time"] = get_time_data(saveStats["dst"]);
//
//    saveStats["unknown_9"] = next_int_len(6 * 4);
//
//    saveStats["thirty2_code"] = get_utf8_string();
//    saveStats["unknown_10"] = load_bonus_hash();
//    saveStats["unknown_11"] = get_length_data(length = 4);
//    saveStats["normal_tickets"] = next_int_len(4);
//    saveStats["rare_tickets"] = next_int_len(4);
//    saveStats["gatya_seen_cats"] = get_length_data();
//    saveStats["unknown_12"] = get_length_data(length = 10);
//    length = next_int(2);
//    bool catStorageLen = true;
//    if (length != 128) {
//        skip(-2);
//        catStorageLen = false;
//        length = 100;
//    }
//
//    int catStorageId = get_length_data(2, 4, length);
//    int catStorageType = get_length_data(2, 4, length);
//    saveStats["cat_storage"] = {
//        {"ids", catStorageId},
//        {"types", catStorageType},
//        {"len", catStorageLen}
//    };
//    int currentSel = get_event_stages_current();
//    saveStats["event_current"] = current_sel;
//
//    saveStats["event_stages"] = get_event_stages(current_sel);
//
//    saveStats["unknown_15"] = get_length_data(length = 38);
//    saveStats["unit_drops"] = get_length_data();
//    saveStats["rare_gacha_seed"] = next_int_len(4);
//    saveStats["unknown_17"] = next_int_len(12);
//    saveStats["unknown_18"] = next_int_len(4);
//
//    saveStats["fourth_time"] = get_time_data(saveStats["dst"]);
//    saveStats["unknown_105"] = get_length_data(length = 5);
//    saveStats["unknown_107"] = get_length_data(separator = 1, length = 3);
//
//    if (saveStats["dst"])
//        saveStats["unknown_110"] = get_utf8_string();
//
//    int totalStrs = next_int(4);
//    std::vector<std::string> unknown108 = {};
//    for (int i = 0; i < totalStrs; i++)
//        unknown108.push_back(get_utf8_string());
//    saveStats["unknown_108"] = unknown108;
//
//    if (saveStats["dst"]) {
//        saveStats["time_stamps"] = get_length_doubles(length = 3);
//
//        int length = next_int(4);
//        std::vector<std::string> strs = {};
//        for (int i = 0; i < length; i++) {        
//            strs.push_back(get_utf8_string());
//            saveStats["unknown_112"] = strs;
//            saveStats["energy_notice"] = next_int_len(1);
//            saveStats["game_version_2"] = next_int_len(4);
//        }
//    }
//
//    saveStats["unknown_111"] = next_int_len(4);
//    saveStats["unlocked_slots"] = next_int_len(1);
//
//    int length1 = next_int(4);
//    int length2 = next_int(4);
//    nlohmann::json unknown20 = {};
//    unknown20 = { "Value", get_length_data(4, 4, length_1 * length_2) };
//    unknown20["Length_1"] = length1;
//    unknown20["Length_2"] = length2;
//    saveStats["unknown_20"] = unknown20;
//
//    saveStats["time_stamps_2"] = get_length_doubles(length = 4);
//
//    saveStats["trade_progress"] = next_int_len(4);
//
//    if (saveStats["dst"])
//        saveStats["time_stamps_2"].push_back(get_double());
//    else
//        saveStats["unknown_24"] = next_int_len(4);
//
//    saveStats["catseye_related_data"] = get_cat_upgrades();
//    saveStats["unknown_22"] = get_length_data(length = 11);
//    saveStats["user_rank_rewards"] = get_length_data(4, 1);
//
//    if (!saveStats["dst"])
//        saveStats["time_stamps_2"].push_back(get_double());
//
//    saveStats["unlocked_forms"] = get_length_data();
//    saveStats["transfer_code"] = get_utf8_string();
//    saveStats["confirmation_code"] = get_utf8_string();
//    saveStats["transfer_flag"] = next_int_len(1);
//
//    std::vector<int> lengths = { next_int(4), next_int(4), next_int(4) };
//    int length = lengths[0] * lengths[1] * lengths[2];
//
//    saveStats["stage_data_related_1"] = {
//        {"Value", get_length_data(4, 1, length)},
//        {"Lengths", lengths},
//    };
//
//    saveStats["event_timed_scores"] = get_event_timed_scores();
//    saveStats["inquiry_code"] = get_utf8_string();
//    saveStats["play_time"] = get_play_time();
//
//    saveStats["unknown_25"] = next_int_len(1);
//
//    saveStats["backup_state"] = next_int_len(4);
//
//    if (saveStats["dst"])
//        saveStats["unknown_119"] = next_int_len(1);
//
//    saveStats["gv_44"] = next_int_len(4);
//
//    saveStats["unknown_120"] = next_int_len(4);
//
//    saveStats["itf_timed_scores"] = Helper::chunks(get_length_data(4, 4, 51 * 3), 51);
//
//    saveStats["unknown_27"] = next_int_len(4);
//    saveStats["cat_related_data_1"] = get_length_data();
//    saveStats["unknown_28"] = next_int_len(1);
//
//    saveStats["gv_45"] = next_int_len(4);
//    saveStats["gv_46"] = next_int_len(4);
//
//    saveStats["unknown_29"] = next_int_len(4);
//    saveStats["lucky_tickets_1"] = get_length_data();
//    saveStats["unknown_32"] = get_length_data();
//
//    saveStats["gv_47"] = next_int_len(4);
//    saveStats["gv_48"] = next_int_len(4);
//
//    if (!saveStats["dst"])
//        saveStats["energy_notice"] = next_int_len(1);
//    saveStats["account_created_time_stamp"] = get_double();
//
//    saveStats["unknown_35"] = get_length_data();
//    saveStats["unknown_36"] = next_int_len(15);
//
//    saveStats["user_rank_popups"] = next_int_len(3);
//
//    saveStats["unknown_37"] = next_int_len(1);
//
//    saveStats["gv_49"] = next_int_len(4);
//    saveStats["gv_50"] = next_int_len(4);
//    saveStats["gv_51"] = next_int_len(4);
//    saveStats["cat_guide_collected"] = get_length_data(4, 1);
//
//    saveStats["gv_52"] = next_int_len(4);
//
//    saveStats["time_stamps_3"] = get_length_doubles(length = 5);
//
//    saveStats["cat_fruit"] = get_length_data();
//    saveStats["cat_related_data_3"] = get_length_data();
//    saveStats["catseye_cat_data"] = get_length_data();
//    saveStats["catseyes"] = get_length_data();
//    saveStats["catamins"] = get_length_data();
//
//    saveStats["gamatoto_time_left"] = Helper::seconds_to_time(int(get_double()));
//    saveStats["gamatoto_exclamation"] = next_int_len(1);
//    saveStats["gamatoto_xp"] = next_int_len(4);
//    saveStats["gamamtoto_destination"] = next_int_len(4);
//    saveStats["gamatoto_recon_length"] = next_int_len(4);
//
//    saveStats["unknown_43"] = next_int_len(4);
//
//    saveStats["gamatoto_complete_notification"] = next_int_len(4);
//
//    saveStats["unknown_44"] = get_length_data(4, 1);
//    saveStats["unknown_45"] = get_length_data(4, 12 * 4);
//    saveStats["gv_53"] = next_int_len(4);
//
//    saveStats["helpers"] = get_length_data();
//
//    saveStats["unknown_47"] = next_int_len(1);
//
//    saveStats["gv_54"] = next_int_len(4);
//
//    saveStats["purchases"] = get_purchase_receipts();
//    saveStats["gv_54"] = next_int_len(4);
//    saveStats["gamatoto_skin"] = next_int_len(4);
//    saveStats["platinum_tickets"] = next_int_len(4);
//
//    saveStats["login_bonuses"] = get_login_bonuses();
//    saveStats["unknown_49"] = next_int_len(16);
//    saveStats["announcment"] = get_length_data(length = 32);
//
//    saveStats["backup_counter"] = next_int_len(4);
//
//    saveStats["unknown_131"] = get_length_data(length = 3);
//    saveStats["gv_55"] = next_int_len(4);
//
//    saveStats["unknown_51"] = next_int_len(1);
//
//    saveStats["unknown_113"] = get_data_before_outbreaks();
//
//    saveStats["dojo_data"] = get_dojo_data_maybe();
//    saveStats["dojo_item_lock"] = next_int_len(1);
//    saveStats["dojo_locks"] = get_length_data(1, 1, 2);
//
//    saveStats["unknown_114"] = next_int_len(4);
//    saveStats["gv_58"] = next_int_len(4); // 0x3a
//    saveStats["unknown_115"] = next_int_len(8);
//
//    saveStats["outbreaks"] = get_outbreaks();
//
//    saveStats["unknown_52"] = get_double();
//    saveStats["item_schemes"] = {};
//    saveStats["item_schemes"]["to_obtain_ids"] = get_length_data();
//    saveStats["item_schemes"]["received_ids"] = get_length_data();
//
//    saveStats["current_outbreaks"] = get_outbreaks();
//
//    saveStats["unknown_55"] = get_mission_data_maybe();
//
//    saveStats["time_stamp_4"] = get_double();
//    saveStats["gv_60"] = next_int_len(4);
//
//    saveStats["unknown_117"] = get_unknown_data();
//
//    saveStats["gv_61"] = next_int_len(4);
//    data = get_unlock_popups();
//    saveStats["unlock_popups"] = data[0];
//
//    saveStats["unknown_118"] = data[1];
//
//    saveStats["base_materials"] = get_length_data();
//
//    saveStats["unknown_56"] = next_int_len(8);
//    saveStats["unknown_57"] = next_int_len(1);
//    saveStats["unknown_58"] = next_int_len(4);
//
//    saveStats["engineers"] = next_int_len(4);
//    saveStats["ototo_cannon"] = get_cat_cannon_data();
//
//    saveStats["unknown_59"] = get_data_near_ht();
//
//    saveStats["tower"] = get_ht_it_data();
//    saveStats["missions"] = get_mission_data();
//    saveStats["tower_item_obtained"] = get_tower_item_obtained();
//    saveStats["unknown_61"] = get_data_after_tower();
//
//    saveStats["challenge"] = { {"Score", next_int_len(4)}, {"Cleared", next_int_len(1)} };
//
//    saveStats["gv_67"] = next_int_len(4);  // 0x43
//
//    saveStats["weekly_event_missions"] = get_dict(int, bool);
//    saveStats["won_dojo_reward"] = next_int_len(1);
//    saveStats["event_flag_update_flag"] = next_int_len(1);
//
//    saveStats["gv_68"] = next_int_len(4);  // 0x44
//
//    saveStats["completed_one_level_in_chapter"] = get_dict(int, int);
//    saveStats["displayed_cleared_limit_text"] = get_dict(int, bool);
//    saveStats["event_start_dates"] = get_dict(int, int);
//    saveStats["stages_beaten_twice"] = get_length_data();
//
//    saveStats["unknown_102"] = get_data_after_challenge();
//
//    lengths = get_uncanny_current();
//    saveStats["uncanny_current"] = lengths;
//    saveStats["uncanny"] = get_uncanny_progress(lengths);
//
//    int total = lengths["total"];
//    saveStats["unknown_62"] = next_int_len(4);
//    saveStats["unknown_63"] = get_length_data(length = total);
//
//    saveStats["unknown_64"] = get_data_after_uncanny();
//
//    total = saveStats["unknown_64"]["progress"]["Lengths"]["total"];
//    saveStats["unknown_65"] = next_int_len(4);
//    val61 = saveStats["unknown_65"];
//
//    saveStats["unknown_66"] = {};
//    //std::vector<any>
//    nlohmann::json unknown66 = {};
//    for (int i = 0; i < total; i++) {
//        val61 = next_int_len(4);
//        unknown66.push_back(val61);
//    }
//    saveStats["unknown_66"] = unknown66;
//
//    int val54 = 0x37;
//    if (val61["Value"] < 0x38)
//        val54 = val61["Value"];
//
//    saveStats["lucky_tickets_2"] = get_length_data(length = val54);
//
//    saveStats["unknown_67"] = {};
//    if (0x37 < val61["Value"])
//        saveStats["unknown_67"] = get_length_data(4, 4, val61["Value"]);
//
//    saveStats["unknown_68"] = next_int_len(1);
//
//    saveStats["gv_77"] = next_int_len(4);  // 0x4d
//
//    saveStats["gold_pass"] = get_gold_pass_data();
//
//    saveStats["talents"] = get_talent_data();
//    saveStats["np"] = next_int_len(4);
//
//    saveStats["unknown_70"] = next_int_len(1);
//
//    saveStats["gv_80000"] = next_int_len(4);  // 80000
//
//    saveStats["unknown_71"] = next_int_len(1);
//
//    saveStats["leadership"] = next_int_len(2);
//    saveStats["officer_pass_cat_id"] = next_int_len(2);
//    saveStats["officer_pass_cat_form"] = next_int_len(2);
//
//    saveStats["gv_80200"] = next_int_len(4); // 80200
//    saveStats["filibuster_stage_id"] = next_int_len(1);
//    saveStats["filibuster_stage_enabled"] = next_int_len(1);
//
//    saveStats["gv_80300"] = next_int_len(4);  // 80300
//
//    saveStats["unknown_74"] = get_length_data();
//
//    saveStats["gv_80500"] = next_int_len(4);  // 80500
//
//    saveStats["unknown_75"] = get_length_data(2, 4);
//
//    lengths = get_legend_quest_current();
//    saveStats["legend_quest_current"] = lengths;
//    saveStats["legend_quest"] = get_legend_quest_progress(lengths);
//
//    saveStats["unknown_133"] = get_length_data(4, 1, lengths["total"]);
//    saveStats["legend_quest_ids"] = get_length_data(4, 4, lengths["stages"]);
//
//    saveStats["unknown_76"] = get_data_after_leadership();
//    saveStats["gv_80700"] = next_int_len(4);  // 80700
//    if (saveStats["dst"]) {
//        saveStats["unknown_104"] = next_int_len(1);
//        saveStats["gv_100600"] = next_int_len(4);
//        if (saveStats["gv_100600"]["Value"] != 100600)
//            skip(-5);
//    }
//    saveStats["restart_pack"] = next_int_len(1);
//
//    saveStats["unknown_101"] = get_data_after_after_leadership(saveStats["dst"]);
//
//    saveStats["medals"] = get_medals();
//
//    saveStats["unknown_103"] = get_data_after_medals();
//
//    lengths = get_gauntlet_current();
//    saveStats["gauntlet_current"] = lengths;
//    saveStats["gauntlets"] = get_gauntlet_progress(lengths);
//
//    saveStats["unknown_77"] = get_length_data(4, 1, lengths["total"]);
//
//    saveStats["gv_90300"] = next_int_len(4);  // 90300
//
//    lengths = get_gauntlet_current();
//    saveStats["unknown_78"] = lengths;
//    saveStats["unknown_79"] = get_gauntlet_progress(lengths);
//
//    saveStats["unknown_80"] = get_length_data(4, 1, lengths["total"]);
//
//    saveStats["enigma_data"] = get_enigma_stages();
//    data = get_cleared_slots();
//    saveStats["cleared_slot_data"] = data[0];
//
//    saveStats["unknown_121"] = data[1];
//
//    lengths = get_gauntlet_current();
//    saveStats["collab_gauntlets_current"] = lengths;
//    saveStats["collab_gauntlets"] = get_gauntlet_progress(lengths);
//    saveStats["unknown_84"] = get_length_data(4, 1, lengths["total"]);
//
//    saveStats["unknown_85"] = data_after_after_gauntlets();
//
//    saveStats["talent_orbs"] = get_talent_orbs(saveStats["game_version"]);
//
//    saveStats["unknown_86"] = get_data_after_orbs();
//
//    saveStats["cat_shrine"] = get_cat_shrine_data();
//
//    saveStats["unknown_130"] = next_int_len(4 * 5);
//
//    saveStats["gv_90900"] = next_int_len(4);  // 90900
//
//    saveStats["slot_names"] = get_slot_names(saveStats);
//    saveStats["gv_91000"] = next_int_len(4);
//    saveStats["legend_tickets"] = next_int_len(4);
//
//    saveStats["unknown_87"] = get_length_data(1, 5);
//    saveStats["unknown_88"] = next_int_len(2);
//
//    saveStats["token"] = get_utf8_string();
//
//    saveStats["unknown_89"] = next_int_len(1 * 3);
//    saveStats["unknown_90"] = next_int_len(8);
//    saveStats["unknown_91"] = next_int_len(8);
//
//    saveStats["gv_100000"] = next_int_len(4);  // 100000
//    saveStats = check_gv(saveStats, 100100);
//    if (saveStats["exit"])
//        return;
//
//    saveStats["date_int"] = next_int_len(4);
//
//    saveStats["gv_100100"] = next_int_len(4);  // 100100
//    saveStats = check_gv(saveStats, 100300);
//    if (saveStats["exit"])
//        return;
//
//    saveStats["unknown_93"] = get_length_data(4, 19, 6);
//
//    saveStats["gv_100300"] = next_int_len(4); // 100300
//    saveStats = check_gv(saveStats, 100700);
//    if (saveStats["exit"])
//        return;
//
//    saveStats["unknown_94"] = get_data_near_end();
//
//    saveStats["platinum_shards"] = next_int_len(4);
//
//    saveStats["unknown_100"] = get_data_near_end_after_shards();
//
//    saveStats["gv_100700"] = next_int_len(4);  // 100700
//    saveStats = check_gv(saveStats, 100900);
//    if (saveStats["exit"])
//        return;
//
//    saveStats["aku"] = get_aku();
//
//    saveStats["unknown_95"] = next_int_len(1 * 2);
//    saveStats["unknown_96"] = get_data_after_aku();
//
//    saveStats["gv_100900"] = next_int_len(4);  // 100900
//    saveStats = check_gv(saveStats, 101000);
//    if (saveStats["exit"])
//        return;
//
//    saveStats["unknown_97"] = next_int_len(1);
//
//    saveStats["gv_101000"] = next_int_len(4); // 101000
//    saveStats = check_gv(saveStats, 110000);
//    if (saveStats["exit"])
//        return;
//
//    saveStats["unknown_98"] = get_data_near_end_after_aku();
//
//    saveStats["gv_110000"] = next_int_len(4);  // 110000
//    saveStats = check_gv(saveStats, 110500);
//    if (saveStats["exit"])
//        return;
//
//    data = get_gauntlet_current();
//    saveStats["behemoth_culling_current"] = data;
//    saveStats["behemoth_culling"] = get_gauntlet_progress(data);
//    saveStats["unknown_124"] = get_length_data(4, 1, data["total"]);
//
//    saveStats["unknown_125"] = next_int_len(1);
//
//    saveStats["gv_110500"] = next_int_len(4);  // 110500
//    saveStats = check_gv(saveStats, 110600);
//    if (saveStats["exit"])
//        return;
//
//    saveStats["unknown_126"] = next_int_len(1);
//
//    saveStats["gv_110600"] = next_int_len(4);  // 110600
//    saveStats = check_gv(saveStats, 110700);
//    if (saveStats["exit"])
//        return;
//
//    saveStats["unknown_127"] = get_110700_data();
//
//    if (saveStats["dst"])
//        saveStats["unknown_128"] = next_int_len(1);
//
//    saveStats["gv_110700"] = next_int_len(4);  // 110700
//    saveStats = check_gv(saveStats, 110800);
//    if (saveStats["exit"])
//        return;
//
//    saveStats["shrine_dialogs"] = next_int_len(4);
//
//    saveStats["unknown_129"] = get_110800_data();
//
//    saveStats["dojo_3x_speed"] = next_int_len(1);
//
//    saveStats["unknown_132"] = get_110800_data_2();
//
//    saveStats["gv_110800"] = next_int_len(4); // 110800
//    saveStats = check_gv(saveStats, 110800);
//    if (saveStats["exit"])
//        return;
//
//    length = saveDataBytes.size() - address - 32;
//    saveStats["extra_data"] = next_int_len(length);
//
//    saveStats = exit_parser(saveStats);
//
//    return;
//}
//
//void BCats::Parser::start_parse(nlohmann::json& returnJson, Bytes saveDataBytes, Data::CountryCode countryCode) {
//    try
//    {
//         parse_save(returnJson, saveDataBytes, countryCode);
//    }
//    catch (const std::exception&)
//    {
//        ///Throw Exception here
//    }
//}