#pragma once
#include <unordered_map>
#include <memory>
#include "CodeToken.h"
#include "Expr.h"

using std::unordered_map;
using std::unique_ptr;
using std::shared_ptr;
using std::string;
using std::pair;

class Env {
public:
    Env(shared_ptr<Env> prev);

    void putSymbol(const CodeToken& token, shared_ptr<Id> id) {
        table.insert({ token.value, id });
    }

    shared_ptr<Id> getSymbol(const CodeToken& token) {
        Env* cur = this;
        while (cur != nullptr) {
			auto endIter = cur->table.end();
            auto it = cur->table.find(token.value);
            if (it != endIter) {
                return it->second;
            }
            cur = cur->prev.get();
        }
        
        return nullptr;
    }

    int getSymbolDepth(const string& str) {
        int depth = 0;
        Env* cur = this;
        while (cur != nullptr) {
            auto endIter = cur->table.end();
            auto it = cur->table.find(str);
            if (it != endIter) {
                return depth;
            }
            cur = cur->prev.get();
            ++depth;
        }

        return -1;
    }

private:
    shared_ptr<Env> prev{ nullptr };

    unordered_map<string, shared_ptr<Id>> table;
};

class ConstEnv{
public:
    ConstEnv(shared_ptr<ConstEnv> prev);

    void putSymbol(const CodeToken& token, shared_ptr<Constant> constant) {
        table.insert({ token.value, constant });
    }

    void putSymbol(const string& name, shared_ptr<Constant> constant) {
        table.insert({ name, constant });
    }

    shared_ptr<Constant> getSymbol(const CodeToken& token) {
        ConstEnv* cur = this;
        while (cur != nullptr) {
			auto endIter = cur->table.end();
            auto it = cur->table.find(token.value);
            if (it != endIter) {
                return it->second;
            }
            cur = cur->prev.get();
        }

        return nullptr;
    }
private:
    shared_ptr<ConstEnv> prev{ nullptr };

    unordered_map<string, shared_ptr<Constant>> table;
};

class FuncDescripter {
public:
	FuncDescripter(){
		id = nullptr;
		paramType = std::vector<Type>();
	};
	shared_ptr<Id> id;
	std::vector<Type> paramType;
};

class FuncEnv{
public:
	FuncEnv(shared_ptr<FuncEnv> prev);

	void putSymbol(const CodeToken& token, shared_ptr<FuncDescripter> funcscrip) {
		table.insert({ token.value, funcscrip });
	}
	void putSymbol(const string& name, shared_ptr<FuncDescripter> funcscrip) {
		table.insert({ name, funcscrip });
	}
	shared_ptr<FuncDescripter> getSymbol(const CodeToken& token) {
		auto cur = this;
		while (cur != nullptr) {
			auto endIter = cur->table.end();
			auto it = cur->table.find(token.value);
			if (it != endIter) {
				return it->second;
			}
			cur = cur->prev.get();
		}
	}

private:
	shared_ptr<FuncEnv> prev{ nullptr };

	unordered_map<string, shared_ptr<FuncDescripter>> table;
};