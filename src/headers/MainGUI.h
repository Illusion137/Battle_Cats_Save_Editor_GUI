#pragma once
#include "Textures.h"
#include "BCatsData.h"

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

namespace BCats {
	namespace Main {
		enum Screens{ Screen_HOME };

		extern int opengl_main_loop();
		extern void on_gui_render(bool *open);
		extern void render_home();
		extern void render_download_save_modal_popup(bool* open);

		extern void on_exit_success();
		extern Screens currentScreen;
	}
}