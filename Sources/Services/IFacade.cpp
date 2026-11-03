#include "IFacade.h"

#include "Facade.h"

using namespace glsr::Services;

IFacade& IFacade::getInstance()
{
    static Facade instance;

    return instance;
}