#include "BufferService.h"

#include <cassert>
#include <cstring>

using namespace glsr::Services::Buffer;

BufferService::BufferService()
    : _nextVboId(0), _nextVaoId(0)
{
}

void BufferService::genBuffers(srGLsizei n, srGLuint *buffers)
{
    for (srGLsizei i = 0; i < n; ++i)
    {
        buffers[i] = ++_nextVboId;

        VboModel vbo;
        vbo.data.clear();
        vbo.size = 0;
        vbo.usage = NULL;
        _buffers[_nextVboId] = vbo;
    }
}

void BufferService::deleteBuffers(srGLsizei n, const srGLuint *buffers)
{
    for (srGLsizei i = 0; i < n; ++i)
    {
        _buffers.erase(buffers[i]);
    }
}

void BufferService::bindBuffer(srGLenum target, srGLuint buffer)
{
    _boundBuffers[target] = buffer;

    // GL_ELEMENT_ARRAY_BUFFER is a special case,
    // as it is associated with the currently bound VAO.
    if (target == srGL_ELEMENT_ARRAY_BUFFER)
    {
        auto vaoIt = _arrays.find(_boundVao);
        if (vaoIt != _arrays.end())
        {
            vaoIt->second.elementsBufferId = buffer;
        }
    }
}

void BufferService::bufferData(srGLenum target, srGLsizeiptr size, const void *data, srGLenum usage)
{
    auto it = _boundBuffers.find(target);
    assert(it != _boundBuffers.end() && "No buffer bound to the specified target!");

    srGLuint bufferId = it->second;

    auto vboIt = _buffers.find(bufferId);
    assert(vboIt != _buffers.end() && "Buffer ID not found!");

    auto &vbo = vboIt->second;
    vbo.size = size;
    vbo.data.resize(size);
    if (data){
        std::memcpy(vbo.data.data(), data, size);
    } else {
        std::memset(vbo.data.data(), 0, size);
    }
    vbo.usage = usage;
}

void BufferService::bufferSubData(srGLenum target, srGLintptr offset, srGLsizeiptr size, const void *data)
{
    auto it = _boundBuffers.find(target);
    assert(it != _boundBuffers.end() && "No buffer bound to the specified target!");

    srGLuint bufferId = it->second;

    auto vboIt = _buffers.find(bufferId);
    assert(vboIt != _buffers.end() && "Buffer ID not found!");

    auto &vbo = vboIt->second;
    assert(offset + size <= vbo.size && "Sub data range exceeds buffer size!");
    std::memcpy(vbo.data.data() + offset, data, size);
}

const VboModel& BufferService::getBufferedData(srGLuint buffer) const
{
    auto vboIt = _buffers.find(buffer);
    assert(vboIt != _buffers.end() && "Buffer ID not found!");

    return vboIt->second;
}

void BufferService::genVertexArrays(srGLsizei n, srGLuint *arrays)
{
    for (srGLsizei i = 0; i < n; ++i)
    {
        arrays[i] = ++_nextVaoId;
        _arrays[_nextVaoId] = VaoModel();
    }
}

void BufferService::bindVertexArray(srGLuint array)
{
    _boundVao = array;
}

void BufferService::vertexAttribPointer(srGLuint index, srGLint size, srGLenum type, srGLboolean normalized, srGLsizei stride, const void *pointer)
{
    auto vaoIt = _arrays.find(_boundVao);
    assert(vaoIt != _arrays.end() && "No vertex array bound!");

    auto &vaoModel = vaoIt->second;
    auto attribIt = vaoModel.attribPointers.find(index);

    VaoArray vaoArray;
    vaoArray.size = size;
    vaoArray.type = type;
    vaoArray.normalized = normalized;
    vaoArray.stride = stride;
    vaoArray.pointer = pointer;
    vaoArray.isEnabled = attribIt != vaoModel.attribPointers.end() ? attribIt->second.isEnabled : srGL_FALSE;

    auto vboId = _boundBuffers[srGL_ARRAY_BUFFER];
    assert(_buffers.find(vboId) != _buffers.end() && "No buffer bound to GL_ARRAY_BUFFER!");
    vaoArray.bufferId = vboId;

    vaoIt->second.attribPointers[index] = vaoArray;
}

void BufferService::enableVertexAttribArray(srGLuint index)
{
    auto vaoIt = _arrays.find(_boundVao);
    assert(vaoIt != _arrays.end() && "No vertex array bound!");

    auto &vaoModel = vaoIt->second;
    auto attribIt = vaoModel.attribPointers.find(index);
    if (attribIt == vaoModel.attribPointers.end())
    {
        VaoArray vaoArray;
        vaoArray.isEnabled = srGL_TRUE;
        vaoModel.attribPointers[index] = vaoArray;
    }
    else
    {
        attribIt->second.isEnabled = srGL_TRUE;
    }
}

void BufferService::deleteVertexArrays(srGLsizei n, const srGLuint *arrays)
{
    for (srGLsizei i = 0; i < n; ++i)
    {
        _arrays.erase(arrays[i]);
    }
}

srGLuint BufferService::getBoundVao() const
{
    return _boundVao;
}

const VaoModel& BufferService::getVao(srGLuint target) const
{
    auto vaoIt = _arrays.find(target);
    assert(vaoIt != _arrays.end() && "Vertex array not found!");

    return vaoIt->second;
}