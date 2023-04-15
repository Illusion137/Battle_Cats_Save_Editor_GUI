#pragma once

#include "vendor/stb_image.h"

#include "imgui.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <set>
#include <unordered_map>

namespace BCats {
	namespace Textures {
		typedef std::string filePath;
		struct Texture {
			int height;
			int width;
			GLuint texture;
		};

		inline std::unordered_map<filePath, Texture> loadedTexturesMap;

		extern void load_texture(Texture& texture, filePath path);
		extern inline void unload_texture(filePath path);
	}
}