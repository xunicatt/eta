#ifndef __ETA_AST_HPP__
#define __ETA_AST_HPP__

#include <cmath>
#include <cstdint>
#include <memory>
#include <print>
#include <string>
#include <token.hpp>
#include <types.hpp>
#include <vector>

using std::string;

namespace ast {
enum ASTType : uint8_t {
  PROGRAM = 0,
  BLOCK,
  IDENTIFIER,
  FUNCTION,
  INTEGER,
  FLOAT,
  BOOL,
  STRING,
  ARRAY,
  PREFIX,
  INFIX,
  IF,
  FOR,
  ASSIGNMENT,
  CALL,
  INDEX,
  OPASSIGNMENT,
  LET,
  RETURN,
  EXPRESSION,
};

template <typename X, typename Y>
std::shared_ptr<Y> cast(std::shared_ptr<X> old) {
  return std::dynamic_pointer_cast<Y>(std::move(old));
}

struct Node {
  virtual auto position() -> types::Position = 0;
  virtual auto type() -> ASTType = 0;
  virtual auto debug() -> string = 0;
  virtual ~Node() = default;
};

struct Statement : public Node {};
struct Expression : public Node {};

// ---------------------------------------
// PROGRAM
struct Program : public Node {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  std::vector<std::shared_ptr<Statement>> statements;
};

// ---------------------------------------
// BLOCK STATEMENT
struct BlockStatement : public Statement {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::vector<std::shared_ptr<Statement>> statements;
};

// ---------------------------------------
// IDENTIFIER
struct Identifier : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  string value;
};

// ---------------------------------------
// LET STATEMENT
struct LetStatement : public Statement {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::shared_ptr<Identifier> name;
  std::shared_ptr<Expression> value;
};

// ---------------------------------------
// FUNCTION
struct FunctionLiteral : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::vector<std::shared_ptr<Identifier>> parameters;
  std::shared_ptr<BlockStatement> body;
};

// ---------------------------------------
// INTEGER
struct IntegerLiteral : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  int64_t value;
};

// ---------------------------------------
// FLOATSTATEMENT
struct FloatLiteral : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  double_t value;
};

// ---------------------------------------
// BOOL
struct BoolLiteral : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  bool value;
};

// ---------------------------------------
// STRING
struct StringLiteral : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  string value;
};

// ---------------------------------------
// ARRAY
struct ArrayLiteral : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::vector<std::shared_ptr<Expression>> elements;
};

// ---------------------------------------
// PREFIX EXPRESSION
struct PrefixExpression : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  token::Token op;
  std::shared_ptr<Expression> right;
};

// ---------------------------------------
// INFIX EXPRESSION
struct InfixExpression : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  token::Token op;
  std::shared_ptr<Expression> right;
  std::shared_ptr<Expression> left;
};

// ---------------------------------------
// IF EXPRESSION
struct IfExpression : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::shared_ptr<Expression> condition;
  std::shared_ptr<BlockStatement> consequence;
  std::shared_ptr<BlockStatement> alternative;
};

// ---------------------------------------
// FOR EXPRESSION
struct ForExpression : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::shared_ptr<LetStatement> intialization;
  std::shared_ptr<Expression> condition;
  std::shared_ptr<Expression> updation;
  std::shared_ptr<BlockStatement> body;
};

// ---------------------------------------
// ASSIGNMENT EXPRESSION
struct AssignmentExpression : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::shared_ptr<Expression> name;
  std::shared_ptr<Expression> value;
};

// ---------------------------------------
// FUNCTION CALL EXPRESSION
struct CallExpression : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::shared_ptr<Expression> function;
  std::vector<std::shared_ptr<Expression>> arguments;
};

// ---------------------------------------
// SUBSCRIPT EXPRESSION
struct IndexExpression : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::shared_ptr<Expression> left;
  std::shared_ptr<Expression> index;
};

// ---------------------------------------
// OP ASS EXPRESSION
struct OpAssignment : public Expression {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  token::Token op;
  std::shared_ptr<Expression> name;
  std::shared_ptr<Expression> value;
};

// ---------------------------------------
// RETURN STATEMENT
struct ReturnStatement : public Statement {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::shared_ptr<Expression> value;
};

// ---------------------------------------
// EXPRESSION STATEMENT
struct ExpressionStatement : public Statement {
  auto position() -> types::Position;
  auto type() -> ASTType;
  auto debug() -> string;

  types::Position pos;
  std::shared_ptr<Expression> expression;
};
}; // namespace ast
#endif
