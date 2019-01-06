#pragma once
#include <unordered_map>
#include <memory>
#include "CodeToken.h"
#include "Expr.h"

using std::unordered_map;
using std::unique_ptr;
using std::shared_ptr;
using std::string;

class Env {
public:
    Env(unique_ptr<Env> prev);

    void putSymbol(const CodeToken& token, shared_ptr<Id> id) {
        table.insert({ token.value, id });
    }

    shared_ptr<Id> getSymbol(const CodeToken& token) {
        Env* cur = this;
        auto endIter = table.end();
        while (cur != nullptr) {
            auto it = table.find(token.value);
            if (it != endIter) {
                return it->second;
            }
            cur = cur->prev.get();
        }
        
        return nullptr;
    }

private:
    unique_ptr<Env> prev{ nullptr };

    unordered_map<string, shared_ptr<Id>> table;
};