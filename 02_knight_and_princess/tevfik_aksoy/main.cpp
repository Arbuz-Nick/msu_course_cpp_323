#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using std::endl;
using std::get;
using std::to_string;
using std::vector;

using VertexId = int;
using EdgeId = int;

struct Vertex {
 public:
  const VertexId id;
  vector<EdgeId> edge_ids;

  explicit Vertex(VertexId _id) : id(_id) {}

  std::string to_JSON() const {
    std::string json_string;

    json_string += "\t{ \"id\": " + to_string(id) + ", \"edge_ids\": [";
    for (int i = 0; i < edge_ids.size(); i++) {
      json_string += to_string(edge_ids[i]);
      if (i + 1 != edge_ids.size())
        json_string += ", ";
    }
    return json_string;
  }
};

struct Edge {
 public:
  const EdgeId id;
  const VertexId source;
  const VertexId destination;

  Edge(VertexId src_id, VertexId dest_id, EdgeId _id)
      : id(_id), source(src_id), destination(dest_id) {}

  std::string to_JSON() const {
    std::string json_string;
    json_string += "\t{ \"id\": " + to_string(id) + ", \"vertex_ids\": [" +
                   to_string(source) + ", " + to_string(destination) + "] }";
    return json_string;
  }
};

class Graph {
 public:
  void insert_node(VertexId source) { vertices.push_back(Vertex(source)); }
  void insert_edge(VertexId source, VertexId destination, EdgeId id) {
    if (!edge_already_exists(id)) {
      if (is_vertex_valid(source) && is_vertex_valid(destination)) {
        edges.push_back(Edge(source, destination, id));
        vertices[source].edge_ids.push_back(id);
        vertices[destination].edge_ids.push_back(id);
      }
    }
  }

  //Проверка существует ли edge у графа, чтобы запретить добавление дубликатов
  bool edge_already_exists(EdgeId id) {
    for (const auto& edge : edges)
      if (edge.id == id) {
        std::cerr << "There already exists edge: " << id << "\n";
        return true;
      }
    return false;
  }

  //Проверка существует ли vertex у графа, чтобы предотвратить попытку на доступ
  //невыделенной памяти
  bool is_vertex_valid(VertexId id) {
    for (const auto& vertex : vertices)
      if (vertex.id == id) {
        return true;
      }
    std::cerr << "Vertex: " << id << " is not valid!\n";
    return false;
  }

  std::string to_JSON() const {
    std::string json_string;
    json_string += "{\n\"vertices\": [\n";
    for (int i = 0; i < vertices.size(); i++) {
      json_string += vertices[i].to_JSON();
      if (i + 1 == vertices.size()) {
        json_string += "] }\n  ],\n";
      } else {
        json_string += "] },\n";
      }
    }

    json_string += "\"edges\": [\n";
    for (int i = 0; i < edges.size(); i++) {
      json_string += edges[i].to_JSON();
      if (i + 1 == edges.size()) {
        json_string += "\n";
      } else {
        json_string += ",\n";
      }
    }
    json_string += "  ]\n}\n";
    return json_string;
  }

 private:
  vector<Edge> edges;
  vector<Vertex> vertices;
};

const Graph generateGraph() {
  Graph graph;
  for (int i = 0; i < 14; i++) {
    graph.insert_node(i);
  }
  graph.insert_edge(0, 1, 0);
  graph.insert_edge(0, 2, 1);
  graph.insert_edge(0, 3, 2);
  graph.insert_edge(1, 4, 3);
  graph.insert_edge(1, 5, 4);
  graph.insert_edge(1, 6, 5);
  graph.insert_edge(2, 7, 6);
  graph.insert_edge(2, 8, 7);
  graph.insert_edge(3, 9, 8);
  graph.insert_edge(4, 10, 9);
  graph.insert_edge(5, 10, 10);
  graph.insert_edge(6, 10, 11);
  graph.insert_edge(7, 11, 12);
  graph.insert_edge(8, 11, 13);
  graph.insert_edge(9, 12, 14);
  graph.insert_edge(10, 13, 15);
  graph.insert_edge(11, 13, 16);
  graph.insert_edge(12, 13, 17);
  //Это не будет добавлено, т.к. такой edge уже существует
  graph.insert_edge(13, 13, 17);
  //Это тоже не будет добавлено, т.к. Vertex 14 не существует
  graph.insert_edge(13, 14, 18);
  return graph;
}

int main() {
  const auto graph = generateGraph();
  std::ofstream file("graph.json", std::ios::out);
  if (!file.is_open())
    std::cerr << "Error opening the file graph.json!" << endl;
  else {
    file << graph.to_JSON();
  }
  return 0;
}
