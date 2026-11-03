#ifndef GLSR_PROGRAM_WORKER_H
#define GLSR_PROGRAM_WORKER_H

#include "IProgramWorker.h"

#include <map>
#include <string>

#include <spvm/context.h>
#include <spvm/state.h>

#include "Services/Texture/ITextureService.h"

using namespace glsr::Services::Texture;

using namespace glsr::Services::Shader::Models;

namespace glsr::Services::Shader
{
    class ProgramWorker : public IProgramWorker
    {
    public:
        ProgramWorker(std::shared_ptr<Texture::ITextureService> textureService);
        ~ProgramWorker() override;

        void attachShader(std::shared_ptr<Models::ShaderModel> shaderModel) override;

        std::vector<OutputValue> runShader(srGLenum shaderType, const std::vector<InputValue> &inputs) override;

        srGLint getUniformLocation(const std::string &name) override;

        void uniform1i(srGLint location, srGLint value) override;

        void uniform3fv(srGLint location, srGLsizei count, const srGLfloat *value) override;

        void uniformMatrix4fv(srGLint location, srGLsizei count, srGLboolean transpose, const srGLfloat *value) override;
    private:
        std::shared_ptr<ITextureService> _textureService;

        std::map<srGLenum, std::shared_ptr<Models::ShaderModel>> _shaderModels;

        spvm_context_t _context;

        std::map<srGLenum, spvm_source> _programs;
        std::map<std::string, srGLint> _uniformLocations;

        srGLint _nextUniformLocation = 0;

        std::map<srGLint, srGLint> _uniform1iValues;
        std::map<srGLint, std::vector<srGLfloat>> _uniform3fvValues;
        std::map<srGLint, std::vector<srGLfloat>> _uniformMatrix4fvValues;
    };
}

#endif // GLSR_PROGRAM_WORKER_H