#include <cstdint>
#include <evaluator.hpp>
#include <fstream>
#include <lexer.hpp>
#include <memory>
#include <object.hpp>
#include <parser.hpp>
#include <print>
#include <repl.hpp>
#include <sstream>

int32_t main(int argc, char *argv[]) {
  if (argc < 2) {
    repl::run();
    return 0;
  }

  char *file_name = argv[1];
  std::ifstream file(file_name);
  if (!file.is_open()) {
    std::println("failed to open file {}", file_name);
    return 1;
  }

  std::ostringstream ss;
  ss << file.rdbuf();
  std::string data;
  data = ss.str();

  auto lexer = lexer::Lexer(file_name, data);
  auto parser = parser::Parser(lexer);
  auto program = parser.parse();
  auto errors = parser.get_errors();

  if (errors.size() > 0) {
    for (const auto &e : errors) {
      std::println("{}", e);
    }
    return 1;
  }

  auto env = std::make_shared<object::Environment>();
  auto eval = evaluator::Eval(lexer);
  auto res = eval.eval(std::move(program), env);
  if (evaluator::is_error(res)) {
    std::println("{}", res->debug());
    return 1;
  }

  return 0;
}
