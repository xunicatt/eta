#include <algorithm>
#include <ast.hpp>
#include <memory>
#include <parser.hpp>
#include <print>
#include <token.hpp>

using namespace parser;

#define LAMDA_PREFIX(fn) [this]() { return fn; }

#define LAMDA_INFIX(fn)                                                        \
  [this](std::shared_ptr<ast::Expression> left) { return fn; }

Parser::Parser(lexer::Lexer &lexer) : lexer(lexer) {
  // PREFIX FNS
  this->register_prefix_fn(token::Token::TIDENTIFIER,
                           LAMDA_PREFIX(this->parse_identifier()));
  this->register_prefix_fn(token::Token::TINT,
                           LAMDA_PREFIX(this->parse_integer()));
  this->register_prefix_fn(token::Token::TFLOAT,
                           LAMDA_PREFIX(this->parse_float()));
  this->register_prefix_fn(token::Token::TBOOL,
                           LAMDA_PREFIX(this->parse_bool()));
  this->register_prefix_fn(token::Token::TSTRING,
                           LAMDA_PREFIX(this->parse_string()));
  this->register_prefix_fn(token::Token::TNOT,
                           LAMDA_PREFIX(this->parse_prefix_expression()));
  this->register_prefix_fn(token::Token::TSUB,
                           LAMDA_PREFIX(this->parse_prefix_expression()));
  this->register_prefix_fn(token::Token::TOPAREN,
                           LAMDA_PREFIX(this->parse_grouped_expression()));
  this->register_prefix_fn(token::Token::TIF, LAMDA_PREFIX(this->parse_if()));
  this->register_prefix_fn(token::Token::TFOR, LAMDA_PREFIX(this->parse_for()));
  this->register_prefix_fn(token::Token::TFUNC,
                           LAMDA_PREFIX(this->parse_func()));
  this->register_prefix_fn(token::Token::TOSQR,
                           LAMDA_PREFIX(this->parse_array()));

  // INFIX
  this->register_infix_fn(
      token::Token::TADD,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TSUB,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TMUL,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TDIV,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TEQL,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TNEQL,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TLES,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TLEE,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TGRT,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TGRE,
      LAMDA_INFIX(this->parse_infix_expression(std::move(left))));
  this->register_infix_fn(token::Token::TOPAREN,
                          LAMDA_INFIX(this->parse_func_call(std::move(left))));
  this->register_infix_fn(token::Token::TASS,
                          LAMDA_INFIX(this->parse_assignment(std::move(left))));
  this->register_infix_fn(
      token::Token::TOSQR,
      LAMDA_INFIX(this->parse_index_expression(std::move(left))));
  this->register_infix_fn(
      token::Token::TADAS,
      LAMDA_INFIX(this->parse_operator_assignment(std::move(left))));
  this->register_infix_fn(
      token::Token::TSBAS,
      LAMDA_INFIX(this->parse_operator_assignment(std::move(left))));
  this->register_infix_fn(
      token::Token::TMLAS,
      LAMDA_INFIX(this->parse_operator_assignment(std::move(left))));
  this->register_infix_fn(
      token::Token::TDVAS,
      LAMDA_INFIX(this->parse_operator_assignment(std::move(left))));
}

auto Parser::parse(this Parser &self) -> std::shared_ptr<ast::Program> {
  auto program = std::make_shared<ast::Program>();

  while (self.lexer.get_token() != token::Token::TEOF) {
    if (self.lexer.get_last_token() == token::Token::TERROR) {
      self.register_error("unknown token");
      return {};
    }

    if (auto stmt = self.parse_statement(); stmt) {
#if __ETA_DEBUG_MODE__
      std::println("parser: {}", stmt->debug());
#endif
      program->statements.push_back(std::move(stmt));
    }
  }

  return program;
}

auto Parser::register_prefix_fn(this Parser &self, token::Token tk,
                                prefix_parse_fn fn) -> void {
  self.prefix_parse_fns[tk] = fn;
}

auto Parser::register_infix_fn(this Parser &self, token::Token tk,
                               infix_parse_fn fn) -> void {
  self.infix_parse_fns[tk] = fn;
}
