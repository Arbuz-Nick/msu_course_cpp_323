#include "game_generator.hpp"
#include "game.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "random.hpp"
namespace uni_course_cpp {

Game GameGenerator::generate() const {
  auto game = Game();
  const auto graph_generator = GraphGenerator(params_);
  auto map = graph_generator.generate();
  game.set_princess_position(graph_generating::get_random_vertex_id(
      map.get_vertex_ids_on_depth(map.depth())));
  game.set_map(map);
  return game;
}

}  // namespace uni_course_cpp
