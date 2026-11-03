#ifndef GLSR_IDRAWING_SERVICE_H
#define GLSR_IDRAWING_SERVICE_H

#include "srtypes.h"

namespace glsr::Services::Drawing
{
    class IDrawingService
    {
    public:
        virtual ~IDrawingService() = default;
        
        virtual void drawArrays(srGLenum mode, srGLint first, srGLsizei count) = 0;

        virtual void drawElements(srGLenum mode, srGLsizei count, srGLenum type, const void *indices) = 0;

        virtual void viewport(srGLint x, srGLint y, srGLsizei width, srGLsizei height) = 0;

        virtual void readPixels(srGLint x, srGLint y, srGLsizei width, srGLsizei height, srGLenum format, srGLenum type, void *data) = 0;

        virtual void clearColor(srGLfloat red, srGLfloat green, srGLfloat blue, srGLfloat alpha) = 0;

        virtual void clear(srGLbitfield mask) = 0;
    };
}

#endif // GLSR_IDRAWING_SERVICE_H