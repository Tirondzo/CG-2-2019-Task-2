#include "Objects3d.h"
#include <TinyGLTF/tiny_gltf.h>
#include <vector>
#include <glad/glad.h>
#include <map>

typedef struct {
  GLuint vb;
} GLBufferState;

typedef struct {
  std::vector<GLuint> diffuseTex;  // for each primitive in mesh
} GLMeshState;

typedef struct {
  std::map<std::string, GLint> attribs;
  std::map<std::string, GLint> uniforms;
} GLProgramState;

typedef struct {
  GLuint vb;     // vertex buffer
  size_t count;  // byte count
} GLCurvesState;


class GLTFModel : public Model
{
  std::map<int, GLBufferState> gBufferState;
  std::map<std::string, GLMeshState> gMeshState;
  std::map<int, GLCurvesState> gCurvesMesh;
  GLProgramState gGLProgramState;
  tinygltf::Model model;
  std::vector<Mesh *> meshes;
  GLuint vao;
  friend class GLTFMesh;
public:
  GLTFModel(const char *file);
  GLTFModel(const GLTFModel&) = delete;
  ~GLTFModel();

  const std::vector<Mesh*> GetMeshes() const override;
  void Draw() override;
};

class GLTFMesh : public Mesh
{
  GLTFModel *parent;
  int mesh_id;
  std::string name;
  GLTFMesh(GLTFModel *parent, int mesh_id, std::string name) : parent(parent), mesh_id(mesh_id), name(name) {}
  friend GLTFModel;
public:
  GLTFMesh(const GLTFMesh&) = delete;
  const std::string &GetName() const override { return name; };
  void Draw() override;
};