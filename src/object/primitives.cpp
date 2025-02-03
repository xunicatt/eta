#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <object.hpp>
#include <ranges>

using namespace object;

Integer::Integer() { value = 0; }
Integer::Integer(int64_t value) { this->value = value; }
auto Integer::type() const -> ObjectType { return ObjectType::OINT; }
auto Integer::debug() const -> string { return std::format("{}", value); }

Float::Float() { value = 0; }
Float::Float(double_t value) { this->value = value; }
auto Float::type() const -> ObjectType { return ObjectType::OFLOAT; }
auto Float::debug() const -> string { return std::format("{}", value); }

Bool::Bool() { value = false; }
Bool::Bool(bool value) { this->value = value; }
auto Bool::type() const -> ObjectType { return ObjectType::OBOOL; }
auto Bool::debug() const -> string { return std::format("{}", value); }

String::String() { value = ""; }
String::String(string value) { this->value = value; }
auto String::type() const -> ObjectType { return ObjectType::OSTRING; }
auto String::debug() const -> string { return std::format("{}", value); }

auto Array::type() const -> ObjectType { return ObjectType::OARRAY; }
auto Array::debug() const -> string {
  string res = "[";
  for (const auto &[i, e] : elements | std::views::enumerate) {
    if (e->type() == ObjectType::OSTRING) {
      res += '"' + e->debug() + '"';
    } else {
      res += e->debug();
    }

    if (elements.size() - 1 != (size_t)i) {
      res += ", ";
    }
  }
  res += ']';

  return res;
}
