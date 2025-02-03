#include <ast.hpp>
#include <memory>
#include <parser.hpp>
#include <print>
#include <token.hpp>

using namespace parser;
using std::string;
using token::Token;

auto Parser::parse_identifier(this Parser &self)
    -> std::shared_ptr<ast::Expression> {
  auto expr = std::make_shared<ast::Identifier>();
  expr->pos = self.lexer.get_last_position();
  expr->value = std::any_cast<string>(self.lexer.get_value());
  return expr;
}

auto Parser::parse_let(this Parser &self)
    -> std::shared_ptr<ast::LetStatement> {
  auto stmt = std::make_shared<ast::LetStatement>();

  if (self.lexer.get_peek_token() != Token::TIDENTIFIER) {
    self.register_error("expected a identifier");
    return nullptr;
  }

  self.lexer.get_token();
  stmt->name = std::make_shared<ast::Identifier>();
  stmt->name->pos = self.lexer.get_last_position();
  stmt->name->value = std::any_cast<string>(self.lexer.get_value());

  if (self.lexer.get_peek_token() != Token::TASS) {
    self.register_error("a variable must be initialized with a value");
    return nullptr;
  }

  self.lexer.get_token();
  self.lexer.get_token();

  stmt->value = self.parse_expression(Precedence::LOWEST);

  if (self.lexer.get_peek_token() == Token::TSEMICOLON) {
    self.lexer.get_token();
  }

  return stmt;
}

auto Parser::parse_assignment(this Parser &self,
                              std::shared_ptr<ast::Expression> name)
    -> std::shared_ptr<ast::Expression> {
  auto expr = std::make_shared<ast::AssignmentExpression>();
  expr->pos = self.lexer.get_position();
  expr->name = name;

  self.lexer.get_token();
  expr->value = self.parse_expression(Precedence::LOWEST);
  return expr;
}

auto Parser::parse_block(this Parser &self)
    -> std::shared_ptr<ast::BlockStatement> {
  auto blk_stmt = std::make_shared<ast::BlockStatement>();
  blk_stmt->pos = self.lexer.get_position();
  self.lexer.get_token();

  while (self.lexer.get_last_token() != Token::TCCURLY &&
         self.lexer.get_last_token() != Token::TEOF) {
    auto stmt = self.parse_statement();
    if (stmt) {
      blk_stmt->statements.push_back(stmt);
    }
    self.lexer.get_token();
  }

  return blk_stmt;
}

auto Parser::parse_statement(this Parser &self)
    -> std::shared_ptr<ast::Statement> {
  switch (self.lexer.get_last_token()) {
  case Token::TSEMICOLON:
    self.lexer.get_token();
    return nullptr;
  case Token::TLET:
    return self.parse_let();
  case Token::TRETURN:
    return self.parse_return();
  case Token::TERROR:
    self.register_error(std::any_cast<string>(self.lexer.get_value()));
    return nullptr;
  default:
    return self.parse_expression_statement();
  }
}
