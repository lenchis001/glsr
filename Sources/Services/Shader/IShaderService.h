#ifndef GLSR_ISHADER_SERVICE_H
#define GLSR_ISHADER_SERVICE_H

#include <vector>

#include "srtypes.h"

#include "Models/InputValue.h"
#include "Models/OutputValue.h"

namespace glsr::Services::Shader
{
    class IShaderService
    {
    public:
        virtual ~IShaderService() = default;

        virtual srGLuint createProgram() = 0;

        virtual srGLuint createShader(srGLenum shaderType) = 0;

        virtual void shaderSource(srGLuint shader, srGLsizei count, const srGLchar *const *string, const srGLint *length) = 0;

        virtual void compileShader(srGLuint shader) = 0;

        virtual void getShaderiv(srGLuint shader, srGLenum pname, srGLint *params) = 0;

        virtual void getShaderInfoLog(srGLuint shader, srGLsizei bufSize, srGLsizei *length, srGLchar *infoLog) = 0;

        virtual void deleteShader(srGLuint shader) = 0;

        virtual void attachShader(srGLuint program, srGLuint shader) = 0;

        virtual void linkProgram(srGLuint program) = 0;

        virtual void getProgramIv(srGLuint program, srGLenum pname, srGLint *params) = 0;

        virtual void getProgramInfoLog(srGLuint program, srGLsizei bufSize, srGLsizei *length, srGLchar *infoLog) = 0;

        virtual void useProgram(srGLuint program) = 0;

        virtual srGLint getUniformLocation(srGLuint program, const std::string &name) = 0;

        virtual void uniform1i(srGLint location, srGLint value) = 0;

        virtual void uniform3fv(srGLint location, srGLsizei count, const srGLfloat *value) = 0;

        virtual void uniformMatrix4fv(srGLint location, srGLsizei count, srGLboolean transpose, const srGLfloat *value) = 0;

        virtual void deleteProgram(srGLuint program) = 0;

        virtual srGLuint getBoundProgram() const = 0;

        virtual std::vector<Models::OutputValue> runShader(srGLuint program, srGLenum shaderType, const std::vector<Models::InputValue> &inputs) = 0;
    };
}

#endif // GLSR_ISHADER_SERVICE_H