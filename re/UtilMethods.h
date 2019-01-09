#pragma once
#include <unordered_set>
#include <string>
#include <algorithm>

using std::unordered_set;
using std::string;

template
<typename T>
void unionWith(unordered_set<T>& lhs, const unordered_set<T>& rhs)
{
    for (const auto& r : rhs) {
        lhs.insert(r);
    }
}

string toUpper(const string& str);

string toUpper(string&& str);