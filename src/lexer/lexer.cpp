#include <any>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <lexer.hpp>
#include <map>
#include <print>
#include <string>
#include <string_view>
#include <token.hpp>
#include <types.hpp>

using lexer::Lexer;
using std::map;
using std::string_view;

static const map<string_view, token::Token> KEYWORDS = {
    {"let", token::Token::TLET},
    {"fn", token::Token::TFUNC},
    {"if", token::Token::TIF},
    {"else", token::Token::TELSE},
    {"for", token::Token::TFOR},
    {"break", token::Token::TBREAK},
    {"continue", token::Token::TCONTINUE},
    {"return", token::Token::TRETURN},
    {"switch", token::Token::TSWITCH},
    {"case", token::Token::TCASE},
    {"true", token::Token::TBOOL},
    {"false", token::Token::TBOOL},
    {"struct", token::Token::TSTRUCT},
};

static const map<char16_t, token::Token> SPECIAL_CHARS = {
    {'(', token::Token::TOPAREN}, {')', token::Token::TCPAREN},
    {'{', token::Token::TOCURLY}, {'}', token::Token::TCCURLY},
    {'[', token::Token::TOSQR},   {']', token::Token::TCSQR},
    {',', token::Token::TCOMMA},  {';', token::Token::TSEMICOLON},
    {':', token::Token::TCOLON},  {'>', token::Token::TGRT},
    {'<', token::Token::TLES},    {'.', token::Token::TDOT},
    {'+', token::Token::TADD},    {'-', token::Token::TSUB},
    {'/', token::Token::TDIV},    {'*', token::Token::TMUL},
    {'^', token::Token::TEXP},    {'&', token::Token::TUAND},
    {'|', token::Token::TUOR},    {'%', token::Token::TPER},
    {'=', token::Token::TASS},    {'~', token::Token::TUNOT},
    {'!', token::Token::TNOT},
};

Lexer::Lexer(const string &filename, const string &data)
    : filename(filename), data(data) {}

auto Lexer::set_position(this Lexer &self, const types::Position &pos) -> void {
  self.last_position = pos;
  self.position = pos;
}

auto Lexer::is_end(this const Lexer &self) -> bool {
  return self.position.cursor >= self.data.length();
}

auto Lexer::curr_char(this const Lexer &self) -> char16_t {
  if (self.is_end()) {
    return 0;
  }

  return self.data[self.position.cursor];
}

auto Lexer::forward(this Lexer &self) -> void {
  if (!self.is_end()) {
    self.position.cursor++;
    if (!self.is_end() && self.curr_char() == '\n') {
      self.position.row++;
      self.position.linebeg = self.position.cursor + 1;
    }
  }
}

auto Lexer::peek_char(this const Lexer &self) -> char16_t {
  if (self.position.cursor + 1 >= self.data.length()) {
    return 0;
  }

  return self.data[self.position.cursor + 1];
}

auto Lexer::pin_position(this Lexer &self) -> void {
  self.last_position = self.position;
}

auto Lexer::trim(this Lexer &self) -> void {
  while (!self.is_end() && std::isspace(self.curr_char())) {
    self.forward();
  }
}

auto Lexer::drop_line(this Lexer &self) -> void {
  while (!self.is_end() && self.curr_char() != '\n') {
    self.forward();
  }
}

auto Lexer::get_last_token(this const Lexer &self) -> token::Token {
  return self.last_token;
}

