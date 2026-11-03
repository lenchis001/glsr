#ifndef GLSR_VBO_MODEL_H
#define GLSR_VBO_MODEL_H

#include <vector>
#include <cstddef>

#include "srtypes.h"

namespace glsr::Services::Buffer::Models
{
    struct VboModel
    {
        srGLsizeiptr size;
        std::vector<std::byte> data;
        srGLenum usage;
    };
}

#endif // GLSR_VBO_MODEL_H