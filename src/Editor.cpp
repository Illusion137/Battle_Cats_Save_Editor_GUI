#include "Editor.h"

static bool hasEntered = false;

#define X_SIZE 160
#define Y_SIZE 130

void BCats::Editor::on_gui_render() {
	static bool initializedEditor = false;
	if (!initializedEditor) {
		std::ifstream catDataJsonFile("res/Data/catDataFinal.json", std::ios::in | std::ios::binary);

		CatInfo::catDataJson = nlohmann::json::parse(catDataJsonFile);

		catDataJsonFile.close();

#ifdef TEST_EDITOR
		FileManager::saveFilePath = FileManager::get_save_folder_path() + TEST_SAVE_FILE_PATH;
		std::ifstream testSaveFile(TEST_SAVE_FILE_PATH, std::ios::in | std::ios::binary);

		try {
			FileManager::saveData = nlohmann::json::parse(testSaveFile);
		}
		catch (const std::exception& e) {
			std::cout << e.what();
			testSaveFile.close();
			exit(-1);
		}

		testSaveFile.close();
#else
		/// Once left Home Screen unload all resources
		Textures::unload_texture("res/Images/Home/title.png");
		Textures::unload_texture("res/Images/Home/background.jpg");
		Textures::unload_texture("res/Images/Home/fieryhenryPfp.png");
		Textures::unload_texture("res/Images/Home/kirby.png");
#endif // TEST_EDITOR

		initializedEditor = true;
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Edit Cats"))
				currentEditor = Editor_Cats;
			if (ImGui::MenuItem("Edit Items"))
				currentEditor = Editor_Items;
			if (ImGui::MenuItem("Edit Gamatoto/Otto"))
				currentEditor = Editor_Gamatoto;
			if (ImGui::MenuItem("Edit Talents"))
				currentEditor = Editor_Talents;
			if (ImGui::MenuItem("Edit Levels"))
				currentEditor = Editor_Levels;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	switch (currentEditor) {
	case Editor_Cats:
		render_edit_cats();
		break;
	case Editor_Items:
		render_edit_items();
		break;
	case Editor_Gamatoto:
		render_edit_gamatoto();
		break;
	case Editor_Talents:
		render_edit_talents();
		break;
	case Editor_Levels:
		render_edit_levels();
		break;
	}

	ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x / 2) - 127, ImGui::GetWindowSize().y - 60));
	static Widgets::Animation::ButtonAnimation saveDataBtnAnim = { false, 0, true, 0 };
	if (Widgets::render_button("Save Data", saveDataBtnAnim)) {
		std::ofstream saveFile(FileManager::saveFilePath, std::ios::out | std::ios::trunc);
		saveFile << FileManager::saveData;
	}
}

