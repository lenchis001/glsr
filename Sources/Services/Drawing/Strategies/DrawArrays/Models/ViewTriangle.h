#ifndef GLSR_VIEW_TRIANGLE
#define GLSR_VIEW_TRIANGLE

#include "Services/Shader/Models/OutputValue.h"

#include "Services/Drawing/Models/Point.h"

#include <array>
#include <vector>

#include "srtypes.h"

namespace glsr::Services::Drawing::Strategies::DrawArrays::Models
{
    template <typename T>
    struct ViewTriangle
    {
        ViewTriangle(
            const std::array<glsr::Services::Drawing::Models::Point<T>, 3> &points,
            const std::array<std::vector<glsr::Services::Shader::Models::OutputValue>, 3> &vertexShaderCustomOutputs,
            const std::array<float, 3> &inverseClipW = {1.0f, 1.0f, 1.0f},
            const std::array<float, 3> &depths = {0.0f, 0.0f, 0.0f})
            : points(points), vertexShaderCustomOutputs(vertexShaderCustomOutputs), inverseClipW(inverseClipW), depths(depths)
        {
        }

        std::array<glsr::Services::Drawing::Models::Point<T>, 3> points;
        std::array<std::vector<glsr::Services::Shader::Models::OutputValue>, 3> vertexShaderCustomOutputs;

        // 1/w per vertex (clip space), needed to perspective-correct the affine
        // screen-space barycentric weights when interpolating custom outputs.
        std::array<float, 3> inverseClipW;

        // Full-precision NDC depth per vertex; points.z only carries screen-space
        // integer coordinates and is not precise enough for depth testing.
        std::array<float, 3> depths;
    };
}

#endif // GLSR_VIEW_TRIANGLE