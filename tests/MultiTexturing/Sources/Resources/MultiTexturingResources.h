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

class MultiTexturingResources final
{
public:
	MultiTexturingResources() = delete;

    static const ResourceView* GetAll(std::size_t& outCount);
    static const unsigned char* GetContent(const char* fileName, std::size_t& outSize);
	static const ResourceView* FindByName(const char* fileName);

    static const ResourceView fragment_shader_glsl;
    static const ResourceView shadows_png;
    static const ResourceView texture_jpg;
    static const ResourceView vertex_shader_glsl;
};

} // namespace BakedResources
