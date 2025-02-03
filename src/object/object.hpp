#ifndef __ETA_OBJECT_HPP__
#define __ETA_OBJECT_HPP__

#include <ast.hpp>
#include <cmath>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using std::string;

namespace object {
enum ObjectType : uint8_t {
  ONULL = 0,
  OINT,
  OFLOAT,
  OBOOL,
  OSTRING,
  OARRAY,
  ORETURNVAL,
  OSIMPLEERROR,
  ODETAILEDERROR,
  OFUNCTION,
  OBUILTINFUNCTION,
};

const std::map<ObjectType, const string> OBJECT_TYPE_NAME = {
    {ONULL, "null"},         {OINT, "int"},
    {OFLOAT, "float"},       {OBOOL, "bool"},
    {OSTRING, "string"},     {OARRAY, "array"},
    {OFUNCTION, "function"}, {OBUILTINFUNCTION, "builtin function"},
};

struct Object {
  virtual auto type() const -> ObjectType = 0;
  virtual auto debug() const -> string = 0;
  virtual ~Object() = default;
};

template <typename X, typename Y>
std::shared_ptr<Y> cast(std::shared_ptr<X> old) {
  return std::shared_ptr<Y>(std::dynamic_pointer_cast<Y>(std::move(old)));
}

class Environment {
public:
  Environment();
  Environment(std::shared_ptr<Environment> outer);

  auto exists(string name) -> bool;
  auto get(string name) -> std::optional<std::shared_ptr<Object>>;
  auto set(string name, std::shared_ptr<Object> obj) -> std::shared_ptr<Object>;
  auto update(string name, std::shared_ptr<Object> obj)
      -> std::shared_ptr<Object>;

private:
  std::map<string, std::shared_ptr<Object>> data;
  std::shared_ptr<Environment> outer;
};

// ---------------------------------------
// INTEGER TYPE
struct Integer : Object {
  int64_t value;

  Integer();
  Integer(int64_t value);
  auto type() const -> ObjectType;
  auto debug() const -> string;
};

// ---------------------------------------
// FLOAT TYPE
struct Float : Object {
  double_t value;

  Float();
  Float(double_t value);
  auto type() const -> ObjectType;
  auto debug() const -> string;
};

// ---------------------------------------
// BOOL TYPE
struct Bool : Object {
  bool value;

  Bool();
  Bool(bool value);
  auto type() const -> ObjectType;
  auto debug() const -> string;
};

// ---------------------------------------
// STRING TYPE
struct String : Object {
  string value;

  String();
  String(string value);
  auto type() const -> ObjectType;
  auto debug() const -> string;
};

// ---------------------------------------
// Array TYPE
struct Array : Object {
  std::vector<std::shared_ptr<Object>> elements;

  auto type() const -> ObjectType;
  auto debug() const -> string;
};

// ---------------------------------------
// NULL TYPE
struct Null : Object {
  auto type() const -> ObjectType;
  auto debug() const -> string;
};

// ---------------------------------------
// RETURN VALUE TYPE
struct ReturnValue : Object {
  std::shared_ptr<Object> value;
  auto type() const -> ObjectType;
  auto debug() const -> string;
};

// ---------------------------------------
// ERROR TYPE
struct SimpleError : Object {
  string value;
  auto type() const -> ObjectType;
  auto debug() const -> string;
};

struct DetailedError : Object {
  string value;
  auto type() const -> ObjectType;
  auto debug() const -> string;
};

// ---------------------------------------
// FUNCTION TYPE
struct Function : Object {
  std::vector<std::shared_ptr<ast::Identifier>> parameters;
  std::shared_ptr<ast::BlockStatement> body;
  std::shared_ptr<Environment> env;

  auto type() const -> ObjectType;
  auto debug() const -> string;
};

// ---------------------------------------
// BUILIN FUNCTION TYPE
typedef std::function<auto(const std::list<std::shared_ptr<Object>> &args)
                          ->const std::shared_ptr<Object>>
    BuiltinFunction;

struct Builtin : Object {
  BuiltinFunction fn;

  auto type() const -> ObjectType;
  auto debug() const -> string;
};
}; // namespace object

#endif
