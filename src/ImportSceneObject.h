#pragma once

#include "Hydra/HydraVSGFExport.h"
#include "TinyXML2/tinyxml2.h"

#include "mesh.h"

#include <string>
#include <memory>
#include <map>

using std::map;
using std::vector;
using std::unique_ptr;

using namespace tinyxml2;

using std::string;

struct Material {
    GLuint tex_id;
};

enum ErrCodes {
    FAILED_TO_LOAD
};

ErrCodes ImportSceneObjectFromFile(const string &filename, vector<unique_ptr<Mesh>> &scene, map<uint32_t, Material> &material);