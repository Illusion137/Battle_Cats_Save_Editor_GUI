#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <nlohmann/json.hpp>

#include <boost/regex.hpp>

#include <iostream>
#include <stack>
#include <tuple>
#include <string>
#include <vector>
#include <deque>
#include <unordered_set>

#include "Settings.h"

#include "Widgets.h"
#include "Textures.h"
#include "CatInfo.h"
#include "FileMananger.h"

namespace BCats {
	namespace Editor {
		enum Editor_ {
			Editor_Cats,
			Editor_Items,
			Editor_Gamatoto,
			Editor_Talents,
			Editor_Levels,
		};

		inline Editor_ currentEditor = Editor_Cats;

		extern void on_gui_render();
		extern void render_edit_cats();
		extern void render_cats(const int unitID, CatInfo::Cat& catData);
		extern void render_filters_popup_modal(bool* open);
		extern void render_edit_items();
		extern void render_edit_gamatoto();
		extern void render_edit_talents();
		extern void render_edit_levels();
	}
}