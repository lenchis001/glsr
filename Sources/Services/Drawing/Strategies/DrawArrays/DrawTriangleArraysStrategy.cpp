#include "DrawTriangleArraysStrategy.h"

#include <cassert>
#include <cstdint>
#include <ranges>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "srglext.h"

using namespace glsr::Services::Drawing::Strategies::DrawArrays;

constexpr float kNearPlaneWEpsilon = 1e-5f;

DrawTriangleArraysStrategy::DrawTriangleArraysStrategy(
    srGLenum mode,
    srGLint first,
    srGLsizei count,
    std::shared_ptr<IShaderService> shaderService,
    std::shared_ptr<IBufferService> buffersService,
    std::shared_ptr<IConfigurationService> configurationService,
    std::shared_ptr<IClippingService> clippingService,
    const Rect &viewport)
    : DrawStrategyBase(first, count, std::move(buffersService), std::move(shaderService)),
      _viewport(viewport),
      _configurationService(std::move(configurationService)),
      _clippingService(std::move(clippingService))
{
    if (mode == srGL_TRIANGLES)
    {
        assert((count - first) % 3 == 0 && "Vertex count must be a multiple of 3 for GL_TRIANGLES mode");
    }
}

std::vector<Fragment> DrawTriangleArraysStrategy::processVertices()
{
    // Collect vertex shader inputs from bound VAO
    // every input - data for a single vertex shader run
    auto inputs = _collectVertexShaderInputs();
    // Run the vertex shader for each input
    auto vertexShaderOutputs = _runVertexShader(inputs);
    
    // Convert vertex shader outputs to view space triangles
    auto viewTriangles = _toViewTriangles(vertexShaderOutputs);

    srGLboolean cullFaceEnabled;
    _configurationService->getBooleanv(srGL_CULL_FACE, &cullFaceEnabled);

    return _rasterize(cullFaceEnabled ? _cullFaceTest(viewTriangles) : viewTriangles);
}

std::map<Point<srGLsizei>, Vector4d<srGLfloat>> DrawTriangleArraysStrategy::processFragments(const std::vector<Fragment>& fragments)
{
    std::map<Point<srGLsizei>, Vector4d<srGLfloat>> fragmentOutputs;

    for (const auto &fragment : fragments)
    {
        auto fragmentShaderOutput = _runFragmentShader(fragment.fragmentShaderInputs);

        auto fragmentColorIt = std::find_if(
            fragmentShaderOutput.begin(),
            fragmentShaderOutput.end(),
            [](const auto &outputValue)
            {
                return outputValue.name == "FragColor";
            });
        assert(fragmentColorIt != fragmentShaderOutput.end() && "Fragment shader output 'FragColor' not found");

        fragmentOutputs[fragment.position] = fragmentColorIt->value.floatVector4dValue;
    }

    return fragmentOutputs;
}

std::vector<std::vector<InputValue>> DrawTriangleArraysStrategy::_collectVertexShaderInputs()
{
    return _collectArrayVertexShaderInputs();
}

std::vector<Fragment> DrawTriangleArraysStrategy::_rasterize(const std::vector<ViewTriangle<srGLsizei>> &viewTriangles)
{
    std::vector<Fragment> rasterizedInputs;

    for (const auto &viewTriangle : viewTriangles)
    {
        auto triangleFragments = _rasterizeTriangle(viewTriangle);
        rasterizedInputs.insert(rasterizedInputs.end(), triangleFragments.begin(), triangleFragments.end());
    }

    return rasterizedInputs;
}

