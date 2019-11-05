#include "Primitives.h"
#include "HelperGL.h"

PrimitiveMesh *CreateSimpleTriangleMesh(string name)
{
  vector<float> pos{
      -1, 1, -0.5, 1,
      -1, -1, -0.5, 1,
      1, -1, -0.5, 1};

  vector<float> norm{
      0, 0, 1, 1,
      0, 0, 1, 1,
      0, 0, 1, 1};

  vector<float> texc{
      0, 1,
      0, 0,
      1, 0};

  vector<uint32_t> ind{
      0, 1, 2};

  return new PrimitiveMesh(pos, norm, texc, ind, -1, name);
}



PrimitiveMesh::PrimitiveMesh(const vector<float> &positions,
                             const vector<float> &normals,
                             const vector<float> &texcoords,
                             const vector<uint32_t> &indices,
                             size_t mat_id,
                             string n)
{
  name = n;

  ind_num = indices.size();

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
    GL_CHECK( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid *)0) );
    GL_CHECK( glEnableVertexAttribArray(0) );

    // Pass Vertex's Normals to shader
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, vboNormals) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GL_FLOAT), normals.data(), GL_STATIC_DRAW) );
    GL_CHECK( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid *)0) );
    GL_CHECK( glEnableVertexAttribArray(1) );

    // Pass Vertex's Texture Coordinates to shader
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(GL_FLOAT), texcoords.data(), GL_STATIC_DRAW) );
    GL_CHECK( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid *)0) );
    GL_CHECK( glEnableVertexAttribArray(2) );

    // Pass Indexes to shader
    GL_CHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices) );
    GL_CHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW) );
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
  GL_CHECK( glDrawElements(GL_TRIANGLES, ind_num, GL_UNSIGNED_INT, nullptr) );
  GL_CHECK( glBindVertexArray(0) );
}

PrimitiveMesh::~PrimitiveMesh()
{
  if (vao)
    GL_CHECK( glDeleteVertexArrays(1, &vao) );
}