#include <ast.hpp>
#include <cstdint>
#include <evaluator.hpp>
#include <iostream>
#include <lexer.hpp>
#include <map>
#include <memory>
#include <object.hpp>
#include <parser.hpp>
#include <print>
#include <repl.hpp>
#include <string>
#include <string_view>

using std::map;
using std::string;
using std::string_view;

static const string_view HELPER = "etalang repl --- type '.help' for help";
static const string_view PROMPT = ">> ";
static const string_view VERSION = "alpha 0.1v";

static const map<string, uint8_t> OPTIONS = {
    {".help", 1},
    {".clear", 2},
    {".ver", 3},
    {".exit", 4},
};

auto repl::run() -> void {
  std::println("{}", HELPER);
  std::println("{}", VERSION);

  string line;
  auto env = std::make_shared<object::Environment>();

  while (true) {
    std::print("{}", PROMPT);
    std::getline(std::cin, line);

    if (OPTIONS.contains(line)) {
      switch (OPTIONS.at(line)) {
      case 1:
        std::println(".help  --- help");
        std::println(".clear --- clear the terminal");
        std::println(".ver   --- shows the eta version");
        std::println(".exit  --- exits the repl");
        break;

      case 2:
        std::println("\033c");
        break;

      case 3:
        std::println("{}", VERSION);
        break;

      case 4:
        return;
      }

      continue;
    }

    auto lex = lexer::Lexer("repl", line);
    auto par = parser::Parser(lex);
    auto prgm = par.parse();
    auto errors = par.get_errors();
    if (errors.size() > 0) {
      for (const auto &e : errors) {
        std::println("{}", e);
      }
      continue;
    }

    auto e = evaluator::Eval(lex);
    auto x = e.eval(std::move(prgm), env);
    std::println("{}", x->debug());
  }
}
