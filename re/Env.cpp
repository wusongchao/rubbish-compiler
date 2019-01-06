#include "Env.h"

Env::Env(unique_ptr<Env> prev)
    :prev(std::move(prev))
{
}