#include <ast.hpp>
#include <cstddef>
#include <evaluator.hpp>
#include <memory>
#include <object.hpp>
#include <print>
#include <token.hpp>

using namespace ast;
using namespace object;
using namespace evaluator;
using token::Token;

auto Eval::expressions(this Eval &self,

                       const std::vector<std::shared_ptr<Expression>> &nodes,
                       std::shared_ptr<Environment> &env)
    -> const std::vector<std::shared_ptr<Object>> {
  std::vector<std::shared_ptr<Object>> res;

  for (auto &expr : nodes) {
    auto obj = self.eval(expr, env);
    if (auto err = self.error(expr->position(), obj); is_error(err)) {
      return {std::move(err)};
    }

    res.push_back(obj);
  }

  return res;
}

auto Eval::infix(this Eval &self, token::Token op,
                 const std::shared_ptr<Object> left,
                 const std::shared_ptr<Object> right)
    -> const std::shared_ptr<Object> {
  if (left->type() == ObjectType::OINT && right->type() == ObjectType::OINT) {
    return self.infix_op_integer(op, std::move(left), std::move(right));
  }

  if (left->type() == ObjectType::OFLOAT &&
      right->type() == ObjectType::OFLOAT) {
    return self.infix_op_float(op, std::move(left), std::move(right));
  }

  if (left->type() == ObjectType::OSTRING &&
      right->type() == ObjectType::OSTRING) {
    return self.infix_op_string(op, std::move(left), std::move(right));
  }

  if (left->type() != right->type()) {
    return self.serror("type mismatch");
  }

  if (op == token::Token::TEQL) {
    return self.boolean(left == right);
  }

  if (op == token::Token::TNEQL) {
    return self.boolean(left != right);
  }

  return self.serror("unknown operator");
}

auto Eval::prefix(this Eval &self, token::Token op,
                  const std::shared_ptr<Object> right)
    -> const std::shared_ptr<Object> {
  switch (op) {
  case Token::TNOT:
    return self.op_not(std::move(right));

  case Token::TSUB:
    return self.op_sub(std::move(right));

  default:
    return self.serror("unknown operator");
  }
}

auto Eval::op_not(this Eval &self, const std::shared_ptr<Object> right)
    -> const std::shared_ptr<Object> {
  if (right->debug() == OBJECT_TRUE->debug()) {
    return OBJECT_FALSE;
  }

  if (right->debug() == OBJECT_FALSE->debug()) {
    return OBJECT_TRUE;
  }

  if (right->debug() == OBJECT_NULL->debug()) {
    return OBJECT_TRUE;
  }

  return OBJECT_FALSE;
}

auto Eval::op_sub(this Eval &self, const std::shared_ptr<Object> right)
    -> const std::shared_ptr<Object> {
  switch (right->type()) {
  case ObjectType::OINT: {
    auto res = std::make_shared<Integer>();
    res->value = -(object::cast<Object, Integer>(std::move(right))->value);
    return res;
  }

  case ObjectType::OFLOAT: {
    auto res = std::make_shared<Float>();
    res->value = -(object::cast<Object, Float>(std::move(right))->value);
    return res;
  }

  default:
    return self.serror("type is not supported");
  }
}

auto Eval::infix_op_integer(this Eval &self, token::Token op,
                            const std::shared_ptr<Object> left,
                            const std::shared_ptr<Object> right)
    -> const std::shared_ptr<Object> {
  auto lval = object::cast<Object, Integer>(std::move(left))->value;
  auto rval = object::cast<Object, Integer>(std::move(right))->value;
  auto res = std::make_shared<Integer>();

  switch (op) {
  case Token::TADD:
    res->value = lval + rval;
    break;

  case Token::TSUB:
    res->value = lval - rval;
    break;

  case Token::TMUL:
    res->value = lval * rval;
    break;

  case Token::TDIV:
    res->value = lval / rval;
    break;

  case Token::TGRT:
    return self.boolean(lval > rval);

  case Token::TGRE:
    return self.boolean(lval >= rval);

  case Token::TLES:
    return self.boolean(lval < rval);

  case Token::TLEE:
    return self.boolean(lval <= rval);

  case Token::TEQL:
    return self.boolean(lval == rval);

  case Token::TNEQL:
    return self.boolean(lval != rval);

  default:
    return self.serror("unknown operator");
  }

  return res;
}

