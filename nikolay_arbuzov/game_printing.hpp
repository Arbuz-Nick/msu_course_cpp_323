#pragma once

#include <string>
#include "game.hpp"
#include "graph_printing.hpp"
namespace uni_course_cpp {
namespace printing {

std::string print_game(const Game& game);

namespace json {
inline std::string print_map(const Graph& map) {
  return print_graph(map);
}
}  // namespace json

}  // namespace printing
}  // namespace uni_course_cpp
