#include <ast.hpp>
#include <cstddef>
#include <cstdio>
#include <format>
#include <ranges>
#include <token.hpp>
#include <types.hpp>

using namespace ast;

// ---------------------------------------
// PROGRAM
auto Program::position() -> types::Position { return types::Position{}; }
auto Program::type() -> ASTType { return ASTType::PROGRAM; }
auto Program::debug() -> string {
  string res = "[";
  for (auto const &[i, s] : statements | std::views::enumerate) {
    res += s->debug();
    if (statements.size() - 1 != static_cast<size_t>(i)) {
      res += ", ";
    }
  }
  res += "]";
  return std::format("{{program: {}}}", res);
}

// ---------------------------------------
// BLOCK STATEMENT
auto BlockStatement::position() -> types::Position { return pos; }
auto BlockStatement::type() -> ASTType { return ASTType::BLOCK; }
auto BlockStatement::debug() -> string {
  string res = "[";
  for (auto const &[i, s] : statements | std::views::enumerate) {
    res += s->debug();
    if (statements.size() - 1 != static_cast<size_t>(i)) {
      res += ", ";
    }
  }
  res += "]";
  return std::format("{{block: {}}}", res);
}

// ---------------------------------------
// IDENTIFIER
auto Identifier::position() -> types::Position { return pos; }
auto Identifier::type() -> ASTType { return ASTType::IDENTIFIER; }
auto Identifier::debug() -> string {
  return std::format("{{identifier: {}}}", value);
}

// ---------------------------------------
// FUNCTION
auto FunctionLiteral::position() -> types::Position { return pos; }
auto FunctionLiteral::type() -> ASTType { return ASTType::FUNCTION; }
auto FunctionLiteral::debug() -> string {
  string args = "[", sbody = "nil";

  for (auto const &[i, a] : parameters | std::views::enumerate) {
    args += a->debug();
    if (parameters.size() - 1 == static_cast<size_t>(i)) {
      args += ", ";
    }
  }
  args += "]";

  if (body) {
    sbody = body->debug();
  }

  return std::format("{{function: {{args: {}, body: {}}}}}", args, sbody);
}