auto Eval::infix_op_float(this Eval &self, token::Token op,
                          const std::shared_ptr<Object> left,
                          const std::shared_ptr<Object> right)
    -> const std::shared_ptr<Object> {
  auto lval = object::cast<Object, Float>(std::move(left))->value;
  auto rval = object::cast<Object, Float>(std::move(right))->value;
  auto res = std::make_shared<Float>();

  switch (op) {
  case Token::TADD:
    res->value = lval + rval;
    break;

  case Token::TSUB:
    res->value = lval - rval;
    break;

  case Token::TMUL:
    res->value = lval * rval;
    break;

  case Token::TDIV:
    res->value = lval / rval;
    break;

  case Token::TGRT:
    return self.boolean(lval > rval);

  case Token::TGRE:
    return self.boolean(lval >= rval);

  case Token::TLES:
    return self.boolean(lval < rval);

  case Token::TLEE:
    return self.boolean(lval <= rval);

  case Token::TEQL:
    return self.boolean(lval == rval);

  case Token::TNEQL:
    return self.boolean(lval != rval);

  default:
    return self.serror("unknown operator");
  }

  return res;
}

auto Eval::infix_op_string(this Eval &self, token::Token op,
                           const std::shared_ptr<Object> left,
                           const std::shared_ptr<Object> right)
    -> const std::shared_ptr<Object> {
  auto lval = object::cast<Object, String>(std::move(left))->value;
  auto rval = object::cast<Object, String>(std::move(right))->value;
  auto res = std::make_shared<String>();

  switch (op) {
  case Token::TADD:
    res->value = lval + rval;
    break;

  case Token::TGRT:
    return self.boolean(lval > rval);

  case Token::TGRE:
    return self.boolean(lval >= rval);

  case Token::TLES:
    return self.boolean(lval < rval);

  case Token::TLEE:
    return self.boolean(lval <= rval);

  case Token::TEQL:
    return self.boolean(lval == rval);

  case Token::TNEQL:
    return self.boolean(lval != rval);

  default:
    return self.serror("unknown operator");
  }

  return res;
}

auto Eval::index_array(this Eval &self, const std::shared_ptr<Object> arr,
                       const std::shared_ptr<Object> index)
    -> const std::shared_ptr<Object> {
  auto obj = object::cast<Object, Array>(std::move(arr));
  auto idx = object::cast<Object, Integer>(std::move(index))->value;

  if (idx < 0 || (size_t)idx >= obj->elements.size()) {
    return self.serror("index out of range");
  }

  return obj->elements[idx];
}

auto Eval::index_string(this Eval &self, const std::shared_ptr<Object> arr,
                        const std::shared_ptr<Object> index)
    -> const std::shared_ptr<Object> {
  auto obj = object::cast<Object, String>(std::move(arr));
  auto idx = object::cast<Object, Integer>(std::move(index))->value;

  if (idx < 0 || (size_t)idx >= obj->value.length()) {
    return self.serror("index out of range");
  }

  auto res = std::make_shared<String>();
  res->value = obj->value[idx];
  return res;
}

auto Eval::index(this Eval &self, const std::shared_ptr<Object> obj,
                 const std::shared_ptr<Object> index)
    -> const std::shared_ptr<Object> {
  if (index->type() != ObjectType::OINT) {
    return self.serror("expected an int type for index");
  }

  switch (obj->type()) {
  case ObjectType::OARRAY: {
    return self.index_array(std::move(obj), std::move(index));
  }

  case ObjectType::OSTRING: {
    return self.index_string(std::move(obj), std::move(index));
  }

  default:
    return self.serror("expected an array or string type");
  }
}

auto Eval::assignment_operator(this Eval &self,
                               std::shared_ptr<AssignmentExpression> node,
                               token::Token op,
                               std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  if (node->name->type() != ASTType::IDENTIFIER) {
    return self.derror(node->name->position(),
                       self.serror("expected a variable"));
  }

  auto name = ast::cast<Expression, Identifier>(std::move(node->name));
  auto obj = env->get(name->value);
  if (!obj.has_value()) {
    return self.derror(node->name->position(),
                       self.serror("undefined variable"));
  }

  auto val = self.eval(node->value, env);
  if (auto err = self.error(node->value->position(), val); is_error(err)) {
    return err;
  }

  auto res = self.infix(op, std::move(obj.value()), std::move(val));
  if (auto err = self.error(node->position(), res); is_error(err)) {
    return err;
  }

  return env->update(name->value, std::move(res));
}
