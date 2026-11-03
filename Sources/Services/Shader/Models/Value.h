#ifndef GLSR_SHADER_VALUE_H
#define GLSR_SHADER_VALUE_H

#include "srtypes.h"

#include "Models/Vector2d.h"
#include "Models/Vector3d.h"
#include "Models/Vector4d.h"

namespace glsr::Services::Shader::Models
{
    union Value
    {
        Value() : intValue(0) {}

        Value(srGLint v) : intValue(v) {}
        Value(srGLuint v) : uintValue(v) {}
        Value(srGLfloat v) : floatValue(v) {}
        Value(srGLdouble v) : doubleValue(v) {}

        Value(glsr::Models::Vector2d<srGLint> v) : intVector2dValue(v) {}
        Value(glsr::Models::Vector2d<srGLuint> v) : uintVector2dValue(v) {}
        Value(glsr::Models::Vector2d<srGLfloat> v) : floatVector2dValue(v) {}
        Value(glsr::Models::Vector2d<srGLdouble> v) : doubleVector2dValue(v) {}

        Value(glsr::Models::Vector3d<srGLint> v) : intVector3dValue(v) {}
        Value(glsr::Models::Vector3d<srGLuint> v) : uintVector3dValue(v) {}
        Value(glsr::Models::Vector3d<srGLfloat> v) : floatVector3dValue(v) {}
        Value(glsr::Models::Vector3d<srGLdouble> v) : doubleVector3dValue(v) {}

        Value(glsr::Models::Vector4d<srGLint> v) : intVector4dValue(v) {}
        Value(glsr::Models::Vector4d<srGLuint> v) : uintVector4dValue(v) {}
        Value(glsr::Models::Vector4d<srGLfloat> v) : floatVector4dValue(v) {}
        Value(glsr::Models::Vector4d<srGLdouble> v) : doubleVector4dValue(v) {}

        Value(srGLboolean v) : boolValue(v) {}

        srGLint intValue;
        srGLuint uintValue;
        srGLfloat floatValue;
        srGLdouble doubleValue;

        glsr::Models::Vector2d<srGLint> intVector2dValue;
        glsr::Models::Vector2d<srGLuint> uintVector2dValue;
        glsr::Models::Vector2d<srGLfloat> floatVector2dValue;
        glsr::Models::Vector2d<srGLdouble> doubleVector2dValue;

        glsr::Models::Vector3d<srGLint> intVector3dValue;
        glsr::Models::Vector3d<srGLuint> uintVector3dValue;
        glsr::Models::Vector3d<srGLfloat> floatVector3dValue;
        glsr::Models::Vector3d<srGLdouble> doubleVector3dValue;

        glsr::Models::Vector4d<srGLint> intVector4dValue;
        glsr::Models::Vector4d<srGLuint> uintVector4dValue;
        glsr::Models::Vector4d<srGLfloat> floatVector4dValue;
        glsr::Models::Vector4d<srGLdouble> doubleVector4dValue;

        srGLboolean boolValue;
    };
}

#endif // GLSR_SHADER_VALUE_H