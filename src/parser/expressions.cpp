#include <ast.hpp>
#include <map>
#include <memory>
#include <parser.hpp>
#include <print>
#include <token.hpp>

using namespace parser;
using token::Token;

auto Parser::peek_precedence(this Parser &self) -> Precedence {
  if (!PRECEDENCES.contains(self.lexer.get_peek_token())) {
    return Precedence::LOWEST;
  }

  return PRECEDENCES.at(self.lexer.get_peek_token());
}

auto Parser::curr_precedence(this Parser &self) -> Precedence {
  if (!PRECEDENCES.contains(self.lexer.get_last_token())) {
    return Precedence::LOWEST;
  }

  return PRECEDENCES.at(self.lexer.get_last_token());
}

auto Parser::parse_prefix_expression(this Parser &self)
    -> std::shared_ptr<ast::Expression> {
  auto expr = std::make_shared<ast::PrefixExpression>();
  expr->pos = self.lexer.get_last_position();
  expr->op = self.lexer.get_last_token();

  self.lexer.get_token();
  expr->right = self.parse_expression(Precedence::PREFIX);
  return expr;
}

auto Parser::parse_infix_expression(this Parser &self,
                                    std::shared_ptr<ast::Expression> left)
    -> std::shared_ptr<ast::Expression> {
  auto expr = std::make_shared<ast::InfixExpression>();
  expr->pos = self.lexer.get_last_position();
  expr->op = self.lexer.get_last_token();
  expr->left = std::move(left);

  auto precedence = self.curr_precedence();
  self.lexer.get_token();
  expr->right = self.parse_expression(precedence);
  return expr;
}

auto Parser::parse_expression(this Parser &self, Precedence p)
    -> std::shared_ptr<ast::Expression> {
  if (!self.prefix_parse_fns.contains(self.lexer.get_last_token())) {
    self.register_error("no prefix parse function");
    return nullptr;
  }

  auto prefix_fn = self.prefix_parse_fns[self.lexer.get_last_token()];
  auto left = prefix_fn();

  while (self.lexer.get_peek_token() != Token::TSEMICOLON &&
         p < self.peek_precedence()) {
    if (!self.infix_parse_fns.contains(self.lexer.get_peek_token())) {
      return left;
    }
    auto infix_fn = self.infix_parse_fns[self.lexer.get_peek_token()];

    self.lexer.get_token();
    left = infix_fn(std::move(left));
  }

  return left;
}

auto Parser::parse_grouped_expression(this Parser &self)
    -> std::shared_ptr<ast::Expression> {
  self.lexer.get_token();
  auto expr = self.parse_expression(Precedence::LOWEST);

  if (self.lexer.get_token() != Token::TCPAREN) {
    self.register_error("expected )");
    return nullptr;
  }

  return expr;
}

auto Parser::parse_expression_statement(this Parser &self)
    -> std::shared_ptr<ast::ExpressionStatement> {
  auto stmt = std::make_shared<ast::ExpressionStatement>();
  stmt->pos = self.lexer.get_position();
  stmt->expression = self.parse_expression(Precedence::LOWEST);

  if (self.lexer.get_peek_token() == Token::TSEMICOLON) {
    self.lexer.get_token();
  }

  return stmt;
}

auto Parser::parse_expression_list(this Parser &self, token::Token end)
    -> std::vector<std::shared_ptr<ast::Expression>> {
  auto args = std::vector<std::shared_ptr<ast::Expression>>{};
  if (self.lexer.get_peek_token() == end) {
    self.lexer.get_token();
    return args;
  }

  self.lexer.get_token();
  while (true) {
    args.push_back(self.parse_expression(Precedence::LOWEST));

    self.lexer.get_token();
    if (self.lexer.get_last_token() == end) {
      break;
    }

    if (self.lexer.get_last_token() != Token::TCOMMA) {
      self.register_error("expected ,");
      return {};
    }

    self.lexer.get_token();
  }

  return args;
}

auto Parser::parse_index_expression(this Parser &self,
                                    std::shared_ptr<ast::Expression> left)
    -> std::shared_ptr<ast::Expression> {
  auto expr = std::make_shared<ast::IndexExpression>();
  expr->pos = self.lexer.get_position();
  expr->left = std::move(left);

  self.lexer.get_token();
  expr->index = self.parse_expression(Precedence::LOWEST);

  if (self.lexer.get_peek_token() != Token::TCSQR) {
    self.register_error("expected ]");
    return nullptr;
  }

  self.lexer.get_token();
  return expr;
}

auto Parser::parse_operator_assignment(this Parser &self,
                                       std::shared_ptr<ast::Expression> left)
    -> std::shared_ptr<ast::Expression> {
  auto new_op = std::map<Token, Token>{
      {Token::TADAS, Token::TADD},
      {Token::TSBAS, Token::TSUB},
      {Token::TMLAS, Token::TMUL},
      {Token::TDVAS, Token::TDIV},
  };

  auto op = new_op[self.lexer.get_last_token()];
  auto expr = std::make_shared<ast::OpAssignment>();
  expr->pos = self.lexer.get_position();
  expr->op = op;
  expr->name = std::move(left);

  self.lexer.get_token();
  expr->value = self.parse_expression(Precedence::LOWEST);
  return expr;
}
