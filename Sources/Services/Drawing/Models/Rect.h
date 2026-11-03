#ifndef GLSR_RECT_H
#define GLSR_RECT_H

#include "srtypes.h"

namespace glsr::Services::Drawing::Models
{
    struct Rect
    {
        Rect(srGLint x, srGLint y, srGLsizei width, srGLsizei height)
            : x(x), y(y), width(width), height(height) {}

        srGLint x;
        srGLint y;
        srGLsizei width;
        srGLsizei height;
    };
}

#endif // GLSR_RECT_H