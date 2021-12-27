#pragma once

#include <vector>
#include "graph.hpp"

namespace uni_course_cpp {

namespace graph_generating {

bool can_generate_vertex(float probability);

Graph::VertexId get_random_vertex_id(
    const std::vector<uni_course_cpp::Graph::VertexId>& vertex_ids);

}  // namespace graph_generating

int get_random_between(int start, int second);

}  // namespace uni_course_cpp
