#ifndef GLSR_SHADER_OUTPUT_VALUE_H
#define GLSR_SHADER_OUTPUT_VALUE_H

#include <string>

#include "srtypes.h"
#include "Value.h"

namespace glsr::Services::Shader::Models
{
    struct OutputValue
    {
        std::string name;

        srGLenum type;
        Value value;
    };
}

#endif // GLSR_SHADER_OUTPUT_VALUE_H