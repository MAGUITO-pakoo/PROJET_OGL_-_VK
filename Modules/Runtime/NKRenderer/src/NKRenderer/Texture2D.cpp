#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "NKRenderer/Texture2D.h"
#include <iostream>
#include "glad/gl.h"

namespace nkentseu {

Texture2D::Texture2D(const std::string& filePath) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mChannels, 0);
    if (!data) {
        std::cerr << "Texture2D::Texture2D - échec du chargement de " << filePath << std::endl;
        return;
    }

    GLenum format = GL_RGB;
    if (mChannels == 4) {
        format = GL_RGBA;
    } else if (mChannels == 3) {
        format = GL_RGB;
    } else if (mChannels == 1) {
        format = GL_RED;
    }

    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

Texture2D::~Texture2D() {
    if (mTextureId != 0) {
        glDeleteTextures(1, &mTextureId);
    }
}

bool Texture2D::IsValid() const {
    return mTextureId != 0;
}

void Texture2D::Bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
}

} // namespace nkentseu
