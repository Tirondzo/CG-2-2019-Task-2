#include "LoaderGLTF.h"
#include "HelperGL.h"
#include <memory>
#include <iostream>
#include <cassert>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <TinyGLTF/tiny_gltf.h>

std::unique_ptr<tinygltf::TinyGLTF> loader;

// Based on this example
// https://github.com/syoyo/tinygltf/blob/master/examples/glview/glview.cc


static std::string GetFilePathExtension(const std::string &FileName) {
  if (FileName.find_last_of(".") != std::string::npos)
    return FileName.substr(FileName.find_last_of(".") + 1);
  return "";
}

static size_t ComponentTypeByteSize(int type) {
  switch (type) {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
    case TINYGLTF_COMPONENT_TYPE_BYTE:
      return sizeof(char);
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
    case TINYGLTF_COMPONENT_TYPE_SHORT:
      return sizeof(short);
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
    case TINYGLTF_COMPONENT_TYPE_INT:
      return sizeof(int);
    case TINYGLTF_COMPONENT_TYPE_FLOAT:
      return sizeof(float);
    case TINYGLTF_COMPONENT_TYPE_DOUBLE:
      return sizeof(double);
    default:
      return 0;
  }
}

GLTFModel::GLTFModel(const char *file)
{
  if (!loader)
    loader = std::make_unique<tinygltf::TinyGLTF>();
  std::string err;
  std::string warn;

  std::string input_filename(file);
  std::string ext = GetFilePathExtension(input_filename);
  bool ret = false;
  if (ext.compare("glb") == 0) {
    // assume binary glTF.
    ret = loader->LoadBinaryFromFile(&model, &err, &warn, input_filename);
  } else {
    // assume ascii glTF.
    ret = loader->LoadASCIIFromFile(&model, &err, &warn, input_filename);
  }

  glGenVertexArrays(1, &vao);

  // Buffer
  {
    for (size_t i = 0; i < model.bufferViews.size(); i++) {
      const tinygltf::BufferView &bufferView = model.bufferViews[i];
      if (bufferView.target == 0) {
        std::cout << "WARN: bufferView.target is zero" << std::endl;
        continue;  // Unsupported bufferView.
      }

      int sparse_accessor = -1;
      for (size_t a_i = 0; a_i < model.accessors.size(); ++a_i) {
        const auto &accessor = model.accessors[a_i];
        if (accessor.bufferView == i) {
          std::cout << i << " is used by accessor " << a_i << std::endl;
          if (accessor.sparse.isSparse) {
            std::cout
                << "WARN: this bufferView has at least one sparse accessor to "
                   "it. We are going to load the data as patched by this "
                   "sparse accessor, not the original data"
                << std::endl;
            sparse_accessor = a_i;
            break;
          }
        }
      }

      const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
      GLBufferState state;
      glGenBuffers(1, &state.vb);
      glBindBuffer(bufferView.target, state.vb);
      std::cout << "buffer.size= " << buffer.data.size()
                << ", byteOffset = " << bufferView.byteOffset << std::endl;

      if (sparse_accessor < 0)
        glBufferData(bufferView.target, bufferView.byteLength,
                     &buffer.data.at(0) + bufferView.byteOffset,
                     GL_STATIC_DRAW);
      else {
        const auto accessor = model.accessors[sparse_accessor];
        // copy the buffer to a temporary one for sparse patching
        unsigned char *tmp_buffer = new unsigned char[bufferView.byteLength];
        memcpy(tmp_buffer, buffer.data.data() + bufferView.byteOffset,
               bufferView.byteLength);

        const size_t size_of_object_in_buffer =
            ComponentTypeByteSize(accessor.componentType);
        const size_t size_of_sparse_indices =
            ComponentTypeByteSize(accessor.sparse.indices.componentType);

        const auto &indices_buffer_view =
            model.bufferViews[accessor.sparse.indices.bufferView];
        const auto &indices_buffer = model.buffers[indices_buffer_view.buffer];

        const auto &values_buffer_view =
            model.bufferViews[accessor.sparse.values.bufferView];
        const auto &values_buffer = model.buffers[values_buffer_view.buffer];

        for (size_t sparse_index = 0; sparse_index < accessor.sparse.count;
             ++sparse_index) {
          int index = 0;
          // std::cout << "accessor.sparse.indices.componentType = " <<
          // accessor.sparse.indices.componentType << std::endl;
          switch (accessor.sparse.indices.componentType) {
            case TINYGLTF_COMPONENT_TYPE_BYTE:
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
              index = (int)*(
                  unsigned char *)(indices_buffer.data.data() +
                                   indices_buffer_view.byteOffset +
                                   accessor.sparse.indices.byteOffset +
                                   (sparse_index * size_of_sparse_indices));
              break;
            case TINYGLTF_COMPONENT_TYPE_SHORT:
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
              index = (int)*(
                  unsigned short *)(indices_buffer.data.data() +
                                    indices_buffer_view.byteOffset +
                                    accessor.sparse.indices.byteOffset +
                                    (sparse_index * size_of_sparse_indices));
              break;
            case TINYGLTF_COMPONENT_TYPE_INT:
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
              index = (int)*(
                  unsigned int *)(indices_buffer.data.data() +
                                  indices_buffer_view.byteOffset +
                                  accessor.sparse.indices.byteOffset +
                                  (sparse_index * size_of_sparse_indices));
              break;
          }
          std::cout << "updating sparse data at index  : " << index
                    << std::endl;
          // index is now the target of the sparse index to patch in
          const unsigned char *read_from =
              values_buffer.data.data() +
              (values_buffer_view.byteOffset +
               accessor.sparse.values.byteOffset) +
              (sparse_index * (size_of_object_in_buffer * accessor.type));

          /*
          std::cout << ((float*)read_from)[0] << "\n";
          std::cout << ((float*)read_from)[1] << "\n";
          std::cout << ((float*)read_from)[2] << "\n";
          */

          unsigned char *write_to =
              tmp_buffer + index * (size_of_object_in_buffer * accessor.type);

          memcpy(write_to, read_from, size_of_object_in_buffer * accessor.type);
        }

        // debug:
        /*for(size_t p = 0; p < bufferView.byteLength/sizeof(float); p++)
        {
          float* b = (float*)tmp_buffer;
          std::cout << "modified_buffer [" << p << "] = " << b[p] << '\n';
        }*/

        glBufferData(bufferView.target, bufferView.byteLength, tmp_buffer,
                     GL_STATIC_DRAW);
        delete[] tmp_buffer;
      }
      glBindBuffer(bufferView.target, 0);

      gBufferState[i] = state;
    }
  }


  // glUseProgram(progId);
  // GLint vtloc = glGetAttribLocation(progId, "in_vertex");
  // GLint nrmloc = glGetAttribLocation(progId, "in_normal");
  // GLint uvloc = glGetAttribLocation(progId, "in_texcoord");
  GLint vtloc = 0;
  GLint nrmloc = 1;
  GLint uvloc = 2;

  // GLint diffuseTexLoc = glGetUniformLocation(progId, "diffuseTex");
  // GLint isCurvesLoc = glGetUniformLocation(progId, "uIsCurves");
  gGLProgramState.attribs["POSITION"] = vtloc;
  gGLProgramState.attribs["NORMAL"] = nrmloc;
  gGLProgramState.attribs["TEXCOORD_0"] = uvloc;
  // gGLProgramState.uniforms["diffuseTex"] = diffuseTexLoc;
  // gGLProgramState.uniforms["isCurvesLoc"] = isCurvesLoc;


  // If the glTF asset has at least one scene, and doesn't define a default one
  // just show the first one we can find
  assert(model.scenes.size() > 0);
  int scene_to_display = model.defaultScene > -1 ? model.defaultScene : 0;
  const tinygltf::Scene &scene = model.scenes[scene_to_display];
  for (size_t i = 0; i < scene.nodes.size(); i++) {
    tinygltf::Node &node = model.nodes[scene.nodes[i]];
    assert(node.children.size() == 0); // Multi-nodes is not supported
    if (node.mesh < 0) continue;
    // If you need this, you can add recursion here
    tinygltf::Mesh &mesh = model.meshes[node.mesh];
    meshes.emplace_back(new GLTFMesh(this, node.mesh, mesh.name));
  }
}

