#ifndef GLSR_POINT_MODEL
#define GLSR_POINT_MODEL

#include "srtypes.h"

namespace glsr::Services::Drawing::Models
{
    template <class T>
    struct Point
    {
        T x;
        T y;

        bool operator<(const Point<T> &other) const
        {
            return (x < other.x) || (x == other.x && y < other.y);
        }

        Point<T> operator-(const Point<T> &other) const
        {
            return Point<T>{x - other.x, y - other.y};
        }
    };
}

#endif // GLSR_POINT_MODEL