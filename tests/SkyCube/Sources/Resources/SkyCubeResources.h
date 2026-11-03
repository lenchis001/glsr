#pragma once

#include <cstddef>

namespace BakedResources
{

struct ResourceView
{
	const char* path;
	const unsigned char* data;
	std::size_t size;
};

class SkyCubeResources final
{
public:
	SkyCubeResources() = delete;

    static const ResourceView* GetAll(std::size_t& outCount);
    static const unsigned char* GetContent(const char* fileName, std::size_t& outSize);
	static const ResourceView* FindByName(const char* fileName);

    static const ResourceView ds_store;
    static const ResourceView fragment_shader_glsl;
    static const ResourceView skybox_back_jpg;
    static const ResourceView skybox_bottom_jpg;
    static const ResourceView skybox_front_jpg;
    static const ResourceView skybox_left_jpg;
    static const ResourceView skybox_right_jpg;
    static const ResourceView skybox_top_jpg;
    static const ResourceView vertex_shader_glsl;
};

} // namespace BakedResources
