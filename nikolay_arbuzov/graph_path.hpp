#pragma once

#include <vector>
#include "graph.hpp"
namespace uni_course_cpp {
struct GraphPath {
  using Distance = int;
  GraphPath(std::vector<Graph::VertexId> _vertex_ids,
            Distance _distance,
            Graph::Edge::Duration _duration)
      : vertex_ids(std::move(_vertex_ids)),
        distance(_distance),
        duration(_duration) {}
  std::vector<Graph::VertexId> vertex_ids;
  Graph::Edge::Duration duration = 0;
  Distance distance = 0;
};

}  // namespace uni_course_cpp
