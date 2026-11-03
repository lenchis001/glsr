#ifndef GLSR_IPROGRAM_WORKER_H
#define GLSR_IPROGRAM_WORKER_H

#include <memory>
#include <vector>

#include "srtypes.h"

#include "Models/InputValue.h"
#include "Models/OutputValue.h"
#include "Models/ShaderModel.h"

namespace glsr::Services::Shader
{
    class IProgramWorker
    {
    public:
        virtual ~IProgramWorker() = default;

        virtual void attachShader(std::shared_ptr<Models::ShaderModel> shaderModel) = 0;

        virtual std::vector<Models::OutputValue> runShader(srGLenum shaderType, const std::vector<Models::InputValue> &inputs) = 0;

        virtual srGLint getUniformLocation(const std::string &name) = 0;

        virtual void uniform1i(srGLint location, srGLint value) = 0;

        virtual void uniform3fv(srGLint location, srGLsizei count, const srGLfloat *value) = 0;

        virtual void uniformMatrix4fv(srGLint location, srGLsizei count, srGLboolean transpose, const srGLfloat *value) = 0;
    };
}

#endif // GLSR_IPROGRAM_WORKER_H