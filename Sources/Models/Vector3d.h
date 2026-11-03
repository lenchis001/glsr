#ifndef GLSR_VECTOR3D_H
#define GLSR_VECTOR3D_H

#define GL_POSITION_NAME "gl_Position"

namespace glsr::Models
{
    template <class T>
    struct Vector3d
    {
        T x;
        T y;
        T z;
    };
}

#endif // GLSR_VECTOR3D_H