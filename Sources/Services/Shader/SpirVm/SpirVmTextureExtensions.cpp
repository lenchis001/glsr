#include "SpirVmTextureExtensions.h"

#include <cassert>
#include <cstdint>
#include <vector>

#include "SpirVmExtensions.h"

using namespace glsr::Models;
using namespace glsr::Services::Texture::Models;

using namespace glsr::Services::Shader::SpirVm;

bool SpirVmTextureExtensions::setSampler2D(
    spvm_state_t state,
    const std::string &name,
    TextureModel &textureModel)
{
    spvm_result_t result = SpirVmExtensions::findUniformByName(state, name);
    if (!result)
        return false;

    // Heap-allocated so pointers remain valid after setSampler2D returns
    auto* imageData = new spvm_image_data{};
    auto data = reinterpret_cast<const float*>(textureModel.getPixelData().data());
    spvm_image_create(imageData, const_cast<float*>(data), textureModel.getWidth(), textureModel.getHeight(), 1);

    auto* sampler = new spvm_sampler{};
    *sampler = _createSampler(textureModel);

    result->members[0].value.image = &imageData->base;
    result->members[1].value.sampler = sampler;

    return true;
}

bool SpirVmTextureExtensions::setSamplerCube(
    spvm_state_t state,
    const std::string &name,
    const TextureModel &textureModel)
{
    spvm_result_t result = SpirVmExtensions::findUniformByName(state, name);
    if (!result)
        return false;

    const size_t facePixelCount = static_cast<size_t>(textureModel.getWidth()) * textureModel.getHeight();

    const auto &pixelData = textureModel.getPixelData();

    auto *imageData = new spvm_image_data{};
    spvm_image_create_ext(
        imageData,
        const_cast<float*>(reinterpret_cast<const float *>(pixelData.data())),
        textureModel.getWidth(),
        textureModel.getHeight(),
        1,
        6,
        1);

    auto *sampler = new spvm_sampler{};
    *sampler = _createSampler(textureModel);

    result->members[0].value.image = &imageData->base;
    result->members[1].value.sampler = sampler;

    return true;
}

std::map<std::string, srGLuint> SpirVmTextureExtensions::findSampler2DUniformNames(spvm_state_t state)
{
	std::map<std::string, srGLuint> samplerUniformTextureUnits;

	if (!state || !state->owner)
		return samplerUniformTextureUnits;

	for (spvm_word i = 0; i < state->owner->bound; i++)
	{
		spvm_result_t result = &state->results[i];

		if (!_isSamplerWithDimension(state, result, SpvDim2D))
			continue;

		samplerUniformTextureUnits.insert_or_assign(result->name, _getBinding(result));
	}

	return samplerUniformTextureUnits;
}

std::map<std::string, srGLuint> SpirVmTextureExtensions::findSamplerCubeUniformNames(spvm_state_t state)
{
	std::map<std::string, srGLuint> samplerUniformTextureUnits;

	if (!state || !state->owner)
		return samplerUniformTextureUnits;

	for (spvm_word i = 0; i < state->owner->bound; i++)
	{
		spvm_result_t result = &state->results[i];
		if (!_isSamplerWithDimension(state, result, SpvDimCube))
			continue;

		samplerUniformTextureUnits.insert_or_assign(result->name, _getBinding(result));
	}

	return samplerUniformTextureUnits;
}

spvm_sampler SpirVmTextureExtensions::_createSampler(const Texture::Models::TextureModel &textureModel)
{
    spvm_sampler sampler{};

    sampler.desc.filter_min = _toSamplerFilter(textureModel.getParameter(srGL_TEXTURE_MIN_FILTER, srGL_NEAREST_MIPMAP_LINEAR));
    sampler.desc.filter_mag = _toSamplerFilter(textureModel.getParameter(srGL_TEXTURE_MAG_FILTER, srGL_LINEAR));
    sampler.desc.mipmap_mode = spvm_sampler_filter_nearest;
    sampler.desc.address_mode_u = _toSamplerAddressMode(textureModel.getParameter(srGL_TEXTURE_WRAP_S, srGL_REPEAT));
    sampler.desc.address_mode_v = _toSamplerAddressMode(textureModel.getParameter(srGL_TEXTURE_WRAP_T, srGL_REPEAT));
    sampler.desc.address_mode_w = _toSamplerAddressMode(textureModel.getParameter(srGL_TEXTURE_WRAP_R, srGL_REPEAT));
    sampler.desc.border_color = spvm_vec4f{{0.0f, 0.0f, 0.0f, 0.0f}};
    sampler.desc.mip_bias = 0.0f;
    sampler.desc.compare_op = spvm_sampler_compare_op_never;
    sampler.desc.min_lod = 0.0f;
    sampler.desc.max_lod = 0.0f; // textures have a single mip level in this renderer

    return sampler;
}

spvm_sampler_address_mode SpirVmTextureExtensions::_toSamplerAddressMode(srGLint wrapMode)
{
    switch (wrapMode)
    {
    case srGL_CLAMP_TO_EDGE:
        return spvm_sampler_address_mode_clamp_to_edge;
    case srGL_CLAMP_TO_BORDER:
        return spvm_sampler_address_mode_clamp_to_border;
    case srGL_MIRRORED_REPEAT:
        return spvm_sampler_address_mode_mirrored_repeat;
    case srGL_REPEAT:
    default:
        return spvm_sampler_address_mode_repeat;
    }
}

spvm_sampler_filter SpirVmTextureExtensions::_toSamplerFilter(srGLint filter)
{
    switch (filter)
    {
    case srGL_LINEAR:
    case srGL_LINEAR_MIPMAP_NEAREST:
    case srGL_LINEAR_MIPMAP_LINEAR:
        return spvm_sampler_filter_linear;
    case srGL_NEAREST:
    case srGL_NEAREST_MIPMAP_NEAREST:
    case srGL_NEAREST_MIPMAP_LINEAR:
    default:
        return spvm_sampler_filter_nearest;
    }
}

bool SpirVmTextureExtensions::_isSamplerWithDimension(spvm_state_t state, spvm_result_t result, SpvDim dimension)
{
    if (!result || result->storage_class != SpvStorageClassUniformConstant || !result->name)
        return false;

    spvm_result_t sampledImageType = spvm_state_get_type_info(
        state->results,
        &state->results[result->pointer]);
    if (!sampledImageType || sampledImageType->value_type != spvm_value_type_sampled_image)
        return false;

    spvm_result_t imageType = spvm_state_get_type_info(
        state->results,
        &state->results[sampledImageType->pointer]);
    return imageType && imageType->value_type == spvm_value_type_image &&
        imageType->image_info && imageType->image_info->dim == dimension;
}

srGLuint SpirVmTextureExtensions::_getBinding(spvm_result_t result)
{
    for (spvm_word d = 0; d < result->decoration_count; d++)
    {
        if (result->decorations[d].type == SpvDecorationBinding)
            return static_cast<srGLuint>(result->decorations[d].literal1);
    }

    return 0;
}
