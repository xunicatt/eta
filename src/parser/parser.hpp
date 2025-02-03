#ifndef __ETA_PARSER_HPP__
#define __ETA_PARSER_HPP__

#include <ast.hpp>
#include <debug.hpp>
#include <functional>
#include <lexer.hpp>
#include <map>
#include <memory>
#include <string>
#include <token.hpp>
#include <unistd.h>
#include <vector>

using std::string;

namespace parser {
typedef std::function<auto()->std::shared_ptr<ast::Expression>> prefix_parse_fn;
typedef std::function<
    auto(std::shared_ptr<ast::Expression>)->std::shared_ptr<ast::Expression>>
    infix_parse_fn;

enum Precedence {
  NONE = 0,
  LOWEST,
  ASSIGNMENT,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIX,
  CALL,
  INDEX,
};

const std::map<token::Token, Precedence> PRECEDENCES = {
    {token::Token::TASS, ASSIGNMENT},  {token::Token::TADAS, ASSIGNMENT},
    {token::Token::TSBAS, ASSIGNMENT}, {token::Token::TMLAS, ASSIGNMENT},
    {token::Token::TDVAS, ASSIGNMENT}, {token::Token::TEQL, EQUALS},
    {token::Token::TNEQL, EQUALS},     {token::Token::TLES, LESSGREATER},
    {token::Token::TLEE, LESSGREATER}, {token::Token::TGRT, LESSGREATER},
    {token::Token::TGRE, LESSGREATER}, {token::Token::TADD, SUM},
    {token::Token::TSUB, SUM},         {token::Token::TDIV, PRODUCT},
    {token::Token::TMUL, PRODUCT},     {token::Token::TOPAREN, CALL},
    {token::Token::TOSQR, INDEX},
};

class Parser {
public:
  Parser(lexer::Lexer &lexer);
  auto parse(this Parser &self) -> std::shared_ptr<ast::Program>;
  auto get_errors(this Parser const &self) -> const std::vector<string> &;

private:
  auto register_prefix_fn(this Parser &self, token::Token tk,
                          prefix_parse_fn fn) -> void;
  auto register_infix_fn(this Parser &self, token::Token tk, infix_parse_fn fn)
      -> void;

  auto register_error(this Parser &self, string msg) -> void;
  auto parse_identifier(this Parser &self) -> std::shared_ptr<ast::Expression>;
  auto parse_let(this Parser &self) -> std::shared_ptr<ast::LetStatement>;
  auto parse_assignment(this Parser &self,
                        std::shared_ptr<ast::Expression> name)
      -> std::shared_ptr<ast::Expression>;
  auto parse_block(this Parser &self) -> std::shared_ptr<ast::BlockStatement>;
  auto parse_statement(this Parser &self) -> std::shared_ptr<ast::Statement>;
  auto parse_integer(this Parser &self) -> std::shared_ptr<ast::Expression>;
  auto parse_float(this Parser &self) -> std::shared_ptr<ast::Expression>;
  auto parse_bool(this Parser &self) -> std::shared_ptr<ast::Expression>;
  auto parse_string(this Parser &self) -> std::shared_ptr<ast::Expression>;
  auto parse_array(this Parser &self) -> std::shared_ptr<ast::Expression>;
  auto peek_precedence(this Parser &self) -> Precedence;
  auto curr_precedence(this Parser &self) -> Precedence;
  auto parse_prefix_expression(this Parser &self)
      -> std::shared_ptr<ast::Expression>;
  auto parse_infix_expression(this Parser &self,
                              std::shared_ptr<ast::Expression> left)
      -> std::shared_ptr<ast::Expression>;
  auto parse_expression(this Parser &self, Precedence p)
      -> std::shared_ptr<ast::Expression>;
  auto parse_grouped_expression(this Parser &self)
      -> std::shared_ptr<ast::Expression>;
  auto parse_expression_statement(this Parser &self)
      -> std::shared_ptr<ast::ExpressionStatement>;
  auto parse_expression_list(this Parser &self, token::Token end)
      -> std::vector<std::shared_ptr<ast::Expression>>;
  auto parse_index_expression(this Parser &self,
                              std::shared_ptr<ast::Expression> left)
      -> std::shared_ptr<ast::Expression>;
  auto parse_operator_assignment(this Parser &self,
                                 std::shared_ptr<ast::Expression> left)
      -> std::shared_ptr<ast::Expression>;
  auto parse_if(this Parser &self) -> std::shared_ptr<ast::Expression>;
  auto parse_for(this Parser &self) -> std::shared_ptr<ast::Expression>;
  auto parse_return(this Parser &self) -> std::shared_ptr<ast::ReturnStatement>;

  auto parse_func_parameters(this Parser &self)
      -> std::vector<std::shared_ptr<ast::Identifier>>;
  auto parse_func(this Parser &self) -> std::shared_ptr<ast::Expression>;
  auto parse_func_call(this Parser &self, std::shared_ptr<ast::Expression> fn)
      -> std::shared_ptr<ast::Expression>;

  lexer::Lexer &lexer;
  std::vector<string> errors;
  std::map<token::Token, prefix_parse_fn> prefix_parse_fns;
  std::map<token::Token, infix_parse_fn> infix_parse_fns;
};
}; // namespace parser
#endif
