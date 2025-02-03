#ifndef __ETA_EVALUATOR_HPP__
#define __ETA_EVALUATOR_HPP__

#include <ast.hpp>
#include <debug.hpp>
#include <expected>
#include <lexer.hpp>
#include <map>
#include <memory>
#include <object.hpp>
#include <token.hpp>
#include <types.hpp>
#include <vector>

using namespace object;
using namespace ast;

namespace evaluator {
const std::shared_ptr<Object> OBJECT_NULL = std::make_shared<Null>();
const std::shared_ptr<Object> OBJECT_TRUE = std::make_shared<Bool>(true);
const std::shared_ptr<Object> OBJECT_FALSE = std::make_shared<Bool>(false);

auto is_error(const std::shared_ptr<Object> err) -> bool;

class Eval {
public:
  Eval(lexer::Lexer &l);
  auto eval(std::shared_ptr<Node> node, std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

private:
  auto derror(this Eval &self, const types::Position &node_pos,
              const std::shared_ptr<SimpleError> err)
      -> const std::shared_ptr<DetailedError>;

  auto serror(string msg) -> const std::shared_ptr<SimpleError>;

  auto error(const types::Position &node_pos, const std::shared_ptr<Object> err)
      -> const std::shared_ptr<Object>;

  auto program(this Eval &self, std::shared_ptr<Program> node,
               std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

  auto identifier(this Eval &self, std::shared_ptr<Identifier> node,
                  std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

  auto declare(this Eval &self, std::shared_ptr<LetStatement> node,
               std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

  auto assignment_identifier(this Eval &self, std::shared_ptr<Identifier> name,
                             std::shared_ptr<Expression> value,
                             std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

  auto assignement_array(this Eval &self, const std::shared_ptr<Array> array,
                         std::shared_ptr<Expression> index,
                         std::shared_ptr<Expression> value,
                         std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

  auto assignement_string(this Eval &self, const std::shared_ptr<String> string,
                          std::shared_ptr<Expression> index,
                          std::shared_ptr<Expression> value,
                          std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

  auto assignment(this Eval &self, std::shared_ptr<AssignmentExpression> node,
                  std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

  auto block(this Eval &self, std::shared_ptr<BlockStatement> node,
             std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

  auto boolean(bool value) -> const std::shared_ptr<Object>;

  auto truthy(const std::shared_ptr<Object> obj) -> bool;

  auto branch(this Eval &self, std::shared_ptr<IfExpression> node,
              std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;
  auto loop(this Eval &self, std::shared_ptr<ForExpression> node,
            std::shared_ptr<Environment> &env) -> const std::shared_ptr<Object>;

  auto expressions(this Eval &self,
                   const std::vector<std::shared_ptr<Expression>> &nodes,
                   std::shared_ptr<Environment> &env)
      -> const std::vector<std::shared_ptr<Object>>;

  auto infix(this Eval &self, token::Token op,
             const std::shared_ptr<Object> left,
             const std::shared_ptr<Object> right)
      -> const std::shared_ptr<Object>;

  auto prefix(this Eval &self, token::Token op,
              const std::shared_ptr<Object> right)
      -> const std::shared_ptr<Object>;

  auto op_not(this Eval &self, const std::shared_ptr<Object> right)
      -> const std::shared_ptr<Object>;

  auto op_sub(this Eval &self, const std::shared_ptr<Object> right)
      -> const std::shared_ptr<Object>;

  auto infix_op_integer(this Eval &self, token::Token op,
                        const std::shared_ptr<Object> left,
                        const std::shared_ptr<Object> right)
      -> const std::shared_ptr<Object>;

  auto infix_op_float(this Eval &self, token::Token op,
                      const std::shared_ptr<Object> left,
                      const std::shared_ptr<Object> right)
      -> const std::shared_ptr<Object>;

  auto infix_op_string(this Eval &self, token::Token op,
                       const std::shared_ptr<Object> left,
                       const std::shared_ptr<Object> right)
      -> const std::shared_ptr<Object>;

  auto index_array(this Eval &self, const std::shared_ptr<Object> arr,
                   const std::shared_ptr<Object> index)
      -> const std::shared_ptr<Object>;

  auto index_string(this Eval &self, const std::shared_ptr<Object> str,
                    const std::shared_ptr<Object> index)
      -> const std::shared_ptr<Object>;

  auto index(this Eval &self, const std::shared_ptr<Object> obj,
             const std::shared_ptr<Object> index)
      -> const std::shared_ptr<Object>;
  auto assignment_operator(this Eval &self,
                           std::shared_ptr<AssignmentExpression> node,
                           token::Token op, std::shared_ptr<Environment> &env)
      -> const std::shared_ptr<Object>;

  auto extend_environment(this Eval &self, const std::shared_ptr<Function> fn,
                          const std::vector<std::shared_ptr<Object>> &args)
      -> std::shared_ptr<Environment>;

  auto function(this Eval &self, const std::shared_ptr<Object> fn,
                const std::vector<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto register_builtin_fn(string name, BuiltinFunction fn) -> void;

  auto builtin_fn_len(this Eval &self,
                      const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto builtin_fn_int(this Eval &self,
                      const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto builtin_fn_float(this Eval &self,
                        const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto builtin_fn_type(this Eval &self,
                       const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto builtin_fn_print(this Eval &self,
                        const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto builtin_fn_println(this Eval &self,
                          const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto builtin_fn_any(this Eval &self,
                      const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto builtin_fn_push(this Eval &self,
                       const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto builtin_fn_pop(this Eval &self,
                      const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  auto builtin_fn_slice(this Eval &self,
                        const std::list<std::shared_ptr<Object>> &args)
      -> const std::shared_ptr<Object>;

  lexer::Lexer &lexer;
  std::map<string, std::shared_ptr<Builtin>> builinfns;
};
}; // namespace evaluator

#endif
