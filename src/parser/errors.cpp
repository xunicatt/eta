#include <cmath>
#include <format>
#include <parser.hpp>
#include <string>

using namespace parser;

auto Parser::register_error(this Parser &self, string msg) -> void {
  auto pos       = self.lexer.get_position();
  auto last_pos  = self.lexer.get_last_position();
  auto error_msg = std::format(
    "eta: \u001b[31merror in file: {}:{}:{}\033[0m\n",
    self.lexer.get_filename(), last_pos.row + 1,
    last_pos.cursor - last_pos.linebeg + 1
  );
  error_msg +=
    std::format("{} | {}\n", last_pos.row + 1, self.lexer.get_line());
  error_msg += std::format(
    "{}{}   \u001b[31m{}\033[0m\n",
    std::string(floor(log10(last_pos.row + 1) + 1), ' '),
    std::string(last_pos.cursor - last_pos.linebeg, ' '),
    std::string(pos.cursor - last_pos.cursor, '^')
  );
  error_msg += std::format(
    "{}{}   \u001b[31m{}\033[0m\n",
    std::string(floor(log10(last_pos.row + 1) + 1), ' '),
    std::string(last_pos.cursor - last_pos.linebeg, ' '), msg
  );
  self.errors.push_back(error_msg);
}

auto Parser::get_errors(this Parser const &self)
  -> const std::vector<string> & {
  return self.errors;
}
