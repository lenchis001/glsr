#ifndef GLSR_SPIRVM_TEXTURE_EXTENSIONS_H
#define GLSR_SPIRVM_TEXTURE_EXTENSIONS_H

#include <map>
#include <string>
#include <array>

#include <spvm/state.h>
#include <spvm/image.h>

#include "srtypes.h"
#include "Models/Vector4d.h"

#include "Services/Texture/Models/TextureModel.h"

namespace glsr::Services::Shader::SpirVm
{
    class SpirVmTextureExtensions
    {
    public:
        static bool setSampler2D(
            spvm_state_t state,
            const std::string &name,
            Texture::Models::TextureModel &textureModel);

        static bool setSamplerCube(
            spvm_state_t state,
            const std::string &name,
            const Texture::Models::TextureModel &textureModel);

        static std::map<std::string, srGLuint> findSampler2DUniformNames(spvm_state_t state);

        static std::map<std::string, srGLuint> findSamplerCubeUniformNames(spvm_state_t state);

    private:
        static spvm_sampler _createSampler(const Texture::Models::TextureModel &textureModel);
        static spvm_sampler_address_mode _toSamplerAddressMode(srGLint wrapMode);
        static spvm_sampler_filter _toSamplerFilter(srGLint filter);
        static bool _isSamplerWithDimension(spvm_state_t state, spvm_result_t result, SpvDim dimension);
        static srGLuint _getBinding(spvm_result_t result);
    };
}

#endif // GLSR_SPIRVM_TEXTURE_EXTENSIONS_H