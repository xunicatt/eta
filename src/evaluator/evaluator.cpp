#include <ast.hpp>
#include <evaluator.hpp>
#include <lexer.hpp>
#include <memory>
#include <object.hpp>
#include <print>

#define LAMBDA_BUILTIN_FN(fn)                                                  \
  [this](const std::list<std::shared_ptr<object::Object>> &args) {             \
    return fn(args);                                                           \
  }

using namespace ast;
using namespace object;
using namespace evaluator;
using namespace ast;

Eval::Eval(lexer::Lexer &l) : lexer(l) {
  register_builtin_fn("len", LAMBDA_BUILTIN_FN(this->builtin_fn_len));
  register_builtin_fn("int", LAMBDA_BUILTIN_FN(this->builtin_fn_int));
  register_builtin_fn("float", LAMBDA_BUILTIN_FN(this->builtin_fn_float));
  register_builtin_fn("type", LAMBDA_BUILTIN_FN(this->builtin_fn_type));
  register_builtin_fn("print", LAMBDA_BUILTIN_FN(this->builtin_fn_print));
  register_builtin_fn("println", LAMBDA_BUILTIN_FN(this->builtin_fn_println));
  register_builtin_fn("any", LAMBDA_BUILTIN_FN(this->builtin_fn_any));
  register_builtin_fn("push", LAMBDA_BUILTIN_FN(this->builtin_fn_push));
  register_builtin_fn("pop", LAMBDA_BUILTIN_FN(this->builtin_fn_pop));
  register_builtin_fn("slice", LAMBDA_BUILTIN_FN(this->builtin_fn_slice));
}

