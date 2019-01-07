#include "Type.h"

Type::Type(TypeTag typeTag, int width):
    tag(typeTag), width(width)
{
}

const Type Type::Int{ TypeTag::Int, 4 };
const Type Type::Bool{ TypeTag::Bool, 1 };