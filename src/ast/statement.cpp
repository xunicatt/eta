#include <ast.hpp>

using namespace ast;

// ---------------------------------------
// LET STATEMENT
auto LetStatement::position() -> types::Position { return pos; }
auto LetStatement::type() -> ASTType { return ASTType::LET; }
auto LetStatement::debug() -> string {
  string sname = "nil", svalue = "nil";
  if (name) {
    sname = name->debug();
  }

  if (value) {
    svalue = value->debug();
  }

  return std::format("{{let: {{name: {}, value: {}}}}}", sname, svalue);
}

// ---------------------------------------
// RETURN STATEMENT
auto ReturnStatement::position() -> types::Position { return pos; }
auto ReturnStatement::type() -> ASTType { return ASTType::RETURN; }
auto ReturnStatement::debug() -> string {
  string svalue = "nil";
  if (value) {
    svalue = value->debug();
  }

  return std::format("{{return: {{value: {}}}}}", svalue);
}

// ---------------------------------------
// EXPRESSION STATEMENT
auto ExpressionStatement::position() -> types::Position { return pos; }
auto ExpressionStatement::type() -> ASTType { return ASTType::EXPRESSION; }
auto ExpressionStatement::debug() -> string {
  if (expression) {
    return expression->debug();
  }

  return "nil";
}