void GLTFModel::Draw()
{
  for (Mesh* m : meshes)
    m->Draw();
}

GLTFModel::~GLTFModel()
{
  for (Mesh* m : meshes)
    delete m;
  meshes.clear();
}

const std::vector<Mesh *> GLTFModel::GetMeshes() const
{
  return meshes;
}


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void GLTFMesh::Draw()
{
  tinygltf::Model &model = parent->model;
  tinygltf::Mesh &mesh = model.meshes[mesh_id];
  std::map<int, GLBufferState> &gBufferState = parent->gBufferState;
  GLProgramState &gGLProgramState = parent->gGLProgramState;

  glBindVertexArray(parent->vao);

  for (size_t i = 0; i < mesh.primitives.size(); i++) {
    const tinygltf::Primitive &primitive = mesh.primitives[i];

    if (primitive.indices < 0) return;

    // Assume TEXTURE_2D target for the texture object.
    // glBindTexture(GL_TEXTURE_2D, gMeshState[mesh.name].diffuseTex[i]);

    std::map<std::string, int>::const_iterator it(primitive.attributes.begin());
    std::map<std::string, int>::const_iterator itEnd(
        primitive.attributes.end());

    for (; it != itEnd; it++) {
      assert(it->second >= 0);
      const tinygltf::Accessor &accessor = model.accessors[it->second];
      int vb = gBufferState[accessor.bufferView].vb;
      glBindBuffer(GL_ARRAY_BUFFER, vb);
      GL_CHECK_ERRORS;
      int size = 1;
      if (accessor.type == TINYGLTF_TYPE_SCALAR) {
        size = 1;
      } else if (accessor.type == TINYGLTF_TYPE_VEC2) {
        size = 2;
      } else if (accessor.type == TINYGLTF_TYPE_VEC3) {
        size = 3;
      } else if (accessor.type == TINYGLTF_TYPE_VEC4) {
        size = 4;
      } else {
        assert(0);
      }
      // it->first would be "POSITION", "NORMAL", "TEXCOORD_0", ...
      if ((it->first.compare("POSITION") == 0) ||
          (it->first.compare("NORMAL") == 0) ||
          (it->first.compare("TEXCOORD_0") == 0)) {
        if (gGLProgramState.attribs[it->first] >= 0) {
          // Compute byteStride from Accessor + BufferView combination.
          int byteStride =
              accessor.ByteStride(model.bufferViews[accessor.bufferView]);
          assert(byteStride != -1);
          glVertexAttribPointer(gGLProgramState.attribs[it->first], size,
                                accessor.componentType,
                                accessor.normalized ? GL_TRUE : GL_FALSE,
                                byteStride, BUFFER_OFFSET(accessor.byteOffset));
          GL_CHECK_ERRORS;
          glEnableVertexAttribArray(gGLProgramState.attribs[it->first]);
          GL_CHECK_ERRORS;
        }
      }
    }

    const tinygltf::Accessor &indexAccessor =
        model.accessors[primitive.indices];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 gBufferState[indexAccessor.bufferView].vb);
    GL_CHECK_ERRORS;
    int mode = -1;
    if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
      mode = GL_TRIANGLES;
    } else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP) {
      mode = GL_TRIANGLE_STRIP;
    } else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN) {
      mode = GL_TRIANGLE_FAN;
    } else if (primitive.mode == TINYGLTF_MODE_POINTS) {
      mode = GL_POINTS;
    } else if (primitive.mode == TINYGLTF_MODE_LINE) {
      mode = GL_LINES;
    } else if (primitive.mode == TINYGLTF_MODE_LINE_LOOP) {
      mode = GL_LINE_LOOP;
    } else {
      assert(0);
    }
    glDrawElements(mode, indexAccessor.count, indexAccessor.componentType,
                   BUFFER_OFFSET(indexAccessor.byteOffset));
    GL_CHECK_ERRORS;

    {
      std::map<std::string, int>::const_iterator it(
          primitive.attributes.begin());
      std::map<std::string, int>::const_iterator itEnd(
          primitive.attributes.end());

      for (; it != itEnd; it++) {
        if ((it->first.compare("POSITION") == 0) ||
            (it->first.compare("NORMAL") == 0) ||
            (it->first.compare("TEXCOORD_0") == 0)) {
          if (gGLProgramState.attribs[it->first] >= 0) {
            glDisableVertexAttribArray(gGLProgramState.attribs[it->first]);
          }
        }
      }
    }
  }
  glBindVertexArray(0);
}