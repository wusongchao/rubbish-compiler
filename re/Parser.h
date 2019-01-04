#pragma once
#include <memory>

#include "Scanner.h"

using std::unique_ptr;
using std::shared_ptr;

class Parser {
public:
    void program();


private:

    unique_ptr<Scanner> scanner;
};