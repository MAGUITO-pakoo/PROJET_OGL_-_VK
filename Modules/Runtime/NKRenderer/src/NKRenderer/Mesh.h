#pragma once

#include "NKMath/NKVec2f.h"
#include "NKMath/NKVec3f.h"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace nkentseu {

struct Vertex {
    ::NkVec3f position;
    ::NkVec3f normal;
    ::NkVec2f uv;
    ::NkVec3f tangent;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void Draw() const;

private:
    unsigned int mVAO = 0;
    unsigned int mVBO = 0;
    unsigned int mEBO = 0;
    uint32_t mIndexCount = 0;
};

} // namespace nkentseu
