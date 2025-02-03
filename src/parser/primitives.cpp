#include <ast.hpp>
#include <cmath>
#include <cstdint>
#include <memory>
#include <parser.hpp>
#include <token.hpp>

using namespace parser;
using token::Token;

auto Parser::parse_integer(this Parser &self)
    -> std::shared_ptr<ast::Expression> {
  auto lit = std::make_shared<ast::IntegerLiteral>();
  lit->pos = self.lexer.get_position();
  lit->value = std::any_cast<int64_t>(self.lexer.get_value());
  return lit;
}

auto Parser::parse_float(this Parser &self)
    -> std::shared_ptr<ast::Expression> {
  auto lit = std::make_shared<ast::FloatLiteral>();
  lit->pos = self.lexer.get_position();
  lit->value = std::any_cast<double_t>(self.lexer.get_value());
  return lit;
}

auto Parser::parse_bool(this Parser &self) -> std::shared_ptr<ast::Expression> {
  auto lit = std::make_shared<ast::BoolLiteral>();
  lit->pos = self.lexer.get_position();
  lit->value = std::any_cast<bool>(self.lexer.get_value());
  return lit;
}

auto Parser::parse_string(this Parser &self)
    -> std::shared_ptr<ast::Expression> {
  auto lit = std::make_shared<ast::StringLiteral>();
  lit->pos = self.lexer.get_position();
  lit->value = std::any_cast<string>(self.lexer.get_value());
  return lit;
}

auto Parser::parse_array(this Parser &self)
    -> std::shared_ptr<ast::Expression> {
  auto lit = std::make_shared<ast::ArrayLiteral>();
  lit->pos = self.lexer.get_position();
  lit->elements = self.parse_expression_list(Token::TCSQR);
  self.lexer.get_token();
  return lit;
}
