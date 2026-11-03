#ifndef GLSR_VECTOR4D_H
#define GLSR_VECTOR4D_H

namespace glsr::Models
{
    template <class T>
    struct Vector4d
    {
        Vector4d() : x(0), y(0), z(0), w(0) {}

        Vector4d(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        T x;
        T y;
        T z;
        T w;
    };
}

#endif // GLSR_VECTOR4D_H