void BCats::Editor::render_edit_cats()
{
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0x0, 0x0, 0x0, 0xff));
	static Textures::Texture undoTexture;
	Textures::load_texture(undoTexture, "res/Images/Editor/backArrow.png");

	const static int catCount = FileManager::saveData.at("cats").size();

	const CatInfo::Cat baseCat = { 1, 0, false, CatInfo::CatForm_Base, CatInfo::CatForm_Base, {"", "", ""}, { true, 0 }, {false, 0, true, 0} };
	static std::vector<CatInfo::Cat> catData(catCount, baseCat);

	static bool initializedData = false;
	if (!initializedData) {
		CatInfo::catIDs.reserve(catCount);
		for (int i = 0; i < catCount; i++) {
			if (FileManager::saveData.at("cats")[i] == 1) {
				CatInfo::catIDs.push_back(i);

				catData[i].unlockedValue = true;

				catData[i].level = FileManager::saveData.at("cat_upgrades").at("Base")[i] + 1;
				catData[i].plusLevel = FileManager::saveData.at("cat_upgrades").at("Plus")[i];

				catData[i].names[0] = CatInfo::get_cat_name_from_id(i, CatInfo::CatForm_Base);
				catData[i].names[1] = CatInfo::get_cat_name_from_id(i, CatInfo::CatForm_Evolved);
				catData[i].names[2] = CatInfo::get_cat_name_from_id(i, CatInfo::CatForm_True);

				int formsCurrent = FileManager::saveData.at("current_forms")[i];
				int formsUnlocked = FileManager::saveData.at("unlocked_forms")[i];
				catData[i].maxFormBegin = (CatInfo::CatForm_)(max(formsCurrent, formsUnlocked) + 49);
				catData[i].form = catData[i].maxFormBegin;
			}
		}
		initializedData = true;
	}

	static bool showFilters = false;
	render_filters_popup_modal(&showFilters);

	ImVec2 position = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(7, 18));
	static Widgets::Animation::ImageButtonAnimation undoAnim{ true, 0 };
	if (Widgets::render_image_button("Undo", (ImTextureID)(intptr_t)undoTexture.texture, undoAnim, ImVec2(undoTexture.width >> 1, undoTexture.height >> 1))) {
		//std::cout << 1;
	}
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0xff, 0xff, 0xff, 0xff));
	Widgets::render_tooltip("Undo Previous 'Batch Change'");
	ImGui::PopStyleColor(1);

	ImGui::SameLine();

	ImGui::SetCursorPos(ImVec2(60, 10));
	ImGui::BeginGroup();
	ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0xff, 0xff, 0xff, 0xaa));
	static char namesBuffer[512];
	if (ImGui::InputText("name(s)", namesBuffer, 512)) {
		std::vector<std::string> result;
		std::stringstream ss(namesBuffer);
		std::string item;

		while (getline(ss, item, ','))
			result.push_back(item);

		if (result.size() > 0) {
			CatInfo::Filters::get_instance().names = result;
		}
		else {
			CatInfo::Filters::get_instance().names = {};
		}
		CatInfo::update_cat_ids_from_filters();
	}
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0xff, 0xff, 0xff, 0xff));
	Widgets::render_tooltip("name(s) separated by commas");
	ImGui::PopStyleColor(1);

	static char idsBuffer[1000];
	if (ImGui::InputText("id(s)", idsBuffer, 1000)) {
		std::vector<std::string> result;
		std::stringstream ss(idsBuffer);
		std::string item;

		while (getline(ss, item, ','))
			result.push_back(item);

		const int resSize = result.size();
		std::vector<int> idsList(resSize, -1);
		for (int i = 0; i < resSize; i++) {
			try
			{
				std::string clearedResult = boost::regex_replace(result[i], boost::regex("([^0-9])"), "");
				if (clearedResult != "" && clearedResult.length() <= 3)
					idsList[i] = std::stoi(clearedResult);
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << '\n';
			}
		}
		if (idsList.size() > 0) {
			CatInfo::Filters::get_instance().ids = idsList;
		}
		else {
			CatInfo::Filters::get_instance().ids = {};
		}
		CatInfo::update_cat_ids_from_filters();
	}
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0xff, 0xff, 0xff, 0xff));
	Widgets::render_tooltip("id(s) separated by commas");
	ImGui::PopStyleColor(1);

	ImGui::PopStyleColor(1);
	ImGui::EndGroup();

	ImGui::SameLine();

	static Widgets::Animation::ButtonAnimation filterButtonAnim{ false, 0, true, 0 };
	position = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(position.x + 5, position.y + 10));
	if (Widgets::render_button("Filters", filterButtonAnim)) {
		showFilters = true;
	}

	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0xf9, 0xe0, 0x81, 0xff));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 1));
	if (ImGui::BeginChild("Cat Editor", ImVec2(0, ImGui::GetWindowSize().y - 150), true)) {
		ImGui::Text("Cat Editor ");
		int unitsUsedModded = 0;
		int xUnitsAvailable = ImGui::GetContentRegionAvail().x / (X_SIZE + 10);

		if (xUnitsAvailable != 0)
			for (const int catID : CatInfo::catIDs) {
				render_cats(catID, catData[catID]);
				if (unitsUsedModded != xUnitsAvailable - 1) {
					ImGui::SameLine(((unitsUsedModded + 1) * (X_SIZE + 10)));
					++unitsUsedModded;
				}
				else {
					unitsUsedModded = 0;
				}
			}

		ImGui::EndChild();
	}

	ImGui::PopStyleColor(2);

	ImGui::PopStyleColor(1);
}

