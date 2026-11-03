#ifndef GLSR_IBUFFER_SERVICE_H
#define GLSR_IBUFFER_SERVICE_H

#include "srtypes.h"

#include "Models/VboModel.h"
#include "Models/VaoModel.h"

namespace glsr::Services::Buffer
{
    class IBufferService
    {
    public:
        virtual ~IBufferService() = default;

        virtual void genBuffers(srGLsizei n, srGLuint *buffers) = 0;

        virtual void deleteBuffers(srGLsizei n, const srGLuint *buffers) = 0;

        virtual void bindBuffer(srGLenum target, srGLuint buffer) = 0;

        virtual void bufferData(srGLenum target, srGLsizeiptr size, const void *data, srGLenum usage) = 0;

        virtual void bufferSubData(srGLenum target, srGLintptr offset, srGLsizeiptr size, const void *data) = 0;

        virtual const Models::VboModel& getBufferedData(srGLuint buffer) const = 0;

        virtual void genVertexArrays(srGLsizei n, srGLuint *arrays) = 0;

        virtual void bindVertexArray(srGLuint array) = 0;

        virtual void vertexAttribPointer(srGLuint index, srGLint size, srGLenum type, srGLboolean normalized, srGLsizei stride, const void *pointer) = 0;

        virtual void enableVertexAttribArray(srGLuint index) = 0;

        virtual void deleteVertexArrays(srGLsizei n, const srGLuint *arrays) = 0;

        virtual srGLuint getBoundVao() const = 0;

        virtual const Models::VaoModel& getVao(srGLuint target) const = 0;
    };
}

#endif // GLSR_IBUFFER_SERVICE_H