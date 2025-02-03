#include <ast.hpp>
#include <ranges>

using namespace ast;

// ---------------------------------------
// INTEGER
auto IntegerLiteral::position() -> types::Position { return pos; }
auto IntegerLiteral::type() -> ASTType { return ASTType::INTEGER; }
auto IntegerLiteral::debug() -> string {
  return std::format("{{integer: {}}}", value);
}

// ---------------------------------------
// FLOAT
auto FloatLiteral::position() -> types::Position { return pos; }
auto FloatLiteral::type() -> ASTType { return ASTType::FLOAT; }
auto FloatLiteral::debug() -> string {
  return std::format("{{float: {}}}", value);
}

// ---------------------------------------
// BOOL
auto BoolLiteral::position() -> types::Position { return pos; }
auto BoolLiteral::type() -> ASTType { return ASTType::BOOL; }
auto BoolLiteral::debug() -> string {
  return std::format("{{bool: {}}}", value ? "true" : "false");
}

// ---------------------------------------
// STRING
auto StringLiteral::position() -> types::Position { return pos; }
auto StringLiteral::type() -> ASTType { return ASTType::STRING; }
auto StringLiteral::debug() -> string {
  return std::format("{{string: {}}}", value);
}

// ---------------------------------------
// Array
auto ArrayLiteral::position() -> types::Position { return pos; }
auto ArrayLiteral::type() -> ASTType { return ASTType::ARRAY; }
auto ArrayLiteral::debug() -> string {
  string res = "[";
  for (auto const &[i, e] : elements | std::views::enumerate) {
    res += e->debug();
    if (elements.size() - 1 != static_cast<size_t>(i)) {
      res += ", ";
    }
  }
  res += "]";
  return std::format("{{array: {}}}", res);
}
