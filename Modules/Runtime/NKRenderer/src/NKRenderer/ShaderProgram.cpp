#include "NKRenderer/ShaderProgram.h"
#include <glad/gl.h>
#include <fstream>
#include <iostream>
#include <sstream>

namespace nkentseu {

static std::string ReadTextFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    std::string vertexSource = ReadTextFile(vertexShaderPath);
    std::string fragmentSource = ReadTextFile(fragmentShaderPath);

    if (vertexSource.empty()) {
        std::cerr << "ShaderProgram error: impossible de lire le vertex shader '" << vertexShaderPath << "'\n";
        return;
    }
    if (fragmentSource.empty()) {
        std::cerr << "ShaderProgram error: impossible de lire le fragment shader '" << fragmentShaderPath << "'\n";
        return;
    }

    unsigned int vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER, vertexShaderPath);
    if (vertexShader == 0) {
        return;
    }

    unsigned int fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER, fragmentShaderPath);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return;
    }

    mProgramId = glCreateProgram();
    glAttachShader(mProgramId, vertexShader);
    glAttachShader(mProgramId, fragmentShader);
    glLinkProgram(mProgramId);

    if (!CheckCompileErrors(mProgramId, "PROGRAM")) {
        glDeleteProgram(mProgramId);
        mProgramId = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram() {
    if (mProgramId != 0) {
        glDeleteProgram(mProgramId);
    }
}

void ShaderProgram::Use() const {
    if (mProgramId != 0) {
        glUseProgram(mProgramId);
    }
}

bool ShaderProgram::IsValid() const {
    return mProgramId != 0;
}

void ShaderProgram::SetUniform(const std::string& name, int value) const {
    int location = GetUniformLocation(name);
    if (location >= 0) {
        glUniform1i(location, value);
    }
}

void ShaderProgram::SetUniform(const std::string& name, float value) const {
    int location = GetUniformLocation(name);
    if (location >= 0) {
        glUniform1f(location, value);
    }
}

void ShaderProgram::SetUniform(const std::string& name, const NkVec3f& value) const {
    int location = GetUniformLocation(name);
    if (location >= 0) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void ShaderProgram::SetUniform(const std::string& name, const NkMat4x4& value) const {
    int location = GetUniformLocation(name);
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
    }
}

unsigned int ShaderProgram::CompileShader(const std::string& source, unsigned int shaderType, const std::string& name) const {
    unsigned int shader = glCreateShader(shaderType);
    const char* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    if (!CheckCompileErrors(shader, name)) {
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool ShaderProgram::CheckCompileErrors(unsigned int handle, const std::string& type) const {
    GLint success = 0;
    if (type == "PROGRAM") {
        glGetProgramiv(handle, GL_LINK_STATUS, &success);
    } else {
        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    }

    if (success == GL_FALSE) {
        GLint logLength = 0;
        if (type == "PROGRAM") {
            glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
        } else {
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
        }

        std::string infoLog;
        infoLog.resize(static_cast<size_t>(logLength));
        if (type == "PROGRAM") {
            glGetProgramInfoLog(handle, logLength, nullptr, &infoLog[0]);
        } else {
            glGetShaderInfoLog(handle, logLength, nullptr, &infoLog[0]);
        }

        std::cerr << "ShaderProgram " << type << " error:\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

int ShaderProgram::GetUniformLocation(const std::string& name) const {
    if (mProgramId == 0) {
        return -1;
    }
    return glGetUniformLocation(mProgramId, name.c_str());
}

} // namespace nkentseu
