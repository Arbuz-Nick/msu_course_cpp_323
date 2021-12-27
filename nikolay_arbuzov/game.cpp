#include "game.hpp"
#include "graph_path.hpp"
#include "graph_traverser.hpp"
namespace uni_course_cpp {
GraphPath Game::find_shortest_path() const {
  const auto graph_traverser = GraphTraverser(map_);
  return graph_traverser.find_shortest_path(map_, knight_position_,
                                            princess_position_);
}
GraphPath Game::find_fastest_path() const {
  const auto graph_traverser = GraphTraverser(map_);
  return graph_traverser.find_fastest_path(map_, knight_position_,
                                           princess_position_);
}
}  // namespace uni_course_cpp
