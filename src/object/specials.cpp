#include <object.hpp>

using namespace object;

auto Null::type() const -> ObjectType { return ObjectType::ONULL; }
auto Null::debug() const -> string { return "null"; }

auto ReturnValue::type() const -> ObjectType { return ObjectType::ORETURNVAL; }
auto ReturnValue::debug() const -> string { return value->debug(); }

auto SimpleError::type() const -> ObjectType {
  return ObjectType::OSIMPLEERROR;
}
auto SimpleError::debug() const -> string { return value; }

auto DetailedError::type() const -> ObjectType {
  return ObjectType::ODETAILEDERROR;
}
auto DetailedError::debug() const -> string { return value; }

auto Function::type() const -> ObjectType { return ObjectType::OFUNCTION; }
auto Function::debug() const -> string { return "function"; }

auto Builtin::type() const -> ObjectType {
  return ObjectType::OBUILTINFUNCTION;
}
auto Builtin::debug() const -> string { return "builtin function"; }