auto Lexer::_get_token(this Lexer &self) -> token::Token {
  self.trim();

  while (self.curr_char() == '#') {
    self.drop_line();
    self.trim();
  }

  self.pin_position();

  if (self.is_end()) {
    return token::Token::TEOF;
  }

  if (std::isalpha(self.curr_char()) || self.curr_char() == '_') {
    while (!self.is_end() &&
           (std::isalpha(self.curr_char()) || std::isdigit(self.curr_char()) ||
            self.curr_char() == '_')) {
      self.forward();
    }

    auto identifier =
        self.data.substr(self.last_position.cursor,
                         self.position.cursor - self.last_position.cursor);
    if (!KEYWORDS.contains(identifier)) {
      self.value = identifier;
      return token::Token::TIDENTIFIER;
    }

    auto tokenype = KEYWORDS.at(identifier);
    if (tokenype == token::Token::TBOOL) {
      self.value = identifier == "true";
    }
    return tokenype;
  }

  if (std::isdigit(self.curr_char())) {
    auto is_float = false;

    while (!self.is_end() &&
           (std::isdigit(self.curr_char()) || self.curr_char() == '.')) {
      if (self.curr_char() == '.' && !is_float) {
        is_float = true;
      }

      self.forward();
    }

    auto result =
        self.data.substr(self.last_position.cursor,
                         self.position.cursor - self.last_position.cursor);
    if (is_float) {
      self.value = (double_t)std::stold(result);
      return token::Token::TFLOAT;
    }

    self.value = (int64_t)std::stoll(result);
    return token::Token::TINT;
  }

  if (self.curr_char() == '\'' || self.curr_char() == '"') {
    auto q = self.curr_char();
    self.forward();

    string res{};
    while (!self.is_end() && self.curr_char() != q) {
      if (self.curr_char() == '\\' &&
          (self.peek_char() == q || self.peek_char() == '\\')) {
        self.forward();
      }

      res += self.curr_char();
      self.forward();
    }

    if (self.curr_char() != q) {
      self.value = "unterminated string literal";
      return token::Token::TERROR;
    }

    self.forward();
    self.value = res;
    return token::Token::TSTRING;
  }

  if (SPECIAL_CHARS.contains(self.curr_char())) {
    auto currokenype = SPECIAL_CHARS.at(self.curr_char());
    auto finalokenype = currokenype;
    self.forward();
    auto next_char = self.curr_char();

    if (SPECIAL_CHARS.contains(next_char)) {
      auto nextokenype = SPECIAL_CHARS.at(self.curr_char());
      switch (currokenype) {
      case token::Token::TASS:
        if (nextokenype == token::Token::TASS) {
          finalokenype = token::Token::TEQL;
        }
        break;

      case token::Token::TUOR:
        if (nextokenype == token::Token::TUOR) {
          finalokenype = token::Token::TOR;
        }
        break;

      case token::Token::TUAND:
        if (nextokenype == token::Token::TUAND) {
          finalokenype = token::Token::TAND;
        }
        break;

      case token::Token::TGRT:
        if (nextokenype == token::Token::TASS) {
          finalokenype = token::Token::TGRE;
        }
        break;

      case token::Token::TLEE:
        if (nextokenype == token::Token::TASS) {
          finalokenype = token::Token::TLEE;
        }
        break;

      case token::Token::TNOT:
        if (nextokenype == token::Token::TASS) {
          finalokenype = token::Token::TNEQL;
        }
        break;

      case token::Token::TADD:
        if (nextokenype == token::Token::TASS) {
          finalokenype = token::Token::TADAS;
        }
        break;

      case token::Token::TSUB:
        if (nextokenype == token::Token::TASS) {
          finalokenype = token::Token::TSBAS;
        }
        break;

      case token::Token::TMUL:
        if (nextokenype == token::Token::TASS) {
          finalokenype = token::Token::TMLAS;
        }
        break;

      case token::Token::TDIV:
        if (nextokenype == token::Token::TASS) {
          finalokenype = token::Token::TDVAS;
        }
        break;

      default:
        break;
      }

      if (currokenype != finalokenype) {
        self.forward();
        self.last_token = finalokenype;
      }
    }

    return finalokenype;
  }

  return token::Token::TERROR;
}

auto Lexer::get_token(this Lexer &self) -> token::Token {
  self.last_token = self._get_token();
#if __ETA_DEBUG_MODE__
  std::println("lexer: {}:{}:{} {}", self.filename, self.last_position.row + 1,
               self.last_position.cursor + 1 - self.last_position.linebeg,
               token::TokenName[self.last_token]);
#endif
  return self.last_token;
}

auto Lexer::get_value(this const Lexer &self) -> std::any { return self.value; }

auto Lexer::get_line(this const Lexer &self) -> string {
  auto end_index = self.data.find_first_of('\n', self.last_position.linebeg);
  if (end_index == std::string::npos) {
    return self.data.substr(self.last_position.linebeg);
  }
  auto res = self.data.substr(self.last_position.linebeg,
                              end_index - self.last_position.linebeg);
  return res;
}

auto Lexer::get_peek_token(this Lexer &self) -> token::Token {
  Lexer l = self;
  return l.get_token();
}

auto Lexer::get_position(this const Lexer &self) -> types::Position {
  return self.position;
}

auto Lexer::get_last_position(this const Lexer &self) -> types::Position {
  return self.last_position;
}

auto Lexer::get_filename(this const Lexer &self) -> const string & {
  return self.filename;
}