std::vector<ViewTriangle<srGLsizei>> DrawTriangleArraysStrategy::_toViewTriangles(const std::vector<std::vector<OutputValue>> &vertexShaderOutputs)
{
    std::vector<ViewTriangle<srGLsizei>> triangles;

    assert(vertexShaderOutputs.size() % 3 == 0 && "Vertex shader outputs should be a multiple of 3 for triangles");
    
    // Every three vertex shader outputs form a triangle
    for (size_t i = 0; i + 2 < vertexShaderOutputs.size(); i += 3)
    {
        std::array<std::vector<OutputValue>, 3> triangleVertexOutputs = {
            vertexShaderOutputs[i],
            vertexShaderOutputs[i + 1],
            vertexShaderOutputs[i + 2]};

        auto clippedTriangles = _clippingService->clip(triangleVertexOutputs, kNearPlaneWEpsilon);
        for (const auto &clippedTriangle : clippedTriangles)
        {
            std::array<Point<srGLsizei>, 3> triangleVertices;
            std::array<std::vector<OutputValue>, 3> triangleCustomOutputs;
            std::array<float, 3> inverseClipW;
            std::array<float, 3> depths;

            for (size_t v = 0; v < 3; ++v)
            {
                const auto &vertexOutputs = clippedTriangle[v];

                auto glPosition = std::find_if(vertexOutputs.begin(), vertexOutputs.end(), [](const auto &outputValue)
                                                { return outputValue.name == GL_POSITION_NAME; });
                assert(glPosition != vertexOutputs.end());

                auto clipW = glPosition->value.floatVector4dValue.w;
                auto ndcX = glPosition->value.floatVector4dValue.x / clipW;
                auto ndcY = glPosition->value.floatVector4dValue.y / clipW;
                auto ndcZ = glPosition->value.floatVector4dValue.z / clipW;

                triangleVertices[v] = Point<srGLsizei>(
                    std::lerp(_viewport.x, _viewport.x + _viewport.width, (ndcX + 1.0f) * 0.5f),
                    std::lerp(_viewport.y, _viewport.y + _viewport.height, (ndcY + 1.0f) * 0.5f));
                depths[v] = (ndcZ + 1.0f) * 0.5f;
                inverseClipW[v] = 1.0f / clipW;

                // All custom interpolated values (for example colors, UV coordinates etc.)
                // should be passed further for fragment shader
                std::vector<OutputValue> customOutputs;
                for (const auto &outputValue : vertexOutputs)
                {
                    if (outputValue.name != GL_POSITION_NAME && outputValue.name != "")
                        customOutputs.push_back(outputValue);
                }
                triangleCustomOutputs[v] = std::move(customOutputs);
            }

            triangles.push_back(ViewTriangle<srGLsizei>(triangleVertices, triangleCustomOutputs, inverseClipW, depths));
        }
    }

    return triangles;
}

std::vector<Fragment> DrawTriangleArraysStrategy::_rasterizeTriangle(const ViewTriangle<srGLsizei> &vertexShaderTriangle)
{
    const auto &v0 = vertexShaderTriangle.points[0];
    const auto &v1 = vertexShaderTriangle.points[1];
    const auto &v2 = vertexShaderTriangle.points[2];

    // Create bounding box
    srGLsizei minX = std::max(std::min({v0.x, v1.x, v2.x}), _viewport.x);
    srGLsizei maxX = std::min(std::max({v0.x, v1.x, v2.x}), _viewport.x + _viewport.width - 1);
    srGLsizei minY = std::max(std::min({v0.y, v1.y, v2.y}), _viewport.y);
    srGLsizei maxY = std::min(std::max({v0.y, v1.y, v2.y}), _viewport.y + _viewport.height - 1);

    std::vector<Fragment> insideTrianglePoints;

    // int64_t: near-plane-clipped vertices can land far outside the viewport in
    // screen space, and the products below would overflow int32_t in that case.
    int64_t denom = static_cast<int64_t>(v1.y - v2.y) * (v0.x - v2.x) + static_cast<int64_t>(v2.x - v1.x) * (v0.y - v2.y);
    if (denom == 0)
    {
        // Degenerate triangle, nothing to rasterize
        return {};
    }

    // Barycentric numerators (a * denom, b * denom) are linear functions of x and y,
    // so instead of recomputing them from scratch for every pixel we step them
    // incrementally: a fixed delta per column and a fixed delta per row.
    int64_t aStepX = v1.y - v2.y;
    int64_t aStepY = v2.x - v1.x;
    int64_t bStepX = v2.y - v0.y;
    int64_t bStepY = v0.x - v2.x;

    int64_t aRowStart = aStepX * (minX - v2.x) + aStepY * (minY - v2.y);
    int64_t bRowStart = bStepX * (minX - v2.x) + bStepY * (minY - v2.y);

    bool positiveDenom = denom > 0;

    for (srGLsizei y = minY; y <= maxY; ++y)
    {
        int64_t aNumerator = aRowStart;
        int64_t bNumerator = bRowStart;

        for (srGLsizei x = minX; x <= maxX; ++x)
        {
            int64_t cNumerator = denom - aNumerator - bNumerator;

            bool inside = positiveDenom
                              ? (aNumerator >= 0 && bNumerator >= 0 && cNumerator >= 0)
                              : (aNumerator <= 0 && bNumerator <= 0 && cNumerator <= 0);

            if (inside)
            {
                float w0 = static_cast<float>(aNumerator) / static_cast<float>(denom);
                float w1 = static_cast<float>(bNumerator) / static_cast<float>(denom);
                float w2 = 1.0f - w0 - w1;

                auto interpolatedDepth =
                    w0 * vertexShaderTriangle.depths[0] +
                    w1 * vertexShaderTriangle.depths[1] +
                    w2 * vertexShaderTriangle.depths[2];

                // Screen-space (w0, w1, w2) are affine, but custom outputs must be
                // interpolated perspective-correctly, otherwise steep/grazing
                // triangles (e.g. skybox faces viewed from up close) warp badly.
                float pw0 = w0 * vertexShaderTriangle.inverseClipW[0];
                float pw1 = w1 * vertexShaderTriangle.inverseClipW[1];
                float pw2 = w2 * vertexShaderTriangle.inverseClipW[2];
                float pwSum = pw0 + pw1 + pw2;
                pw0 /= pwSum;
                pw1 /= pwSum;
                pw2 /= pwSum;

                auto interpolatedOutputs = _interpolateOutputs(
                    vertexShaderTriangle.vertexShaderCustomOutputs,
                    pw0,
                    pw1,
                    pw2);

                insideTrianglePoints.push_back(
                    Fragment(
                        Point<srGLsizei>{x, y},
                        interpolatedDepth,
                        interpolatedOutputs));
            }

            aNumerator += aStepX;
            bNumerator += bStepX;
        }

        aRowStart += aStepY;
        bRowStart += bStepY;
    }

    return insideTrianglePoints;
}

