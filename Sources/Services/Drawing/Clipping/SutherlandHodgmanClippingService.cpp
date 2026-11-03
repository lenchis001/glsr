#include "SutherlandHodgmanClippingService.h"

#include <cassert>
#include <algorithm>

#include "srglext.h"

using namespace glsr::Services::Drawing::Clipping;

std::vector<std::array<std::vector<OutputValue>, 3>> 
    SutherlandHodgmanClippingService::clip(const std::array<std::vector<Shader::Models::OutputValue>, 3> &vertices, float nearPlaneWEpsilon)
{
    // Sutherland-Hodgman clip against the single "w > epsilon" plane: a triangle
    // produces a convex polygon of 0, 3 or 4 vertices, which is then fan-triangulated.
    std::vector<std::vector<OutputValue>> outputPolygon;

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        const auto &current = vertices[i];
        const auto &previous = vertices[(i + vertices.size() - 1) % vertices.size()];

        auto currentClipSpaceW = _getClipSpaceW(current); 
        auto previousClipSpaceW = _getClipSpaceW(previous);

        bool isCurrentInside = currentClipSpaceW > nearPlaneWEpsilon;
        bool isPreviousInside = previousClipSpaceW > nearPlaneWEpsilon;

        if (isCurrentInside != isPreviousInside)
        {
            float t = (nearPlaneWEpsilon - previousClipSpaceW) / (currentClipSpaceW - previousClipSpaceW);
            outputPolygon.push_back(_lerpVertexOutputs(previous, current, t));
        }

        if (isCurrentInside)
            outputPolygon.push_back(current);
    }

    std::vector<std::array<std::vector<OutputValue>, 3>> clippedTriangles;
    for (size_t i = 1; i + 1 < outputPolygon.size(); ++i)
    {
        clippedTriangles.push_back({outputPolygon[0], outputPolygon[i], outputPolygon[i + 1]});
    }

    return clippedTriangles;
}


float SutherlandHodgmanClippingService::_getClipSpaceW(const std::vector<OutputValue> &vertexOutputs)
{
    auto glPosition = std::find_if(vertexOutputs.begin(), vertexOutputs.end(), [](const auto &outputValue)
                                    { return outputValue.name == GL_POSITION_NAME; });
    assert(glPosition != vertexOutputs.end() && "Vertex shader did not write gl_Position");
    return glPosition->value.floatVector4dValue.w;
}

std::vector<OutputValue> SutherlandHodgmanClippingService::_lerpVertexOutputs(
    const std::vector<OutputValue> &a,
    const std::vector<OutputValue> &b,
    float t)
{
    std::vector<OutputValue> lerped;
    lerped.reserve(a.size());

    for (size_t i = 0; i < a.size(); ++i)
    {
        const auto &v0 = a[i];
        const auto &v1 = b[i];

        Value value;
        switch (v0.type)
        {
        case srGL_FLOAT:
            value.floatValue = v0.value.floatValue + t * (v1.value.floatValue - v0.value.floatValue);
            break;
        case srGL_FLOAT_VEC2:
            value.floatVector2dValue = {
                v0.value.floatVector2dValue.x + t * (v1.value.floatVector2dValue.x - v0.value.floatVector2dValue.x),
                v0.value.floatVector2dValue.y + t * (v1.value.floatVector2dValue.y - v0.value.floatVector2dValue.y)};
            break;
        case srGL_FLOAT_VEC3:
            value.floatVector3dValue = {
                v0.value.floatVector3dValue.x + t * (v1.value.floatVector3dValue.x - v0.value.floatVector3dValue.x),
                v0.value.floatVector3dValue.y + t * (v1.value.floatVector3dValue.y - v0.value.floatVector3dValue.y),
                v0.value.floatVector3dValue.z + t * (v1.value.floatVector3dValue.z - v0.value.floatVector3dValue.z)};
            break;
        case srGL_FLOAT_VEC4:
            value.floatVector4dValue = {
                v0.value.floatVector4dValue.x + t * (v1.value.floatVector4dValue.x - v0.value.floatVector4dValue.x),
                v0.value.floatVector4dValue.y + t * (v1.value.floatVector4dValue.y - v0.value.floatVector4dValue.y),
                v0.value.floatVector4dValue.z + t * (v1.value.floatVector4dValue.z - v0.value.floatVector4dValue.z),
                v0.value.floatVector4dValue.w + t * (v1.value.floatVector4dValue.w - v0.value.floatVector4dValue.w)};
            break;
        default:
            assert(false && "Unsupported vertex shader output type for near-plane clipping");
            break;
        }

        lerped.push_back(OutputValue{v0.name, v0.type, value});
    }

    return lerped;
}