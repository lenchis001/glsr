#ifndef GLSR_VAO_ARRAY_H
#define GLSR_VAO_ARRAY_H

#include "srtypes.h"

namespace glsr::Services::Buffer::Models
{
    struct VaoArray
    {
        srGLint size;
        srGLenum type;
        srGLboolean normalized;
        srGLsizei stride;
        const void *pointer;

        srGLboolean isEnabled;

        srGLuint bufferId;
    };
}

#endif // GLSR_VAO_ARRAY_H