auto Eval::eval(std::shared_ptr<Node> node, std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
#if __ETA_DEBUG_MODE__
  std::println("eval: {}", node->debug());
#endif

  switch (node->type()) {
  case ASTType::PROGRAM: {
    auto prgm = ast::cast<Node, Program>(std::move(node));
    return program(std::move(prgm), env);
  }

  case ASTType::EXPRESSION: {
    auto expr = ast::cast<Node, ExpressionStatement>(std::move(node));
    return eval(expr->expression, env);
  }

  case ASTType::INFIX: {
    auto expr = ast::cast<Node, InfixExpression>(std::move(node));

    auto right_pos = expr->right->position();
    auto right = eval(expr->right, env);
    if (auto err = error(right_pos, right); is_error(err)) {
      return err;
    }

    auto left_pos = expr->left->position();
    auto left = eval(expr->left, env);
    if (auto err = error(left_pos, left); is_error(err)) {
      return err;
    }

    auto res = infix(expr->op, std::move(left), std::move(right));
    if (auto err = error(expr->position(), res); is_error(err)) {
      return err;
    }

    return res;
  }

  case ASTType::PREFIX: {
    auto expr = ast::cast<Node, PrefixExpression>(std::move(node));

    auto right_pos = expr->right->position();
    auto right = eval(expr->right, env);
    if (auto err = error(right_pos, right); is_error(err)) {
      return err;
    }

    auto res = prefix(expr->op, std::move(right));
    if (auto err = error(expr->position(), res); is_error(err)) {
      return err;
    }

    return res;
  }

  case ASTType::LET: {
    auto stmt = ast::cast<Node, LetStatement>(std::move(node));
    return declare(std::move(stmt), env);
  }

  case ASTType::ASSIGNMENT: {
    auto expr = ast::cast<Node, AssignmentExpression>(std::move(node));
    return assignment(std::move(expr), env);
  }

  case ASTType::IDENTIFIER: {
    auto expr = ast::cast<Node, Identifier>(std::move(node));
    return identifier(std::move(expr), env);
  }

  case ASTType::INDEX: {
    auto expr = ast::cast<Node, IndexExpression>(std::move(node));

    auto left_pos = expr->left->position();
    auto left = eval(expr->left, env);
    if (auto err = error(left_pos, left); is_error(err)) {
      return err;
    }

    auto idx_pos = expr->index->position();
    auto idx = eval(expr->index, env);
    if (auto err = error(idx_pos, idx); is_error(err)) {
      return err;
    }

    auto res = index(std::move(left), std::move(idx));
    if (auto err = error(expr->position(), res); is_error(err)) {
      return err;
    }

    return res;
  }
  case ASTType::OPASSIGNMENT: {
    auto expr = ast::cast<Node, OpAssignment>(std::move(node));

    auto new_expr = std::make_shared<AssignmentExpression>();
    new_expr->name = expr->name;
    new_expr->pos = expr->position();
    new_expr->value = expr->value;

    auto res = assignment_operator(std::move(new_expr), expr->op, env);
    if (auto err = error(expr->position(), res); is_error(res)) {
      return err;
    }

    return res;
  }

  case ASTType::FUNCTION: {
    auto expr = ast::cast<Node, FunctionLiteral>(std::move(node));

    auto res = std::make_shared<Function>();
    res->parameters = expr->parameters;
    res->env = env;
    res->body = expr->body;

    return res;
  }

  case ASTType::CALL: {
    auto expr = ast::cast<Node, CallExpression>(std::move(node));

    auto fn_pos = expr->function->position();
    auto fn = eval(expr->function, env);
    if (auto err = error(fn_pos, fn); is_error(fn)) {
      return err;
    }

    auto args = expressions(expr->arguments, env);
    if (args.size() >= 1 && is_error(args[0])) {
      return error(expr->position(), args[0]);
    }

    auto res = function(fn, args);
    if (auto err = error(expr->position(), res); is_error(err)) {
      return err;
    }

    return res;
  }

  case ASTType::BLOCK: {
    auto blk = ast::cast<Node, BlockStatement>(std::move(node));
    return block(std::move(blk), env);
  }

  case ASTType::IF: {
    auto new_env = std::make_shared<Environment>(env);
    auto expr = ast::cast<Node, IfExpression>(std::move(node));
    return branch(std::move(expr), new_env);
  }

  case ASTType::FOR: {
    auto new_env = std::make_shared<Environment>(env);
    auto expr = ast::cast<Node, ForExpression>(std::move(node));
    return loop(std::move(expr), new_env);
  }

  case ASTType::RETURN: {
    auto stmt = ast::cast<Node, ReturnStatement>(std::move(node));

    std::shared_ptr<Object> val;
    if (stmt->value) {
      auto val_pos = stmt->value->position();
      val = eval(stmt->value, env);
      if (auto err = error(val_pos, val); is_error(err)) {
        return err;
      }
    } else {
      val = OBJECT_NULL;
    }

    auto res = std::make_shared<ReturnValue>();
    res->value = std::move(val);
    return res;
  }

  case ASTType::INTEGER: {
    return std::make_shared<Integer>(
        ast::cast<Node, IntegerLiteral>(std::move(node))->value);
  }

  case ASTType::FLOAT: {
    return std::make_shared<Float>(
        ast::cast<Node, FloatLiteral>(std::move(node))->value);
  }

  case ASTType::BOOL: {
    return (ast::cast<Node, BoolLiteral>(std::move(node))->value)
               ? OBJECT_TRUE
               : OBJECT_FALSE;
  }

  case ASTType::STRING: {
    return std::make_shared<String>(
        ast::cast<Node, StringLiteral>(std::move(node))->value);
  }

  case ASTType::ARRAY: {
    auto expr = ast::cast<Node, ArrayLiteral>(std::move(node));

    auto elements = expressions(expr->elements, env);
    if (elements.size() >= 1 && is_error(elements[0])) {
      return error(expr->position(), elements[0]);
    }

    auto res = std::make_shared<Array>();
    res->elements = std::move(elements);
    return res;
  }

  default:
    return OBJECT_NULL;
  }
}

auto Eval::program(this Eval &self, std::shared_ptr<Program> node,
                   std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  auto result = OBJECT_NULL;

  for (auto &stmt : node->statements) {
    result = self.eval(std::move(stmt), env);

    switch (result->type()) {
    case ObjectType::ORETURNVAL: {
      auto res = object::cast<Object, ReturnValue>(result);
      return res->value;
    }

    case ObjectType::ODETAILEDERROR:
      return result;

    default:
      break;
    }
  }

  return result;
}
