#ifndef __ETA_TYPES_H__
#define __ETA_TYPES_H__

#include <cstddef>

namespace types {
  struct Position {
    size_t cursor;
    size_t row;
    size_t linebeg;

    Position();
  };
};  // namespace types

#endif