std::vector<InputValue> DrawTriangleArraysStrategy::_interpolateOutputs(
    const std::array<std::vector<OutputValue>, 3> &vertexShaderCustomOutputs,
    float w0, float w1, float w2)
{
    const auto &outputs0 = vertexShaderCustomOutputs[0];
    const auto &outputs1 = vertexShaderCustomOutputs[1];
    const auto &outputs2 = vertexShaderCustomOutputs[2];

    std::vector<InputValue> interpolatedOutputs;
    interpolatedOutputs.reserve(outputs0.size());

    for (size_t i = 0; i < outputs0.size(); ++i)
    {
        const auto &v0 = outputs0[i];
        const auto &v1 = outputs1[i];
        const auto &v2 = outputs2[i];

        Value value;
        switch (v0.type)
        {
        case srGL_FLOAT:
            value.floatValue =
                w0 * v0.value.floatValue +
                w1 * v1.value.floatValue +
                w2 * v2.value.floatValue;
            break;
        case srGL_FLOAT_VEC2:
            value.floatVector2dValue = {
                w0 * v0.value.floatVector2dValue.x + w1 * v1.value.floatVector2dValue.x + w2 * v2.value.floatVector2dValue.x,
                w0 * v0.value.floatVector2dValue.y + w1 * v1.value.floatVector2dValue.y + w2 * v2.value.floatVector2dValue.y};
            break;
        case srGL_FLOAT_VEC3:
            value.floatVector3dValue = {
                w0 * v0.value.floatVector3dValue.x + w1 * v1.value.floatVector3dValue.x + w2 * v2.value.floatVector3dValue.x,
                w0 * v0.value.floatVector3dValue.y + w1 * v1.value.floatVector3dValue.y + w2 * v2.value.floatVector3dValue.y,
                w0 * v0.value.floatVector3dValue.z + w1 * v1.value.floatVector3dValue.z + w2 * v2.value.floatVector3dValue.z};
            break;
        case srGL_FLOAT_VEC4:
            value.floatVector4dValue = {
                w0 * v0.value.floatVector4dValue.x + w1 * v1.value.floatVector4dValue.x + w2 * v2.value.floatVector4dValue.x,
                w0 * v0.value.floatVector4dValue.y + w1 * v1.value.floatVector4dValue.y + w2 * v2.value.floatVector4dValue.y,
                w0 * v0.value.floatVector4dValue.z + w1 * v1.value.floatVector4dValue.z + w2 * v2.value.floatVector4dValue.z,
                w0 * v0.value.floatVector4dValue.w + w1 * v1.value.floatVector4dValue.w + w2 * v2.value.floatVector4dValue.w};
            break;
        default:
            assert(false && "Unsupported vertex shader output type for interpolation");
            break;
        }

        interpolatedOutputs.push_back(InputValue(v0.name, v0.type, value));
    }

    return interpolatedOutputs;
}

std::vector<ViewTriangle<srGLsizei>> DrawTriangleArraysStrategy::_cullFaceTest(const std::vector<ViewTriangle<srGLsizei>> &triangles)
{
    srGLenum cullFaceMode;
    _configurationService->getIntegerv(srGL_CULL_FACE, reinterpret_cast<srGLint *>(&cullFaceMode));
    assert(cullFaceMode == srGL_BACK && "Only back face culling is supported");

    std::vector<ViewTriangle<srGLsizei>> visibleTriangles;
    for (const auto &triangle : triangles)
    {
        auto p1 = triangle.points[0];
        auto p2 = triangle.points[1];
        auto p3 = triangle.points[2];

        // Compute the signed area (2D cross product) of two edges to get winding order
        auto edge1 = p2 - p1;
        auto edge2 = p3 - p1;
        auto signedArea = edge1.x * edge2.y - edge1.y * edge2.x;

        if (signedArea >= 0)
        {
            visibleTriangles.push_back(triangle);
        }
    }
    return visibleTriangles;
}
