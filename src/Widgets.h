#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <Windows.h>

#include <iostream>
#include <string>

#include "Textures.h"

#define BUTTON_SIZE_X 254
#define BUTTON_SIZE_Y 48

namespace BCats {
	namespace Widgets {
		namespace Animation{
			struct ButtonAnimation {
				bool isPurpleCycle;
				unsigned char hoverTick;
				bool isPressedAnimationFinished;
				unsigned char pressedTick;
			};
			struct ImageButtonAnimation {
				bool isPressedAnimationFinished;
				unsigned char pressedTick;
			};
		}
	}
}

namespace ImGui {
	//https://github.com/ocornut/imgui/issues/4722
	bool GradientButton(const char* label, const ImVec2& size, ImU32 text_color, ImU32 bg_color_1, ImU32 bg_color_2, 
		BCats::Widgets::Animation::ButtonAnimation& animation);
}
//ImDrawList::AddText imgui_draw.cpp & imconfig.h MODIFIED
namespace BCats {
	namespace Widgets {
		inline ImFont* defaultFont;
		inline ImFont* outlinedFont;
		extern bool render_image_button(const char* str_id, ImTextureID user_texture_id, Animation::ImageButtonAnimation& anim, const ImVec2& size,
			const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), ImVec4& tint_col = ImVec4(1, 1, 1, 1), ImGuiButtonFlags flags = 0);
		
		extern bool render_button(std::string title, Animation::ButtonAnimation& animation,
			ImVec2 size = ImVec2(BUTTON_SIZE_X, BUTTON_SIZE_Y), ImU32 textColor = IM_COL32(255, 255, 255, 255) );
		extern void render_text(std::string title, ImU32 textColor = IM_COL32(255, 255, 255, 255));
		extern void render_outlined_text(std::string title, ImU32 textColor = IM_COL32(255, 255, 255, 255));
		extern bool render_checkbox(std::string label, bool* checkBoxActive);

		extern bool render_toggle_button_text(std::string title, bool& value, Animation::ButtonAnimation& anim, ImU32 toggledOnColor = IM_COL32(0x5a, 0x99, 0x56, 255), ImVec2 size = ImVec2(BUTTON_SIZE_X, BUTTON_SIZE_Y), ImU32 toggledOnColorGradient = IM_COL32(0x3c, 0x67, 0x3a, 255),
			ImU32 textColor = IM_COL32(255, 255, 255, 255), ImU32 toggledOffColor = IM_COL32(0x78, 0x78, 0x78, 0xff));
		extern bool render_toggle_button_icon(const char* strId, ImTextureID iconTexture, ImVec2 size, bool& value, Animation::ImageButtonAnimation& anim, ImVec4 toggledOnColor = ImVec4(0.23, 1, 0.24, 1),
			ImVec4 toggledOffColor = ImVec4(0.76, 0.76, 0.76, 1));

		extern bool render_drag_int_range(const char* label, int* data, const int min = 0, const int max = 99, ImU32 textColor = IM_COL32(0, 0, 255, 255));
		
		extern void render_tooltip(const char* message, float delaySeconds = 0.5);

	}
}