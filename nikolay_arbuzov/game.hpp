#pragma once

#include "graph_path.hpp"
#include "graph.hpp"

class Game {
 public:
  GraphPath find_shortest_path() const;
  GraphPath find_fastest_path() const;
 private:
  Graph map_;
  Graph::VertexId knight_position_;
  Graph::VertexId princess_position_;
};