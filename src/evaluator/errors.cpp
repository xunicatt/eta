#include "object.hpp"
#include "types.hpp"
#include <evaluator.hpp>
#include <memory>

using namespace evaluator;

auto Eval::serror(string msg) -> const std::shared_ptr<SimpleError> {
  auto res = std::make_shared<SimpleError>();
  res->value = msg;
  return res;
}

auto Eval::derror(this Eval &self, const types::Position &node_pos,
                  const std::shared_ptr<SimpleError> err)
    -> const std::shared_ptr<DetailedError> {
  self.lexer.set_position(node_pos);
  self.lexer.get_token();

  auto pos = self.lexer.get_position();
  auto last_pos = self.lexer.get_last_position();
  auto error_msg =
      std::format("eta: \u001b[31merror in file: {}:{}:{}\033[0m\n",
                  self.lexer.get_filename(), last_pos.row + 1,
                  last_pos.cursor - last_pos.linebeg + 1);
  error_msg +=
      std::format("{} | {}\n", last_pos.row + 1, self.lexer.get_line());
  error_msg += std::format("{}{}   \u001b[31m{}\033[0m\n",
                           std::string(floor(log10(last_pos.row + 1) + 1), ' '),
                           std::string(last_pos.cursor - last_pos.linebeg, ' '),
                           std::string(pos.cursor - last_pos.cursor, '^'));
  error_msg += std::format("{}{}   \u001b[31m{}\033[0m\n",
                           std::string(floor(log10(last_pos.row + 1) + 1), ' '),
                           std::string(last_pos.cursor - last_pos.linebeg, ' '),
                           err->value);

  auto res = std::make_shared<DetailedError>();
  res->value = error_msg;
  return res;
}

auto Eval::error(const types::Position &node_pos,
                 const std::shared_ptr<Object> err)
    -> const std::shared_ptr<Object> {
  if (!err) {
    return OBJECT_NULL;
  }

  if (err->type() == ObjectType::OSIMPLEERROR) {
    return derror(node_pos, object::cast<Object, SimpleError>(std::move(err)));
  }

  return err;
}

auto evaluator::is_error(const std::shared_ptr<Object> err) -> bool {
  return (err && (err->type() == ObjectType::OSIMPLEERROR ||
                  err->type() == ObjectType::ODETAILEDERROR));
}
