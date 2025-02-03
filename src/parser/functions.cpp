#include <ast.hpp>
#include <memory>
#include <parser.hpp>
#include <token.hpp>
#include <vector>

using namespace parser;
using token::Token;

auto Parser::parse_func_parameters(this Parser &self)
    -> std::vector<std::shared_ptr<ast::Identifier>> {
  auto identifiers = std::vector<std::shared_ptr<ast::Identifier>>{};
  if (self.lexer.get_peek_token() == Token::TCPAREN) {
    self.lexer.get_token();
    return identifiers;
  }

  self.lexer.get_token();
  while (true) {
    if (self.lexer.get_last_token() != Token::TIDENTIFIER) {
      self.register_error("expected an identifier");
      return {};
    }

    auto identifier = std::make_shared<ast::Identifier>();
    identifier->pos = self.lexer.get_last_position();
    identifier->value = std::any_cast<string>(self.lexer.get_value());
    identifiers.push_back(std::move(identifier));
    self.lexer.get_token();

    if (self.lexer.get_last_token() == Token::TCPAREN) {
      break;
    }

    if (self.lexer.get_last_token() != Token::TCOMMA) {
      self.register_error("expected ,");
      return {};
    }

    self.lexer.get_token();
  }

  return identifiers;
}

auto Parser::parse_func(this Parser &self) -> std::shared_ptr<ast::Expression> {
  auto lit = std::make_shared<ast::FunctionLiteral>();
  lit->pos = self.lexer.get_position();

  if (self.lexer.get_peek_token() != Token::TOPAREN) {
    self.register_error("expected (");
    return nullptr;
  }

  self.lexer.get_token();
  lit->parameters = std::vector<std::shared_ptr<ast::Identifier>>(
      self.parse_func_parameters());

  if (self.lexer.get_peek_token() != Token::TOCURLY) {
    self.register_error("expected {");
    return nullptr;
  }

  self.lexer.get_token();
  lit->body = self.parse_block();
  return lit;
}

auto Parser::parse_func_call(this Parser &self,
                             std::shared_ptr<ast::Expression> fn)
    -> std::shared_ptr<ast::Expression> {
  auto expr = std::make_shared<ast::CallExpression>();
  expr->pos = self.lexer.get_position();
  expr->function = std::move(fn);
  expr->arguments = self.parse_expression_list(Token::TCPAREN);
  return expr;
}
