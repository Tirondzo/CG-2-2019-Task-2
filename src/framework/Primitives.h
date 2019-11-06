#pragma once

#include "Objects3d.h"
#include <glad/glad.h>
#include <LiteMath.h>

#include <vector>
#include <string>

using LiteMath::float4x4;
using std::string;
using std::vector;

class PrimitiveMesh : public Mesh
{
protected:
  string name;
  GLuint vboVertices, vboIndices, vboNormals, vboTexCoords, vao;
  size_t ind_num;
  size_t pos_num;
  size_t tc_dim;

public:
  float4x4 model;
  uint32_t material_id;

  PrimitiveMesh() = default;
  PrimitiveMesh(const vector<float> &positions,
                const vector<float> &normals,
                const vector<float> &texcoords,
                const vector<uint32_t> &indices,
                size_t tex_cords_dim,
                size_t mat_id,
                string n);

  const string &GetName() const override;

  void Draw() override;
  void DrawInstanced(size_t count) override;

  ~PrimitiveMesh();
};

PrimitiveMesh* CreateSimpleTriangleMesh(string name = "SimpleTriangle");
PrimitiveMesh* CreateSimplePlaneMesh(string name = "SimplePlane");
PrimitiveMesh* CreateSimpleBoxMesh(string name = "SimpleBox");