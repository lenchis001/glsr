#ifndef GLSR_SHADER_SUPERVISOR_H
#define GLSR_SHADER_SUPERVISOR_H

#include "IShaderService.h"

#include <map>
#include <memory>

#include <glslang/Public/ShaderLang.h>

#include "Models/ShaderModel.h"
#include "IProgramWorker.h"

#include "Services/Texture/ITextureService.h"

using namespace glsr::Services::Shader::Models;

struct spvm_state;

namespace glsr::Services::Shader
{
    class ShaderSupervisor : public IShaderService
    {
    public:
        ShaderSupervisor(std::shared_ptr<Texture::ITextureService> textureService);

        srGLuint createProgram() override;

        srGLuint createShader(srGLenum shaderType) override;

        void shaderSource(srGLuint shader, srGLsizei count, const srGLchar *const *string, const srGLint *length) override;

        void compileShader(srGLuint shader) override;

        void getShaderiv(srGLuint shader, srGLenum pname, srGLint *params) override;

        void getShaderInfoLog(srGLuint shader, srGLsizei bufSize, srGLsizei *length, srGLchar *infoLog) override;

        void deleteShader(srGLuint shader) override;

        void attachShader(srGLuint program, srGLuint shader) override;

        void linkProgram(srGLuint program) override;

        void getProgramIv(srGLuint program, srGLenum pname, srGLint *params) override;

        void getProgramInfoLog(srGLuint program, srGLsizei bufSize, srGLsizei *length, srGLchar *infoLog) override;

        void useProgram(srGLuint program) override;

        srGLint getUniformLocation(srGLuint program, const std::string &name) override;

        void uniform1i(srGLint location, srGLint value) override;

        void uniform3fv(srGLint location, srGLsizei count, const srGLfloat *value) override;

        void uniformMatrix4fv(srGLint location, srGLsizei count, srGLboolean transpose, const srGLfloat *value) override;

        void deleteProgram(srGLuint program) override;

        srGLuint getBoundProgram() const override;

        std::vector<OutputValue> runShader(srGLuint program, srGLenum shaderType, const std::vector<InputValue> &inputs) override;

    private:
        EShLanguage _getGlslangShaderType(srGLenum shaderType);

        std::shared_ptr<Texture::ITextureService> _textureService;

        int _nextShaderId = 1;
        int _nextProgramId = 1;
        std::map<srGLuint, std::shared_ptr<Models::ShaderModel>> shaders;
        std::map<srGLuint, std::unique_ptr<IProgramWorker>> programs;

        srGLuint _boundProgram = 0;
    };
}

#endif // GLSR_SHADER_SUPERVISOR_H