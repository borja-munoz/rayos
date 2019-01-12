#include "primitive.h"


//-----------------------------------------------

Primitive::Primitive()
{

}


//-----------------------------------------------

Primitive::~Primitive()
{	
    delete(this->material);
}


//-----------------------------------------------

Material * Primitive::getMaterial(void)
{
    return(new Material(*(this->material)));
}


//-----------------------------------------------

ObjectType Primitive::getType(void)
{
    return(this->type);
}


//-----------------------------------------------

