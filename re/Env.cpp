#include "Env.h"

Env::Env(shared_ptr<Env> prev)
    :prev(std::move(prev))
{
}

ConstEnv::ConstEnv(shared_ptr<ConstEnv> prev)
    :prev(std::move(prev))
{
};

FuncEnv::FuncEnv(shared_ptr<FuncEnv> prev)
	:prev(std::move(prev))
{
	table = unordered_map<string, shared_ptr<FuncDescripter>>();
};
