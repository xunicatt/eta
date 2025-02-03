#include "ast.hpp"
#include "object.hpp"
#include <cstddef>
#include <evaluator.hpp>
#include <memory>
#include <print>

using namespace evaluator;
using namespace ast;
using namespace object;

auto Eval::identifier(this Eval &self, std::shared_ptr<Identifier> node,
                      std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  auto res = env->get(node->value);
  if (res.has_value()) {
    return res.value();
  }

  if (self.builinfns.contains(node->value)) {
    return self.builinfns.at(node->value);
  }

  return self.derror(node->position(), self.serror("undefined identifier"));
}

auto Eval::declare(this Eval &self, std::shared_ptr<LetStatement> node,
                   std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  if (env->exists(node->name->value)) {
    return self.derror(node->name->position(),
                       self.serror("redeclaration of same varibale"));
  }

  if (self.builinfns.contains(node->name->value)) {
    return self.derror(node->name->position(),
                       self.serror("a function with same name already exists"));
  }

  auto res = self.eval(node->value, env);
  if (is_error(res)) {
    return res;
  }

  return env->set(node->name->value, res);
}

auto Eval::assignment_identifier(this Eval &self,
                                 std::shared_ptr<Identifier> name,
                                 std::shared_ptr<Expression> value,
                                 std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  auto obj = env->get(name->value);
  if (!obj.has_value()) {
    return self.derror(name->position(), self.serror("undefined variable"));
  }

  if (obj.value()->type() == ObjectType::OFUNCTION) {
    return self.derror(
        name->position(),
        self.serror("a function type variable can not be reassigned"));
  }

  auto pos = value->position();
  auto res = self.eval(std::move(value), env);
  if (auto err = self.error(pos, res); is_error(err)) {
    return err;
  }

  if (obj.value()->type() != ObjectType::ONULL &&
      obj.value()->type() != res->type()) {
    return self.derror(
        name->pos,
        self.serror("a variable cannot be reassigned with a new type"));
  }

  return env->update(name->value, std::move(res));
}

auto Eval::assignement_array(this Eval &self,
                             const std::shared_ptr<Array> array,
                             std::shared_ptr<Expression> index,
                             std::shared_ptr<Expression> value,
                             std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  auto idx_pos = index->position();
  auto idx = self.eval(std::move(index), env);
  if (auto err = self.error(idx_pos, idx); is_error(err)) {
    return err;
  }

  if (idx->type() != ObjectType::OINT) {
    return self.derror(idx_pos, self.serror("expected an int type for index"));
  }

  auto i = object::cast<Object, Integer>(std::move(idx))->value;
  if (i < 0 || (size_t)i >= array->elements.size()) {
    return self.derror(idx_pos, self.serror("index out of range"));
  }

  auto val_pos = value->position();
  auto val = self.eval(std::move(value), env);
  if (auto err = self.error(val_pos, val); is_error(err)) {
    return err;
  }

  array->elements[i] = std::move(val);
  return array;
}

auto Eval::assignement_string(this Eval &self,
                              const std::shared_ptr<String> string,
                              std::shared_ptr<Expression> index,
                              std::shared_ptr<Expression> value,
                              std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  auto idx_pos = index->position();
  auto idx = self.eval(std::move(index), env);
  if (auto err = self.error(idx_pos, idx); is_error(err)) {
    return err;
  }

  if (idx->type() != ObjectType::OINT) {
    return self.derror(idx_pos, self.serror("expected an int type for index"));
  }

  auto i = object::cast<Object, Integer>(std::move(idx))->value;
  if (i < 0 || (size_t)i >= string->value.length()) {
    return self.derror(idx_pos, self.serror("index out of range"));
  }

  auto val_pos = value->position();
  auto val = self.eval(std::move(value), env);
  if (auto err = self.error(val_pos, val); is_error(err)) {
    return err;
  }

  if (val->type() != ObjectType::OSTRING) {
    return self.derror(val_pos, self.serror("expected a string type"));
  }

  // [TODO] checking for length on RHS string and ""
  string->value[i] = object::cast<Object, String>(val)->value[0];
  return string;
}

auto Eval::assignment(this Eval &self,
                      std::shared_ptr<AssignmentExpression> node,
                      std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  switch (node->name->type()) {
  case ASTType::IDENTIFIER:
    return self.assignment_identifier(
        ast::cast<Expression, Identifier>(node->name), node->value, env);

  case ASTType::INDEX: {
    auto expr = ast::cast<Expression, IndexExpression>(node->name);
    if (expr->left->type() != ASTType::IDENTIFIER) {
      return self.derror(expr->left->position(),
                         self.serror("expected an identifier"));
    }

    auto ident_pos = expr->left->position();
    auto ident = ast::cast<Expression, Identifier>(expr->left);
    auto obj = env->get(ident->value);
    if (!obj.has_value()) {
      return self.derror(ident_pos, self.serror("undefined identifier"));
    }

    switch (obj.value()->type()) {
    case ObjectType::OARRAY:
      return self.assignement_array(
          object::cast<Object, Array>(std::move(obj.value())), expr->index,
          node->value, env);

    case ObjectType::OSTRING: {
      auto res = self.assignement_string(
          object::cast<Object, String>(std::move(obj.value())), expr->index,
          node->value, env);
      return res;
    }

    default:
      return OBJECT_NULL;
    }
  }

  default:
    return OBJECT_NULL;
  }
}

auto Eval::block(this Eval &self, std::shared_ptr<BlockStatement> node,
                 std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  auto result = OBJECT_NULL;

  for (auto &stmt : node->statements) {
    if (result = self.eval(stmt, env);
        result && (result->type() == ObjectType::ORETURNVAL ||
                   result->type() == ObjectType::ODETAILEDERROR)) {
      return result;
    }
  }

  return result;
}
