#ifndef __ETA_TOKEN_HPP__
#define __ETA_TOKEN_HPP__

#include <array>
#include <cstdint>
#include <string_view>

using std::array;
using std::string_view;

namespace token {
  enum Token : uint8_t {
    // KEYWORDS
    TNONE = 0,
    TIMPORT,
    TSTRUCT,
    TMUT,
    TFOR,
    TIF,
    TELSE,
    TRETURN,
    TBREAK,
    TCONTINUE,
    TSWITCH,
    TCASE,
    TEXTERN,
    TENUM,
    TDEFER,
    TFUNC,
    TLET,

    // TYPES
    TINT,
    TFLOAT,
    TSTRING,
    TBOOL,

    // SPECIAL CHARS
    TOPAREN,
    TCPAREN,
    TOCURLY,
    TCCURLY,
    TOSQR,
    TCSQR,
    TCOMMA,
    TSEMICOLON,
    TCOLON,
    TGRT,
    TGRE,
    TLES,
    TLEE,
    TDOT,
    TADD,
    TSUB,
    TDIV,
    TMUL,
    TEXP,
    TOR,
    TAND,
    TUOR,
    TUAND,
    TPER,
    TASS,
    TEQL,
    TNEQL,
    TUNOT,
    TNOT,

    TADAS,
    TSBAS,
    TMLAS,
    TDVAS,

    // EXTRAS
    TIDENTIFIER,
    TUNKNOWN,
    TEOF,
    TERROR,
    __TOKENCOUNT__,
  };

  const array<string_view, __TOKENCOUNT__> TokenName = {
    "none",        "import",
    "struct",      "mut",
    "for",         "if",
    "else",        "return",
    "break",       "continue",
    "switch",      "case",
    "extern",      "enum",
    "defer",       "function",
    "let",

    "int",         "float",
    "string",      "bool",

    "(",           ")",
    "{",           "}",
    "[",           "]",
    ",",           ";",
    ":",           ">",
    ">=",          "<",
    "<=",          ".",
    "+",           "-",
    "/",           "*",
    "^",           "||",
    "&&",          "|",
    "&",           "%",
    "=",           "==",
    "!=",          "~",
    "!",

    "+=",          "-=",
    "*=",          "/=",

    "identifier",  "unknown token",
    "end of file", "error",
  };
};  // namespace token

#endif
