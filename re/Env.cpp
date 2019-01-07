#include "Env.h"

Env::Env(shared_ptr<Env> prev)
    :prev(prev)
{
}

ConstEnv::ConstEnv(shared_ptr<ConstEnv> prev)
    :prev(prev)
{
}