void BCats::Editor::render_cats(const int unitID, CatInfo::Cat& catData) {
	static Textures::Texture blackTexture, catTexture, rotateFormTexture;

	std::string s_unitId = std::to_string(unitID + 1);
	int unitIdLength = s_unitId.length();
	s_unitId.insert(0, 3 - unitIdLength, '0');

	Textures::load_texture(rotateFormTexture, "res/Images/Editor/rotate.png");
	Textures::load_texture(blackTexture, "res/Images/Home/black.png");
	Textures::load_texture(catTexture, "res/Images/Cats/BCatsDBImages/u" + s_unitId + "-" + (char)catData.form + ".png");

	s_unitId = std::to_string(unitID);
	unitIdLength = s_unitId.length();
	s_unitId.insert(0, 3 - unitIdLength, '0');

	ImVec2 position = ImGui::GetCursorPos();

	ImGui::BeginGroup();

	ImGui::Image((ImTextureID)(intptr_t)blackTexture.texture, ImVec2(X_SIZE, Y_SIZE));
	
	ImGui::SetWindowFontScale(0.6);
	ImGui::SetCursorPos(ImVec2(position.x + (X_SIZE / 2) - (ImGui::CalcTextSize(catData.names[catData.form-49].c_str()).x / 2), position.y + 3));
	Widgets::render_text(catData.names[catData.form - 49]);
	ImGui::SetCursorPos(ImVec2(position.x+5, position.y + 20));
#define CAT_X_SIZE 150
#define CAT_Y_SIZE 60
	ImGui::Image((ImTextureID)(intptr_t)catTexture.texture, ImVec2(CAT_X_SIZE, CAT_Y_SIZE), ImVec2(0,0), ImVec2(1, 1),
		catData.unlockedValue ? ImVec4(1,1,1,1) : ImVec4(1, 1, 1, 0.6));
	ImGui::SetWindowFontScale(0.47);
	ImGui::SetCursorPos(ImVec2(position.x + 86, position.y + 66));
	Widgets::render_text("Level", IM_COL32(0xb8, 0x83, 0x39, 0xff));

	ImGui::SetWindowFontScale(0.9);
	ImGui::SetCursorPos(ImVec2(position.x + 123, position.y + 55));
	Widgets::render_drag_int_range(("Level " + s_unitId + (char)catData.form).c_str(), &catData.level,
		catData.form == CatInfo::CatForm_True ? 20 - catData.plusLevel : catData.form == CatInfo::CatForm_Evolved ? 10 - catData.plusLevel : 1, 99, IM_COL32(0xff, 0xc6, 0x4a, 0xff));

	ImGui::SetWindowFontScale(0.7);
	ImGui::SetCursorPos(ImVec2(position.x + 130, position.y + 77));
	Widgets::render_drag_int_range(("PlusLevel" + s_unitId + (char)catData.form).c_str(), &catData.plusLevel, 0, 99, IM_COL32(0xff, 0xc6, 0x4a, 0xff));
	
	ImGui::SetWindowFontScale(1);
	ImGui::SetCursorPos(ImVec2(position.x + 115, position.y + 71));
	Widgets::render_text("+", IM_COL32(0xff, 0xc6, 0x4a, 0xff));

	ImGui::SetWindowFontScale(0.5);
	ImGui::SetCursorPos(ImVec2(position.x + 8, position.y + 80));

	Widgets::render_text(s_unitId + '-' + (char)max(catData.maxFormBegin, catData.form), IM_COL32(0xff, 0xff, 0xff, 0xff));

	ImGui::SetCursorPos(ImVec2(position.x + 5, position.y + 100));

	ImGui::SetWindowFontScale(0.35);
	static bool val = false;
	Widgets::render_toggle_button_text((catData.unlockedValue ? "Lock " : "Unlock ") + s_unitId, catData.unlockedValue, catData.unlockedAnim, IM_COL32(0x5a, 0x99, 0x56, 255), ImVec2(70, 25));

	ImGui::SetWindowFontScale(1);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(0,0,0,0));
	ImGui::SetCursorPos(ImVec2(position.x + 130, position.y + 100));
	if (Widgets::render_image_button(("Rotate" + s_unitId + (char)catData.form).c_str(), (ImTextureID)(intptr_t)rotateFormTexture.texture, catData.rotateAnim, ImVec2(rotateFormTexture.width >> 1, rotateFormTexture.height >> 1),
		ImVec2(0,0), ImVec2(1,1), ImVec4(0,0,0,0))) {
		switch (catData.form) {
		case(CatInfo::CatForm_Base):
			catData.level = catData.level + catData.plusLevel >= 10 ? catData.level : 10 - catData.plusLevel;
			catData.form = CatInfo::CatForm_Evolved;
			break;
		case(CatInfo::CatForm_Evolved):
			if (CatInfo::is_valid_cat_form(unitID)) {
				catData.level = catData.level + catData.plusLevel >= 20 ? catData.level : 20 - catData.plusLevel;
				catData.form = CatInfo::CatForm_True;
			}
			else
				catData.form = CatInfo::CatForm_Base;
			break;
		case(CatInfo::CatForm_True):
			catData.form = CatInfo::CatForm_Base;
			break;
		}
	}
	ImGui::PopStyleColor(3);

	ImGui::EndGroup();
}

