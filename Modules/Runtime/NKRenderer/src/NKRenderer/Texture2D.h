#pragma once

#include <string>

namespace nkentseu {

class Texture2D {
public:
    Texture2D() = default;
    explicit Texture2D(const std::string& filePath);
    ~Texture2D();

    bool IsValid() const;
    void Bind(unsigned int unit = 0) const;

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }

private:
    unsigned int mTextureId = 0;
    int mWidth = 0;
    int mHeight = 0;
    int mChannels = 0;
};
}