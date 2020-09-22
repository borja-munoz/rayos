#include "primitive.h"

Primitive::Primitive()
{

}

std::shared_ptr<Material> Primitive::getMaterial(void)
{
    return(this->material);
}

ObjectType Primitive::getType(void)
{
    return(this->type);
}