void BCats::Editor::render_filters_popup_modal(bool* open)
{
	if (*open) {
		ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(0x8a, 0x8a, 0x8a, 0xff));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5,0));
		ImGui::OpenPopup("Cat Filters ( Beta )");
		ImGui::SetNextWindowSize(ImVec2(1008, 672));
		if (ImGui::BeginPopupModal("Cat Filters ( Beta )", open, ImGuiWindowFlags_NoResize))
		{
			static std::deque<bool> settingsValues(4, false);
			static bool initialized = false;
			if (!initialized) {
				settingsValues[0] = true;
				initialized = true;
			}
			if (ImGui::BeginChild("FiltersSettings", ImVec2(0, ImGui::GetWindowSize().y * 0.1))) {
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 8, ImGui::GetContentRegionAvail().y * 0.4));
				ImGui::SetWindowFontScale(0.8);
				Widgets::render_outlined_text("Settings", IM_COL32(0xe1, 0xb4, 0x20, 0xff));
				ImVec2 cursorPos = ImGui::GetCursorScreenPos();
				auto xSize = ImGui::GetContentRegionAvail().x;
				ImGui::GetWindowDrawList()->AddLine(ImVec2(cursorPos.x + 100, cursorPos.y - 65), ImVec2(cursorPos.x + 100, cursorPos.y + 30), IM_COL32(0x93, 0x93, 0x93, 0xff), 3);
				
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY() - 50));
				Widgets::render_checkbox("Owned", &settingsValues[0]); ImGui::SameLine();
				Widgets::render_checkbox("Not Owned", &settingsValues[1]); ImGui::SameLine();
				Widgets::render_checkbox("And Search", &settingsValues[2]); ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY() + 5));

				Widgets::render_checkbox("Has True Form", &settingsValues[3]);

				ImGui::SetWindowFontScale(1);
				ImGui::EndChild();
			}

			{
				ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
				auto* draw_list = ImGui::GetWindowDrawList();
				auto xSize = ImGui::GetContentRegionAvail().x;
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y), ImVec2(cursor_pos.x + xSize, cursor_pos.y), IM_COL32(0x7c, 0x7c, 0x7c, 0xff), 1);
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y + 1), ImVec2(cursor_pos.x + xSize, cursor_pos.y + 1), IM_COL32(0x69, 0x69, 0x69, 0xff), 1);
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y + 2), ImVec2(cursor_pos.x + xSize, cursor_pos.y + 2), IM_COL32(0x71, 0x71, 0x71, 0xff), 1);
			}

			static std::deque<bool> rarityValues(6, false);
			if (ImGui::BeginChild("FiltersRarity", ImVec2(0, ImGui::GetWindowSize().y * 0.15))) {
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 10, ImGui::GetContentRegionAvail().y * 0.35));
				Widgets::render_outlined_text("Rarity", IM_COL32(0xe1, 0xb4, 0x20, 0xff));
				ImVec2 cursorPos = ImGui::GetCursorScreenPos();
				auto xSize = ImGui::GetContentRegionAvail().x;
				ImGui::GetWindowDrawList()->AddLine(ImVec2(cursorPos.x + 100, cursorPos.y - 65), ImVec2(cursorPos.x + 100, cursorPos.y + 30), IM_COL32(0x93, 0x93, 0x93, 0xff), 3);
				
				static std::vector<Widgets::Animation::ButtonAnimation> buttonAnims(6, { false, 0, true, 0 });
				ImGui::SetWindowFontScale(0.75);
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY() - 55));
				if (Widgets::render_toggle_button_text("Normal", rarityValues[0], buttonAnims[0], IM_COL32(0x35, 0xa8, 0x1e, 255), ImVec2(150, 35))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Special", rarityValues[1], buttonAnims[1], IM_COL32(0x35, 0xa8, 0x1e, 255), ImVec2(150, 35))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Rare", rarityValues[2], buttonAnims[2], IM_COL32(0x35, 0xa8, 0x1e, 255), ImVec2(150, 35))) {

				} ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY()));
				if (Widgets::render_toggle_button_text("Super Rare", rarityValues[3], buttonAnims[3], IM_COL32(0x35, 0xa8, 0x1e, 255), ImVec2(150, 35))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Uber Rare", rarityValues[4], buttonAnims[4], IM_COL32(0x35, 0xa8, 0x1e, 255), ImVec2(150, 35))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Legend Rare", rarityValues[5], buttonAnims[5], IM_COL32(0x35, 0xa8, 0x1e, 255), ImVec2(150, 35))) {

				} ImGui::SameLine();

				ImGui::SetWindowFontScale(1);
				ImGui::EndChild();

			}

			{
				ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
				auto* draw_list = ImGui::GetWindowDrawList();
				auto xSize = ImGui::GetContentRegionAvail().x;
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y), ImVec2(cursor_pos.x + xSize, cursor_pos.y), IM_COL32(0x7c, 0x7c, 0x7c, 0xff), 1);
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y + 1), ImVec2(cursor_pos.x + xSize, cursor_pos.y + 1), IM_COL32(0x69, 0x69, 0x69, 0xff), 1);
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y + 2), ImVec2(cursor_pos.x + xSize, cursor_pos.y + 2), IM_COL32(0x71, 0x71, 0x71, 0xff), 1);
			}

			static std::deque<bool> traitValues(10, false);
			if (ImGui::BeginChild("FiltersTraits", ImVec2(0, ImGui::GetWindowSize().y * 0.18))) {
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 10, ImGui::GetContentRegionAvail().y * 0.35));
				ImGui::SetWindowFontScale(0.9);
				Widgets::render_outlined_text("Target", IM_COL32(0xe1, 0xb4, 0x20, 0xff));
				ImVec2 cursorPos = ImGui::GetCursorScreenPos();
				auto xSize = ImGui::GetContentRegionAvail().x;
				ImGui::GetWindowDrawList()->AddLine(ImVec2(cursorPos.x + 100, cursorPos.y - 65), ImVec2(cursorPos.x + 100, cursorPos.y + 30), IM_COL32(0x93, 0x93, 0x93, 0xff), 3);

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY() - 64));
				ImGui::SetWindowFontScale(0.75);
				
				static std::vector<Widgets::Animation::ButtonAnimation> animations(10, { false, 0, true, 0 });

				if (Widgets::render_toggle_button_text("Red", traitValues[0], animations[0], IM_COL32(0xdf, 0x55, 0x26, 255), ImVec2(150, 30), IM_COL32(0xa0, 0x3c, 0x18, 255))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Floating", traitValues[1], animations[1], IM_COL32(0x83, 0xaf, 0x52, 255), ImVec2(150, 30), IM_COL32(0x5e, 0x7e, 0x39, 255))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Black", traitValues[2], animations[2], IM_COL32(0x36, 0x36, 0x36, 255), ImVec2(150, 30), IM_COL32(0x20, 0x20, 0x20, 255))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Metal", traitValues[3], animations[3], IM_COL32(0xb1, 0xb0, 0xb0, 255), ImVec2(150, 30), IM_COL32(0xa4, 0xa3, 0xa2, 255))) {

				} ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY()));
				if (Widgets::render_toggle_button_text("Angel", traitValues[4], animations[4], IM_COL32(0xc7, 0xbc, 0x50, 255), ImVec2(150, 30), IM_COL32(0x8d, 0x86, 0x36, 255))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Alien", traitValues[5], animations[5], IM_COL32(0x69, 0xd7, 0xc4, 255), ImVec2(150, 30), IM_COL32(0x49, 0x99, 0x8b, 255))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Zombies", traitValues[6], animations[6], IM_COL32(0xa5, 0x38, 0xe0, 255), ImVec2(150, 30), IM_COL32(0x86, 0x35, 0xb3, 255))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Aku", traitValues[7], animations[7], IM_COL32(0x5f, 0x8c, 0xb4, 255), ImVec2(150, 30), IM_COL32(0x3d, 0x51, 0x8d, 255))) {

				} ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY()));
				if (Widgets::render_toggle_button_text("Relic", traitValues[8], animations[8], IM_COL32(0x83, 0x90, 0x3f, 255), ImVec2(150, 30), IM_COL32(0x69, 0x70, 0x24, 255))) {

				} ImGui::SameLine();
				if (Widgets::render_toggle_button_text("Traitless", traitValues[9], animations[9], IM_COL32(0xb4, 0xbd, 0xcc, 255), ImVec2(150, 30), IM_COL32(0xa4, 0xaf, 0xe1, 255))) {

				}

				ImGui::SetWindowFontScale(1);
				ImGui::EndChild();
			}

			{
				ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
				auto* draw_list = ImGui::GetWindowDrawList();
				auto xSize = ImGui::GetContentRegionAvail().x;
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y), ImVec2(cursor_pos.x + xSize, cursor_pos.y), IM_COL32(0x7c, 0x7c, 0x7c, 0xff), 1);
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y + 1), ImVec2(cursor_pos.x + xSize, cursor_pos.y + 1), IM_COL32(0x69, 0x69, 0x69, 0xff), 1);
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y + 2), ImVec2(cursor_pos.x + xSize, cursor_pos.y + 2), IM_COL32(0x71, 0x71, 0x71, 0xff), 1);
			}

