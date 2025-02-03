#include <cmath>
#include <cstddef>
#include <cstdint>
#include <evaluator.hpp>
#include <iterator>
#include <list>
#include <memory>
#include <object.hpp>

using namespace object;
using namespace evaluator;

auto Eval::register_builtin_fn(string name, BuiltinFunction fn) -> void {
  auto res = std::make_shared<Builtin>();
  res->fn = fn;
  this->builinfns[name] = std::move(res);
}

auto Eval::builtin_fn_len(this Eval &self,
                          const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  if (args.size() != 1) {
    return self.serror("len() only accepts one argument");
  }

  switch (args.front()->type()) {
  case ObjectType::OSTRING: {
    auto arg = object::cast<Object, String>(std::move(args.front()));
    auto res = std::make_shared<Integer>();
    res->value = arg->value.size();
    return res;
  }

  case ObjectType::OARRAY: {
    auto arg = object::cast<Object, Array>(std::move(args.front()));
    auto res = std::make_shared<Integer>();
    res->value = arg->elements.size();
    return res;
  }

  default:
    return self.serror("type is not supported");
  }
}

auto Eval::builtin_fn_int(this Eval &self,
                          const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  if (args.size() != 1) {
    return self.serror("int() only accepts one argument");
  }

  switch (args.front()->type()) {
  case ObjectType::OINT:
    return args.front();

  case ObjectType::OFLOAT: {
    auto arg = object::cast<Object, Float>(std::move(args.front()));
    auto res = std::make_shared<Integer>();
    res->value = static_cast<int64_t>(arg->value);
    return res;
  }

  case ObjectType::OBOOL: {
    auto arg = object::cast<Object, Bool>(std::move(args.front()));
    auto res = std::make_shared<Integer>();
    res->value = arg->value;
    return res;
  }

  default:
    return self.serror("type is not supported");
  }
}

auto Eval::builtin_fn_float(this Eval &self,
                            const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  if (args.size() != 1) {
    return self.serror("float() only accepts one argument");
  }

  switch (args.front()->type()) {
  case ObjectType::OFLOAT:
    return args.front();

  case ObjectType::OINT: {
    auto arg = object::cast<Object, Integer>(std::move(args.front()));
    auto res = std::make_shared<Float>();
    res->value = static_cast<double_t>(arg->value);
    return res;
  }

  default:
    return self.serror("type is not supported");
  }
}

auto Eval::builtin_fn_type(this Eval &self,
                           const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  if (args.size() != 1) {
    return self.serror("type() only accepts one argument");
  }

  auto res = std::make_shared<String>();
  res->value = OBJECT_TYPE_NAME.at(args.front()->type());
  return res;
}

auto Eval::builtin_fn_print(this Eval &self,
                            const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  auto str_replace_all = [](std::string &str, const std::string &from,
                            const std::string &to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
      str.replace(pos, from.length(), to);
      pos += to.length();
    }
  };

  for (const auto &arg : args) {
    auto res = arg->debug();
    if (arg->type() == ObjectType::OSTRING) {
      str_replace_all(res, "\\n", "\n");
    }
    std::print("{}", res);
  }
  auto res = std::make_shared<Integer>();
  res->value = args.size();
  return res;
}

auto Eval::builtin_fn_println(this Eval &self,
                              const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  auto res = self.builtin_fn_print(args);
  std::print("\n");
  return res;
}

auto Eval::builtin_fn_any(this Eval &self,
                          const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  if (args.size() != 0) {
    return self.serror("any() does not accept any arguments");
  }

  auto res = std::make_shared<Null>();
  return res;
}

auto Eval::builtin_fn_push(this Eval &self,
                           const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  if (args.size() != 2) {
    return self.serror("push() requires 2 arguments");
  }

  auto it = args.begin();
  auto arr = std::move(*it);
  if (arr->type() != ObjectType::OARRAY) {
    return self.serror("expected an array type");
  }

  std::advance(it, 1);
  auto val = std::move(*it);
  auto res = object::cast<Object, Array>(arr);
  res->elements.push_back(std::move(val));
  return res;
}

auto Eval::builtin_fn_pop(this Eval &self,
                          const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  if (args.size() != 1) {
    return self.serror("pop() requires 1 arguments");
  }

  auto arr = std::move(args.front());
  if (arr->type() != ObjectType::OARRAY) {
    return self.serror("expected an array type");
  }

  auto res = object::cast<Object, Array>(arr);
  res->elements.pop_back();
  return res;
}

auto Eval::builtin_fn_slice(this Eval &self,
                            const std::list<std::shared_ptr<Object>> &args)
    -> const std::shared_ptr<Object> {
  if (args.size() >= 1) {
    auto it = args.begin();
    auto arr = *it;
    if (arr->type() != ObjectType::OARRAY) {
      return self.serror("expected a array type");
    }
    auto arr_val = object::cast<Object, Array>(arr);

    switch (args.size()) {
    case 1: {
      auto res = std::make_shared<Array>();
      res->elements = arr_val->elements;
      return res;
    }

    case 3: {
      std::advance(it, 1);
      auto start = *it;
      if (start->type() != ObjectType::OINT) {
        return self.serror("expected start index to be an integer type");
      }

      std::advance(it, 1);
      auto end = *it;
      if (end->type() != ObjectType::OINT) {
        return self.serror("expected end index to be an integer type");
      }

      auto start_val = object::cast<Object, Integer>(std::move(start))->value;
      auto end_val = object::cast<Object, Integer>(std::move(end))->value;
      auto len_val = arr_val->elements.size();

      if (start_val < 0 || end_val < 0 ||
          static_cast<size_t>(start_val) > len_val ||
          static_cast<size_t>(end_val) > len_val) {
        return self.serror("index out of range");
      }

      if (start_val > end_val) {
        return self.serror("start index is greater than end index");
      }

      auto res = std::make_unique<Array>();
      res->elements.assign(arr_val->elements.begin() + start_val,
                           arr_val->elements.begin() + end_val);
      return res;
    }
    }
  }

  return self.serror("slice() requires either 1 or 3 argument");
}
