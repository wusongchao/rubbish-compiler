#pragma once
#include <unordered_set>

using std::unordered_set;

template
<typename T>
void unionWith(unordered_set<T>& lhs, const unordered_set<T>& rhs)
{
    for (const auto& r : rhs) {
        lhs.insert(r);
    }
}