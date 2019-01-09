#include "Env.h"

Env::Env(shared_ptr<Env> prev)
    :prev(prev)
{
}

ConstEnv::ConstEnv(shared_ptr<ConstEnv> prev)
    :prev(prev)
{
};

FuncEnv::FuncEnv(shared_ptr<FuncEnv> prev)
	:prev(prev)
{
	table = unordered_map<string, shared_ptr<FuncDescripter>>();
};
