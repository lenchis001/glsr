#ifndef GLSR_BUFFER_SERVICE_H
#define GLSR_BUFFER_SERVICE_H

#include <map>

#include "IBufferService.h"

using namespace glsr::Services::Buffer::Models;

namespace glsr::Services::Buffer
{
    class BufferService : public IBufferService
    {
    public:
        BufferService();

        void genBuffers(srGLsizei n, srGLuint *buffers) override;

        void deleteBuffers(srGLsizei n, const srGLuint *buffers) override;

        void bindBuffer(srGLenum target, srGLuint buffer) override;

        void bufferData(srGLenum target, srGLsizeiptr size, const void *data, srGLenum usage) override;

        void bufferSubData(srGLenum target, srGLintptr offset, srGLsizeiptr size, const void *data) override;

        const VboModel& getBufferedData(srGLuint buffer) const override;

        void genVertexArrays(srGLsizei n, srGLuint *arrays) override;

        void bindVertexArray(srGLuint array) override;

        void vertexAttribPointer(srGLuint index, srGLint size, srGLenum type, srGLboolean normalized, srGLsizei stride, const void *pointer) override;

        void enableVertexAttribArray(srGLuint index) override;

        void deleteVertexArrays(srGLsizei n, const srGLuint *arrays) override;

        srGLuint getBoundVao() const override;

        const VaoModel& getVao(srGLuint target) const override;
    private:
        std::map<srGLuint, VboModel> _buffers;
        srGLuint _nextVboId;
        std::map<srGLenum, srGLuint> _boundBuffers;

        srGLuint _nextVaoId;
        std::map<srGLuint, VaoModel> _arrays;
        srGLuint _boundVao;
    };
}

#endif // GLSR_BUFFER_SERVICE_H