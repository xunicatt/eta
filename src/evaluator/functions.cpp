#include <evaluator.hpp>
#include <format>
#include <list>
#include <memory>
#include <object.hpp>
#include <print>
#include <ranges>

using namespace ast;
using namespace object;
using namespace evaluator;

auto Eval::extend_environment(this Eval &self,
                              const std::shared_ptr<Function> fn,
                              const std::vector<std::shared_ptr<Object>> &args)
    -> std::shared_ptr<Environment> {
  auto env = std::make_shared<Environment>(fn->env);
  for (auto const &[i, parm] : fn->parameters | std::views::enumerate) {
    env->set(parm->value, args[i]);
  }
  return env;
}

auto Eval::function(this Eval &self, const std::shared_ptr<Object> fn,
                    const std::vector<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  switch (fn->type()) {
  case ObjectType::OFUNCTION: {
    auto func = object::cast<Object, Function>(fn);
    if (func->parameters.size() != args.size()) {
      return self.serror(std::format("expected {} arguments but got {}",
                                     func->parameters.size(), args.size()));
    }

    auto func_env = self.extend_environment(func, args);
    auto res = self.eval(func->body, func_env);

    if (res->type() == ObjectType::ORETURNVAL) {
      return object::cast<Object, ReturnValue>(std::move(res))->value;
    }

    return res;
  }

  case ObjectType::OBUILTINFUNCTION: {
    auto func = object::cast<Object, Builtin>(fn);
    return func->fn(std::list(args.begin(), args.end()));
  }

  default:
    return self.serror("undefined or not a function");
  }
}
