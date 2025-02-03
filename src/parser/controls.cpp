#include <ast.hpp>
#include <memory>
#include <parser.hpp>
#include <token.hpp>

using namespace parser;
using token::Token;

auto Parser::parse_return(this Parser &self)
    -> std::shared_ptr<ast::ReturnStatement> {
  auto stmt = std::make_shared<ast::ReturnStatement>();
  stmt->pos = self.lexer.get_position();
  self.lexer.get_token();

  if (self.lexer.get_last_token() == Token::TSEMICOLON) {
    return stmt;
  }

  stmt->value = self.parse_expression(Precedence::LOWEST);
  if (self.lexer.get_peek_token() == Token::TSEMICOLON) {
    self.lexer.get_token();
  }

  return stmt;
}

auto Parser::parse_if(this Parser &self) -> std::shared_ptr<ast::Expression> {
  auto expr = std::make_shared<ast::IfExpression>();
  expr->pos = self.lexer.get_position();

  if (self.lexer.get_peek_token() != Token::TOPAREN) {
    self.register_error("expected (");
    return nullptr;
  }

  self.lexer.get_token();
  expr->condition = self.parse_expression(LOWEST);

  if (self.lexer.get_last_token() != Token::TCPAREN) {
    self.register_error("expected )");
    return nullptr;
  }

  if (self.lexer.get_peek_token() != Token::TOCURLY) {
    self.register_error("expected {");
    return nullptr;
  }

  self.lexer.get_token();
  expr->consequence = self.parse_block();

  if (self.lexer.get_peek_token() != Token::TELSE) {
    return expr;
  }

  self.lexer.get_token();
  if (self.lexer.get_peek_token() != Token::TOCURLY) {
    self.register_error("expected {");
    return nullptr;
  }

  self.lexer.get_token();
  expr->alternative = self.parse_block();
  return expr;
}

auto Parser::parse_for(this Parser &self) -> std::shared_ptr<ast::Expression> {
  auto expr = std::make_shared<ast::ForExpression>();
  expr->pos = self.lexer.get_position();
  if (self.lexer.get_peek_token() != Token::TOPAREN) {
    self.register_error("expected (");
    return nullptr;
  }
  self.lexer.get_token();

  // init
  if (self.lexer.get_peek_token() != Token::TSEMICOLON) {
    self.lexer.get_token();
    expr->intialization = self.parse_let();
  } else {
    self.lexer.get_token();
  }

  // condition
  if (self.lexer.get_peek_token() != Token::TSEMICOLON) {
    self.lexer.get_token();
    expr->condition = self.parse_expression(LOWEST);
  }
  self.lexer.get_token();

  if (self.lexer.get_peek_token() != Token::TCPAREN) {
    self.lexer.get_token();
    expr->updation = self.parse_expression(LOWEST);
  }

  if (self.lexer.get_peek_token() != Token::TCPAREN) {
    self.register_error("expected )");
    return nullptr;
  }
  self.lexer.get_token();

  if (self.lexer.get_peek_token() != Token::TOCURLY) {
    self.register_error("expected {");
    return nullptr;
  }
  self.lexer.get_token();

  expr->body = self.parse_block();
  return expr;
}
