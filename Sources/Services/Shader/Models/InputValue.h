#ifndef GLSR_SHADER_INPUT_VALUE_H
#define GLSR_SHADER_INPUT_VALUE_H

#include <string>
#include <cassert>

#include "srtypes.h"

#include "Value.h"

namespace glsr::Services::Shader::Models
{
    struct InputValue
    {
        InputValue(srGLuint location, srGLenum type, const Value &value)
            : location(location), name(""), type(type), value(value)
        {
            assert(location >= 0 && "Location must be non-negative");
        }

        InputValue(const std::string &name, srGLenum type, const Value &value)
            : location(-1), name(name), type(type), value(value)
        {
            assert(!name.empty() && "Name must not be empty");
        }

        bool isNamed() const
        {
            return !name.empty();
        }

        srGLuint location;
        std::string name;

        srGLenum type;
        Value value;
    };
}

#endif // GLSR_SHADER_INPUT_VALUE_H