#include "Mesh.h"
#include "HelperGL.h"

Mesh *CreateSimpleTriangleMesh(string name)
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

  return new Mesh(pos, norm, texc, ind, -1, name);
}



Mesh::Mesh(const vector<float> &positions,
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
    //передаем в шейдерную программу атрибут координат вершин
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, vboVertices) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GL_FLOAT), positions.data(), GL_STATIC_DRAW) );
    GL_CHECK( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid *)0) );
    GL_CHECK( glEnableVertexAttribArray(0) );

    //передаем в шейдерную программу атрибут нормалей
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, vboNormals) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GL_FLOAT), normals.data(), GL_STATIC_DRAW) );
    GL_CHECK( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid *)0) );
    GL_CHECK( glEnableVertexAttribArray(1) );

    //передаем в шейдерную программу атрибут текстурных координат
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(GL_FLOAT), texcoords.data(), GL_STATIC_DRAW) );
    GL_CHECK( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid *)0) );
    GL_CHECK( glEnableVertexAttribArray(2) );

    //передаем в шейдерную программу индексы
    GL_CHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices) );
    GL_CHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW) );
  }
  GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, 0) );
  GL_CHECK( glBindVertexArray(0) );

  material_id = mat_id;
}

string Mesh::getName()
{
  return name;
}

void Mesh::draw()
{
  GL_CHECK( glBindVertexArray(vao) );
  GL_CHECK( glDrawElements(GL_TRIANGLES, ind_num, GL_UNSIGNED_INT, nullptr) );
  GL_CHECK( glBindVertexArray(0) );
}

Mesh::~Mesh()
{
  glDeleteVertexArrays(1, &vao);
}