#include "primitive.h"

Primitive::Primitive()
{

}

const Material& Primitive::getMaterial(void) const
{
    return(this->material);
}

ObjectType Primitive::getType(void) const
{
    return(this->type);
}


