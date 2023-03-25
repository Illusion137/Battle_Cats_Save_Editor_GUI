#pragma once

#include "../vendor/stb_image.h"

#include "imgui.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <set>

namespace BCats {
	namespace Textures {
		typedef std::string filePath;
		struct Texture {
			int height;
			int width;
			GLuint texture;
		};

		extern std::set<filePath> loadedTexturesSet;

		extern Texture load_texture(filePath path);
		extern void unload_texture(filePath path);
	}
}