#include "Textures.h"

void BCats::Textures::load_texture(BCats::Textures::Texture& texture, filePath path) {
    if (loadedTexturesMap.count(path)) {
        texture = loadedTexturesMap.at(path);
        return;
    }

    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(path.c_str(), &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        texture = {};

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    texture.texture = image_texture;
    texture.width = image_width;
    texture.height = image_height;

    loadedTexturesMap[path] = texture;
}

inline void BCats::Textures::unload_texture(filePath path) {
    glDeleteTextures(1, &loadedTexturesMap[path].texture);
}