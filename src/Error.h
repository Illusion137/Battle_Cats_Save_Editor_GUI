#pragma once
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

namespace BCats {
	namespace Error {
		extern void render_error();
		inline bool openError = false;
		inline std::string exceptionError = "";
	}
}