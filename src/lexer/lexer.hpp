#ifndef __ETA_LEXER_HPP__
#define __ETA_LEXER_HPP__

#include <any>
#include <debug.hpp>
#include <string>
#include <token.hpp>
#include <types.hpp>

using std::any;
using std::string;

namespace lexer {
class Lexer {
public:
  Lexer(const string &filename, const string &data);
  auto set_position(this Lexer &self, const types::Position &pos) -> void;
  auto get_last_token(this const Lexer &self) -> token::Token;
  auto get_token(this Lexer &self) -> token::Token;
  auto get_value(this const Lexer &self) -> std::any;
  auto get_line(this const Lexer &self) -> string;
  auto get_peek_token(this Lexer &self) -> token::Token;
  auto get_position(this const Lexer &self) -> types::Position;
  auto get_last_position(this const Lexer &self) -> types::Position;
  auto get_filename(this const Lexer &self) -> const string &;

private:
  auto is_end(this const Lexer &self) -> bool;
  auto curr_char(this const Lexer &self) -> char16_t;
  auto peek_char(this const Lexer &self) -> char16_t;
  auto forward(this Lexer &self) -> void;
  auto pin_position(this Lexer &self) -> void;
  auto trim(this Lexer &self) -> void;
  auto drop_line(this Lexer &self) -> void;
  auto _get_token(this Lexer &self) -> token::Token;

  const string &filename;
  const string &data;
  token::Token last_token;
  types::Position position;
  types::Position last_position;
  any value;
};
}; // namespace lexer

#endif
