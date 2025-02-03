#include <memory>
#include <object.hpp>

using namespace object;

Environment::Environment() { this->outer = nullptr; }
Environment::Environment(std::shared_ptr<Environment> outer) {
  this->outer = outer;
}
auto Environment::exists(string name) -> bool { return data.contains(name); }

auto Environment::get(string name) -> std::optional<std::shared_ptr<Object>> {
  if (data.contains(name)) {
    return data.at(name);
  }

  if (outer == nullptr) {
    return {};
  }

  return outer->get(name);
}

auto Environment::set(string name, std::shared_ptr<Object> obj)
    -> std::shared_ptr<Object> {
  data[name] = obj;
  return obj;
}

auto Environment::update(string name, std::shared_ptr<Object> obj)
    -> std::shared_ptr<Object> {
  if (exists(name)) {
    return set(name, obj);
  }

  return outer->update(name, obj);
}
