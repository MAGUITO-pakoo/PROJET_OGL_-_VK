#pragma once

#include "NKMath/NkMat4x4.h"
#include "NKMath/NKVec3f.h"
#include <string>

namespace nkentseu {

class ShaderProgram {
public:
    ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void Use() const;
    bool IsValid() const;

    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, float value) const;
    void SetUniform(const std::string& name, const NkVec3f& value) const;
    void SetUniform(const std::string& name, const NkMat4x4& value) const;

private:
    unsigned int mProgramId = 0;

    unsigned int CompileShader(const std::string& source, unsigned int shaderType, const std::string& name) const;
    bool CheckCompileErrors(unsigned int handle, const std::string& type) const;
    int GetUniformLocation(const std::string& name) const;
};

} // namespace nkentseu
