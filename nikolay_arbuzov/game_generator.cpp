#include "game_generator.hpp"
#include <iostream>
#include "game.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "random.hpp"
namespace uni_course_cpp {

Game GameGenerator::generate() const {
  const auto graph_generator = GraphGenerator(params_);
  auto map = graph_generator.generate();
  auto game =
      Game(std::move(map), graph_generating::get_random_vertex_id(
                               map.get_vertex_ids_on_depth(map.depth())));
  return game;
}

}  // namespace uni_course_cpp
