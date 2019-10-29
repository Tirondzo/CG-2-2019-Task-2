#pragma once

#include <LiteMath.h>

#include <vector>
#include <string>

using LiteMath::float4x4;
using std::string;
using std::vector;

Mesh* CreateSimpleTriangleMesh(string name = "SimpleTriangle");

class Mesh
{
protected:
  string name;
  GLuint vboVertices, vboIndices, vboNormals, vboTexCoords, vao;
  size_t ind_num;

public:
  float4x4 model;
  uint32_t material_id;

  Mesh(const vector<float> &positions,
       const vector<float> &normals,
       const vector<float> &texcoords,
       const vector<uint32_t> &indices,
       size_t mat_id,
       string n);

  string getName();

  void draw();

  ~Mesh();
};