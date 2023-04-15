#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include "Settings.h"

#include "Textures.h"
#include "Widgets.h"
#include "Editor.h"
#include "FileMananger.h"
#include "Error.h"
#include "Helper.h"
#include "PlatformHelper.h"

#include <boost/regex.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <filesystem>
#include <exception>

#include <nlohmann/json.hpp>

#include "vendor/ImGuiFileDialog.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#define WINDOW_NAME "Battle Cat Save File Editor GUI"
#define WINDOW_ICON "res/Images/Icon.png"
#define WINDOW_SIZE_X 1213
#define WINDOW_SIZE_Y 669

#define FIERYHENRY_GITHUB_URL "https://github.com/fieryhenry"
#define ILLUSION137_GITHUB_URL "https://github.com/Illusion137"

namespace BCats {
	namespace Main {
		//Runs once when after initializing ImGui Context
		extern inline void on_enter();
		extern inline void load_window_icon(GLFWwindow* window); // https://stackoverflow.com/questions/44321902/glfw-setwindowicon
		//Initalizes both GLFW-Opengl and ImGui then runs main render loop
		extern int opengl_main_loop(); // https://github.com/ocornut/imgui/blob/docking/examples/example_glfw_opengl3/main.cpp

#ifndef TEST_EDITOR
		inline bool isHomeScreen = true;
#else
		inline bool isHomeScreen = false;
#endif // TEST_EDITOR
		extern void on_gui_render(bool* open); //Setup fullscreen viewport & render either Home | Editor

		extern void render_download_save_modal_popup(bool* open);

		extern void render_loading_popup(bool* open, const std::string threadMessage);

		struct ThreadLocalFileArgs {
			std::string filePath;
			bool& isFinished;
			bool& isGood;
			bool isJson;
			std::string& returnThreadMessage;
		};
		extern void thread_load_file(ThreadLocalFileArgs& args);

		extern void render_home_screen();
	}
}