#include "ast.hpp"
#include <evaluator.hpp>
#include <memory>
#include <print>

using namespace object;
using namespace ast;
using namespace evaluator;

auto Eval::branch(this Eval &self, std::shared_ptr<IfExpression> node,
                  std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  auto cond_pos = node->condition->position();
  auto cond = self.eval(node->condition, env);
  if (auto err = self.error(cond_pos, cond); is_error(err)) {
    return err;
  }

  if (self.truthy(std::move(cond))) {
    return self.eval(node->consequence, env);
  }

  if (node->alternative) {
    return self.eval(node->alternative, env);
  }

  return NULL;
}

auto Eval::loop(this Eval &self, std::shared_ptr<ForExpression> node,
                std::shared_ptr<Environment> &env)
    -> const std::shared_ptr<Object> {
  if (node->intialization) {
    auto init_pos = node->intialization->position();
    auto init = self.eval(node->intialization, env);
    if (auto err = self.error(init_pos, std::move(init)); is_error(err)) {
      return err;
    }
  }

  auto res = OBJECT_NULL;
  while (true) {
    if (node->condition) {
      auto cond_pos = node->condition->position();
      auto cond = self.eval(node->condition, env);
      if (auto err = self.error(cond_pos, cond); is_error(err)) {
        return err;
      }

      if (!self.truthy(std::move(cond))) {
        return res;
      }
    }

    auto body_pos = node->body->position();
    res = self.eval(node->body, env);
    if (auto err = self.error(body_pos, res); is_error(err)) {
      return err;
    }

    if (node->updation) {
      auto updt_pos = node->updation->position();
      auto updt = self.eval(node->updation, env);
      if (auto err = self.error(updt_pos, std::move(updt)); is_error(err)) {
        return err;
      }
    }
  }
}
