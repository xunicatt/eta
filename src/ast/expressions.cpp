#include <ast.hpp>
#include <cstddef>
#include <format>
#include <ranges>

using namespace ast;

// ---------------------------------------
// PREFIX EXPRESSION
auto PrefixExpression::position() -> types::Position { return pos; }
auto PrefixExpression::type() -> ASTType { return ASTType::PREFIX; }

auto PrefixExpression::debug() -> string {
  string sright = "nil";
  if (right) {
    sright = right->debug();
  }
  return std::format("{{operator: {}, right: {}}}", token::TokenName[op],
                     sright);
}

// ---------------------------------------
// INFIX EXPRESSION
auto InfixExpression::position() -> types::Position { return pos; }
auto InfixExpression::type() -> ASTType { return ASTType::INFIX; }
auto InfixExpression::debug() -> string {
  string sright = "nil", sleft = "nil";
  if (right) {
    sright = right->debug();
  }

  if (left) {
    sleft = left->debug();
  }

  return std::format("{{operator: {}, left: {}, right: {}}}",
                     token::TokenName[op], sleft, sright);
}

// ---------------------------------------
// INFIX EXPRESSION
auto IfExpression::position() -> types::Position { return pos; }
auto IfExpression::type() -> ASTType { return ASTType::IF; }
auto IfExpression::debug() -> string {
  string scond = "nil", scons = "nil", saltr = "nil";
  if (condition) {
    scond = condition->debug();
  }

  if (consequence) {
    scons = consequence->debug();
  }

  if (alternative) {
    saltr = alternative->debug();
  }

  return std::format("{{branch: {{condition: {}, if: {}, else: {}}}}}", scond,
                     scons, saltr);
}

// ---------------------------------------
// FOR EXPRESSION
auto ForExpression::position() -> types::Position { return pos; }
auto ForExpression::type() -> ASTType { return ASTType::FOR; }
auto ForExpression::debug() -> string {
  string sinit = "nil", scond = "nil", supdt = "nil", sbody = "nil";
  if (intialization) {
    sinit = intialization->debug();
  }

  if (condition) {
    scond = condition->debug();
  }

  if (updation) {
    supdt = updation->debug();
  }

  if (body) {
    sbody = body->debug();
  }

  return std::format(
      "{{loop: {{initialization: {}, condition: {}, updation: {}, body: {}}}}}",
      sinit, scond, supdt, sbody);
}

// ---------------------------------------
// ASSIGNMENT EXPRESSION
auto AssignmentExpression::position() -> types::Position { return pos; }
auto AssignmentExpression::type() -> ASTType { return ASTType::ASSIGNMENT; }
auto AssignmentExpression::debug() -> string {
  string sname = "nil", svalue = "nil";
  if (name) {
    sname = name->debug();
  }

  if (value) {
    svalue = value->debug();
  }

  return std::format("{{assignment: {{name: {}, value: {}}}}}", sname, svalue);
}

// ---------------------------------------
// FUNCTION CALL EXPRESSION
auto CallExpression::position() -> types::Position { return pos; }
auto CallExpression::type() -> ASTType { return ASTType::CALL; }
auto CallExpression::debug() -> string {
  string sfunction = "nil", sargs = "[";
  if (function) {
    sfunction = function->debug();
  }

  for (auto const &[i, a] : arguments | std::views::enumerate) {
    sargs += a->debug();
    if (arguments.size() - 1 != static_cast<size_t>(i)) {
      sargs += ", ";
    }
  }
  sargs += "]";

  return std::format("{{call: {{function: {}, args: {}}}}}", sfunction, sargs);
}

// ---------------------------------------
// SUBSCRIPT EXPRESSION
auto IndexExpression::position() -> types::Position { return pos; }
auto IndexExpression::type() -> ASTType { return ASTType::INDEX; }
auto IndexExpression::debug() -> string {
  string sleft = "nil", sindex = "nil";
  if (left) {
    sleft = left->debug();
  }

  if (index) {
    sindex = index->debug();
  }

  return std::format("{{subscript: {{left: {}, args: {}}}}}", sleft, sindex);
}

// ---------------------------------------
// OP ASS EXPRESSION
auto OpAssignment::position() -> types::Position { return pos; }
auto OpAssignment::type() -> ASTType { return ASTType::OPASSIGNMENT; }
auto OpAssignment::debug() -> string {
  string sname = "nil", svalue = "nil";
  if (name) {
    sname = name->debug();
  }

  if (value) {
    svalue = value->debug();
  }

  return std::format(
      "{{operator-assignment: {{operator: {}, name: {}, value: {}}}}}",
      token::TokenName[op], sname, svalue);
}
