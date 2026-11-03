#ifndef GLSR_VAO_MODEL_H
#define GLSR_VAO_MODEL_H

#include <map>

#include "VaoArray.h"

namespace glsr::Services::Buffer::Models
{
    struct VaoModel
    {
        std::map<srGLuint, VaoArray> attribPointers;

        srGLuint elementsBufferId;
    };
}

#endif // GLSR_VAO_MODEL_H