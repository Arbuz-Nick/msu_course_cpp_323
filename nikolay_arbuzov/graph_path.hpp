#pragma once

#include <vector>
#include "graph.hpp"
namespace uni_course_cpp {
struct GraphPath {
  using Distance = int;
<<<<<<< HEAD
  GraphPath(std::vector<Graph::VertexId> _vertex_ids,
            Distance _distance,
            Graph::Edge::Duration _duration)
      : vertex_ids(std::move(_vertex_ids)),
        distance(_distance),
        duration(_duration) {}
=======
  GraphPath(std::vector<Graph::VertexId> _vertex_ids, Distance _distance)
      : vertex_ids(std::move(_vertex_ids)), distance(_distance) {}
>>>>>>> be413175a6824cb4348b4497b91990ae47c0fd6c
  std::vector<Graph::VertexId> vertex_ids;
  Graph::Edge::Duration duration = 0;
  Distance distance = 0;
};

}  // namespace uni_course_cpp
