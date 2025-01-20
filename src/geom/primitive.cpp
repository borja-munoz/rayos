#include "primitive.h"

Primitive::Primitive()
{

}

Material Primitive::getMaterial(void)
{
    return(this->material);
}

ObjectType Primitive::getType(void)
{
    return(this->type);
}


