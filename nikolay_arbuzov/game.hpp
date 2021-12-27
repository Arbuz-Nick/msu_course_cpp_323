#pragma once

#include "graph.hpp"
#include "graph_path.hpp"
namespace uni_course_cpp {
class Game {
 public:
  Game(Graph map, Graph::VertexId princess_position)
      : map_(std::move(map)), princess_position_(princess_position) {}
  GraphPath find_shortest_path() const;
  GraphPath find_fastest_path() const;
  const Graph& get_map() const { return map_; }
  const Graph::VertexId& get_knight_position() const {
    return knight_position_;
  }
  const Graph::VertexId& get_princess_position() const {
    return princess_position_;
  }

 private:
  Graph map_;
  Graph::VertexId knight_position_ = 0;
  Graph::VertexId princess_position_;
};
}  // namespace uni_course_cpp