#define EFFECTS_COUNT 45
			static std::deque<bool> characteristicValues(EFFECTS_COUNT, false);
			if (ImGui::BeginChild("FiltersChar", ImVec2(0, ImGui::GetContentRegionAvail().y * 0.75))) {
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 10, ImGui::GetContentRegionAvail().y * 0.35));
				ImGui::SetWindowFontScale(0.9);
				Widgets::render_outlined_text("Effect/\nAbility", IM_COL32(0xe1, 0xb4, 0x20, 0xff));
				ImVec2 cursorPos = ImGui::GetCursorScreenPos();
				auto xSize = ImGui::GetContentRegionAvail().x;
				ImGui::GetWindowDrawList()->AddLine(ImVec2(cursorPos.x + 100, cursorPos.y - 65), ImVec2(cursorPos.x + 100, cursorPos.y + 30), IM_COL32(0x93, 0x93, 0x93, 0xff), 3);

				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY() - 130));
				ImGui::SetWindowFontScale(0.75);

				static std::vector<Widgets::Animation::ImageButtonAnimation> animations(EFFECTS_COUNT, { true, 0 });
				static std::vector<Textures::Texture> effectTextures(EFFECTS_COUNT);
				for (int i = 0; i < EFFECTS_COUNT; i++) {
					Textures::load_texture(effectTextures[i], "res/Images/EffectIcons/ (" + (std::to_string(i+1)) + ").png");
				}
				for (int i = 0; i < EFFECTS_COUNT; i++) {
					Widgets::render_toggle_button_icon(("effectIcons" + std::to_string(i + 1)).c_str(), (ImTextureID)(intptr_t)effectTextures[i].texture, 
						ImVec2(40, 40), characteristicValues[i], animations[i]);
					if(i % 15 == 14)
						ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY()));
					else
						ImGui::SameLine();
				}

				ImGui::SetWindowFontScale(1);
				ImGui::EndChild();
			}
			
			{
				ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
				auto* draw_list = ImGui::GetWindowDrawList();
				auto xSize = ImGui::GetContentRegionAvail().x;
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y), ImVec2(cursor_pos.x + xSize, cursor_pos.y), IM_COL32(0x7c, 0x7c, 0x7c, 0xff), 1);
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y + 1), ImVec2(cursor_pos.x + xSize, cursor_pos.y + 1), IM_COL32(0x69, 0x69, 0x69, 0xff), 1);
				draw_list->AddLine(ImVec2(cursor_pos.x, cursor_pos.y + 2), ImVec2(cursor_pos.x + xSize, cursor_pos.y + 2), IM_COL32(0x71, 0x71, 0x71, 0xff), 1);
			}

			static Widgets::Animation::ButtonAnimation okButtonAnimation = { false, 0, true, 0 };
			if (Widgets::render_button("OK", okButtonAnimation)) {
				CatInfo::Filters::get_instance().owned = settingsValues[0];
				CatInfo::Filters::get_instance().notOwned = settingsValues[1];
				CatInfo::Filters::get_instance().andSearch = settingsValues[2];
				CatInfo::Filters::get_instance().hasTrueForm = settingsValues[3];
				
				CatInfo::Filters::get_instance().rarities = rarityValues;
				CatInfo::Filters::get_instance().targets = traitValues;
				CatInfo::Filters::get_instance().effectAbilities = characteristicValues;

				CatInfo::update_cat_ids_from_filters();

				*open = false;
			}
			ImGui::SameLine();
			static Widgets::Animation::ButtonAnimation clearButtonAnimation = { false, 0, true, 0 };
			if (Widgets::render_button("Clear", clearButtonAnimation)) {
				std::fill(settingsValues.begin(), settingsValues.end(), false);
				settingsValues[0] = true;
				std::fill(rarityValues.begin(), rarityValues.end(), false);
				std::fill(traitValues.begin(), traitValues.end(), false);
				std::fill(characteristicValues.begin(), characteristicValues.end(), false);

			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(1);
	}
}

void BCats::Editor::render_edit_items()
{
	static bool initializedData = false;

	const int cateyeCount = 6;
	static std::vector<int> cateyeValues(cateyeCount, 0);

	if (!initializedData) {
		for (int i = 0; i < cateyeCount; i++) {
			cateyeValues[i] = FileManager::saveData.at("catseyes")[i];
		}
		initializedData = true;
	}

	if (ImGui::BeginChild("Item Editor", ImVec2(0, ImGui::GetWindowSize().y - 150), true)) {
		ImGui::Text("Item Editor");

		for (int i = 0; i < cateyeCount; i++) {
			static Textures::Texture cateyeTexture;
			Textures::load_texture(cateyeTexture, "res/Images/Items/Cateye" + std::to_string(i) + ".png");

			ImGui::Image((ImTextureID)(intptr_t)cateyeTexture.texture, ImVec2(40, 40)); ImGui::SameLine();
			Widgets::render_drag_int_range(("Cateye" + std::to_string(i)).c_str(), &cateyeValues[i], 0, 999);
		}
		ImGui::EndChild();
	}
}

void BCats::Editor::render_edit_gamatoto()
{
}

void BCats::Editor::render_edit_talents()
{
}

void BCats::Editor::render_edit_levels()
{
}
