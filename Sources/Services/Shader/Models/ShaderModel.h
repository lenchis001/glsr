#ifndef GLSR_SHADER_MODEL_H
#define GLSR_SHADER_MODEL_H

#include <string>
#include <vector>
#include <stdint.h>
#include <map>

#include "srtypes.h"

namespace glsr::Services::Shader::Models
{
    struct ShaderModel
    {
        ShaderModel(srGLenum shaderType);

        srGLenum shaderType;
        std::string sourceCode;
        std::vector<uint32_t> spirvCode;

        std::map<srGLenum, srGLint> compileStatus;
        std::string infoLog;
    };
}

#endif // GLSR_SHADER_MODEL_H