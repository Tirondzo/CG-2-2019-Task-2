#include "Primitives.h"
#include "HelperGL.h"

PrimitiveMesh *CreateSimpleTriangleMesh(string name)
{
  vector<float> pos{
      -1, 1, -0.5,
      -1, -1, -0.5,
      1, -1, -0.5};

  vector<float> norm{
      0, 0, 1,
      0, 0, 1,
      0, 0, 1};

  vector<float> texc{
      0, 1,
      0, 0,
      1, 0};

  vector<uint32_t> ind{
      0, 1, 2};

  return new PrimitiveMesh(pos, norm, texc, ind, 2, -1, name);
}

PrimitiveMesh *CreateSimpleBoxMesh(string name)
{
  vector<float> pos{
      1,1,1,
      1,1,-1,
      1,-1,1,
      1,-1,-1,
      -1,1,1,
      -1,1,-1,
      -1,-1,1,
      -1,-1,-1};
  vector<float> norm{};

  vector<float> texc{};

  vector<uint32_t> ind{
      0, 1, 2, 1, 2, 3, //xpos
      0, 1, 4, 1, 4, 5, //ypos
      0, 2, 4, 2, 4, 6, //zpos
      7, 6, 5, 6, 5, 4, //xneg
      7, 6, 3, 6, 3, 2, //yneg
      7, 5, 3, 5, 3, 1};//zneg

  return new PrimitiveMesh(pos, norm, texc, ind, -1,  -1, name);
}

PrimitiveMesh *CreateSimplePlaneMesh(string name)
{
  vector<float> pos{
      1,0,1,
      1,0,-1,
      -1,0,1,
      -1,0,-1};
  vector<float> norm{};

  vector<float> texc{};

  vector<uint32_t> ind{
      0, 1, 2, 1, 2, 3};

  return new PrimitiveMesh(pos, norm, texc, ind, -1,  -1, name);
}


PrimitiveMesh::PrimitiveMesh(const vector<float> &positions,
                             const vector<float> &normals,
                             const vector<float> &texcoords,
                             const vector<uint32_t> &indices,
                             size_t tex_cords_dim,
                             size_t mat_id,
                             string n)
{
  name = n;
  tc_dim = tex_cords_dim;

  ind_num = indices.size();
  pos_num = positions.size();

  GL_CHECK( glGenVertexArrays(1, &vao) );
  GL_CHECK( glGenBuffers(1, &vboVertices) );
  GL_CHECK( glGenBuffers(1, &vboIndices) );
  GL_CHECK( glGenBuffers(1, &vboNormals) );
  GL_CHECK( glGenBuffers(1, &vboTexCoords) );

  GL_CHECK( glBindVertexArray(vao) );
  {
    // Pass Vertex's Positions to shader
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, vboVertices) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GL_FLOAT), positions.data(), GL_STATIC_DRAW) );
    GL_CHECK( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *)0) );
    GL_CHECK( glEnableVertexAttribArray(0) );

    // Pass Vertex's Normals to shader
    if (normals.size())
    {
      GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, vboNormals) );
      GL_CHECK( glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GL_FLOAT), normals.data(), GL_STATIC_DRAW) );
      GL_CHECK( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *)0) );
      GL_CHECK( glEnableVertexAttribArray(1) );
    }

    // Pass Vertex's Texture Coordinates to shader
    if (texcoords.size())
    {
      GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords) );
      GL_CHECK( glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(GL_FLOAT), texcoords.data(), GL_STATIC_DRAW) );
      GL_CHECK( glVertexAttribPointer(2, tc_dim, GL_FLOAT, GL_FALSE, tc_dim * sizeof(GL_FLOAT), (GLvoid *)0) );
      GL_CHECK( glEnableVertexAttribArray(2) );
    }

    // Pass Indexes to shader
    if (indices.size())
    {
      GL_CHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices) );
      GL_CHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW) );
    }
  }
  GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, 0) );
  GL_CHECK( glBindVertexArray(0) );

  material_id = mat_id;
}

const string &PrimitiveMesh::GetName() const
{
  return name;
}

void PrimitiveMesh::Draw()
{
  GL_CHECK( glBindVertexArray(vao) );
  if (ind_num) glDrawElements(GL_TRIANGLES, ind_num, GL_UNSIGNED_INT, nullptr);
  else glDrawArrays(GL_TRIANGLES, 0, pos_num);
  GL_CHECK_ERRORS;
  GL_CHECK( glBindVertexArray(0) );
}

void PrimitiveMesh::DrawInstanced(size_t count)
{
  GL_CHECK( glBindVertexArray(vao) );
  if (ind_num) glDrawElementsInstanced(GL_TRIANGLES, ind_num, GL_UNSIGNED_INT, nullptr, count);
  else glDrawArraysInstanced(GL_TRIANGLES, 0, pos_num, count);
  GL_CHECK_ERRORS;
  GL_CHECK( glBindVertexArray(0) );
}

PrimitiveMesh::~PrimitiveMesh()
{
  if (vao)
    GL_CHECK( glDeleteVertexArrays(1, &vao) );
}