#include "Env.h"

Env::Env(shared_ptr<Env> prev)
    :prev(prev)
{
}