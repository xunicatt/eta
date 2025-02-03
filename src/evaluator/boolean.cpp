#include <evaluator.hpp>

using namespace ast;
using namespace object;
using namespace evaluator;

auto Eval::boolean(bool value) -> const std::shared_ptr<Object> {
  if (value)
    return OBJECT_TRUE;
  return OBJECT_FALSE;
}

auto Eval::truthy(const std::shared_ptr<Object> obj) -> bool {
  if (obj->debug() == OBJECT_NULL->debug())
    return false;

  if (obj->debug() == OBJECT_TRUE->debug())
    return true;

  if (obj->debug() == OBJECT_FALSE->debug())
    return false;

  return true;
}
