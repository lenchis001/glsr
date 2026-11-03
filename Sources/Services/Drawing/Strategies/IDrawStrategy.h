#ifndef GLSR_IDRAW_STRATEGY_H
#define GLSR_IDRAW_STRATEGY_H

#include <vector>
#include <map>

#include "Services/Drawing/Models/Fragment.h"
#include "Services/Drawing/Models/Point.h"
#include "Models/Vector4d.h"

#include "srtypes.h"

namespace glsr::Services::Drawing::Strategies
{
    class IDrawStrategy
    {
    public:
        virtual ~IDrawStrategy() = default;

        virtual std::vector<Models::Fragment> processVertices() = 0;

        virtual std::map<Drawing::Models::Point<srGLsizei>, glsr::Models::Vector4d<srGLfloat>> 
            processFragments(const std::vector<Models::Fragment>& fragments) = 0;
    };
}

#endif // GLSR_IDRAW_STRATEGY_H