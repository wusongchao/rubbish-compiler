#pragma once

enum class TypeTag {
    Int, Bool
};

struct Type {
    TypeTag tag;
    int width = 0;

    Type(TypeTag typeTag, int width);

    const static Type Int;
    const static Type Bool;
};
