#pragma once
#include <string>

using std::string;

enum class CodeTokenType {

};

struct CodeToken {
    CodeTokenType tokenType;
    string value;
    int row;
    int column;
    
};