#include "graph_printer.hpp"

namespace {

const std::string print_vertex(const uni_cpp_practice::Vertex& vertex) {
  std::stringstream ss_out;
  std::string tab_1 = "    ";
  std::string tab_2 = tab_1 + tab_1;
  ss_out << tab_2 << "{\n";
  ss_out << tab_2 << tab_1 << "\"id\": " << vertex.id << ",\n";
  ss_out << tab_2 << tab_1 << "\"edge_ids\": [";
  for (auto it = vertex.get_edge_ids().begin();
       it != vertex.get_edge_ids().end(); ++it) {
    if (it != vertex.get_edge_ids().begin()) {
      ss_out << ", ";
    }
    ss_out << *it;
  }
  ss_out << "],\n";
  ss_out << tab_2 << tab_1 << "\"depth\": " << vertex.depth << "\n";
  ss_out << tab_2 << "}";
  return ss_out.str();
}

const std::string print_edge(const uni_cpp_practice::Edge& edge) {
  std::stringstream ss_out;
  std::string tab_1 = "    ";
  std::string tab_2 = tab_1 + tab_1;
  ss_out << tab_2 << "{\n";
  ss_out << tab_2 << tab_1 << "\"id\": " << edge.get_id() << ",\n";
  ss_out << tab_2 << tab_1 << "\"vertex_ids\": [";
  ss_out << edge.get_binded_vertices().first << ", "
         << edge.get_binded_vertices().second;
  ss_out << "],\n";
  ss_out << tab_2 << tab_1 << "\"color\": "
         << "\"" << color_to_string(edge.color) << "\""
         << "\n";
  ss_out << tab_2 << "}";
  return ss_out.str();
}
}  // namespace

namespace uni_cpp_practice {
std::string GraphPrinter::print() const {
  std::stringstream ss_out;
  std::string tab_1 = "    ";
  ss_out << "{\n";
  ss_out << tab_1 << "\"depth\": " << graph_.get_depth() << ",\n";
  ss_out << tab_1 << "\"vertices\": [\n";
  for (auto it = graph_.get_vertex_map().begin();
       it != graph_.get_vertex_map().end(); ++it) {
    if (it != graph_.get_vertex_map().begin()) {
      ss_out << ", ";
    }
    ss_out << print_vertex(it->second);
  }
  ss_out << std::endl << tab_1 << "],\n";

  ss_out << tab_1 << "\"edges\": [\n";
  for (auto it = graph_.get_edge_map().begin();
       it != graph_.get_edge_map().end(); ++it) {
    if (it != graph_.get_edge_map().begin()) {
      ss_out << ", ";
    }
    ss_out << print_edge(it->second);
  }
  ss_out << std::endl << tab_1 << "]\n";
  ss_out << "}" << std::endl;
  return ss_out.str();
}
}  // namespace uni_cpp_practice
