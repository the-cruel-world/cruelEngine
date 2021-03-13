#include "mesh.hpp"

namespace cruelEngine::cruelScene {

Mesh::Mesh() {}

Mesh::~Mesh() { clear(); }

void Mesh::clear() {
  vertices.clear();
  triangles.clear();
}

void Mesh::addTriangle(u16 a, u16 b, u16 c) {
  triangles.emplace_back(a);
  triangles.emplace_back(b);
  triangles.emplace_back(c);
}

void Mesh::addVertex(Vertex &vertex) {
  vertices.emplace_back(std::move(vertex));
}

} // namespace cruelEngine::cruelScene