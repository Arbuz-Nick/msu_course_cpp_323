#include "game_printing.hpp"
#include <sstream>
#include <string>
#include "game.hpp"
#include "graph_json_printing.hpp"
#include "graph_printing.hpp"
namespace uni_course_cpp {
namespace printing {

std::string print_game(const Game& game) {
  std::stringstream result;

  const auto& map = game.get_map();
  const auto& knight_position = game.get_knight_position();
  const auto& princess_position = game.get_princess_position();
  result << "  map: " << print_graph(map);
  result << "," << std::endl;
  result << "  knight position: {vertex_id: " << knight_position;
  result << ", depth: " << map.get_vertex_depth(knight_position) << "},"
         << std::endl;
  result << "  princess position: {vertex_id: " << princess_position;
  result << ", depth: " << map.get_vertex_depth(princess_position) << "}"
         << std::endl
         << "}";
  return result.str();
}

}  // namespace printing
}  // namespace uni_course_cpp
