#ifndef GLSR_FRAGMENT_H
#define GLSR_FRAGMENT_H

#include <vector>

#include "Services/Drawing/Models/Point.h"
#include "Services/Shader/Models/InputValue.h"

namespace glsr::Services::Drawing::Models
{
    struct Fragment
    {
        Fragment(
            const Drawing::Models::Point<srGLsizei> &position,
            const float depth,
            const std::vector<Shader::Models::InputValue> &fragmentShaderInputs)
            : position(position),
              depth(depth),
              fragmentShaderInputs(fragmentShaderInputs)
        {
        }

        Drawing::Models::Point<srGLsizei> position;
        float depth;

        std::vector<Shader::Models::InputValue> fragmentShaderInputs;
    };
}

#endif // GLSR_FRAGMENT_H