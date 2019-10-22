#include "ImportSceneObject.h"

#include <functional>
#include <iostream>
#include <sstream>

using namespace std;


vector<string> split(string str, const string &dev) {
    vector<string> ret;
    size_t dev_p = 0;

    while ((dev_p = str.find(dev)) != string::npos) {
        ret.push_back(str.substr(0, dev_p));
        str = str.substr(str.find(dev) + 1);
    }

    ret.push_back(str);

    return ret;
}

pair<XMLNode*, string> WalkHeirarchy(XMLNode *node, string name) {
    auto l = split(name, "/");
    name = l.back();
    l.pop_back();

    for (auto &v : l) {
        node = node->FirstChildElement(v.c_str());
    }

    return make_pair(node, name);
}

void XMLIterChildren(XMLNode *node, string name, function<void (XMLElement*)> cb) {
    auto p = WalkHeirarchy(node, name);

    node = p.first;
    name = p.second;

    for (auto child = node->FirstChildElement(name.c_str()); child;
              child = child->NextSiblingElement(name.c_str())) {
        cb(child);
    }
}

float4x4 ParseFloat4(const string &s) {
    stringstream ss(s);

    float4x4 m;
    
    for (int i = 0; i < 4; i++) {
        ss >> m.row[i].x;
        ss >> m.row[i].y;
        ss >> m.row[i].z;
        ss >> m.row[i].w;
    }
    return transpose4x4(m);
}

struct TextureData {
    int32_t width, height;
    vector<unsigned char> data;
};

TextureData LoadTexture(string filename) {
    TextureData ret;
    
    ifstream input(filename, std::ios::binary);
    input.seekg(0, ios::end);
    size_t s = input.tellg();
    input.seekg(0, ios::beg);

    input.read((char*)&ret.width , sizeof(ret.width));
    input.read((char*)&ret.height, sizeof(ret.height));
    s -= 8;

    ret.data.resize(s);
    input.read((char *)ret.data.data(), ret.data.size());

    return move(ret);
}

map<size_t, GLuint> LoadTextures(XMLNode *node) {
    map<size_t, GLuint> ret;

    XMLIterChildren(node, "textures_lib/texture", [&ret] (auto v) {
        GLuint id;
        
        glGenTextures(1, &id);
        ret[v->UnsignedAttribute("id")] = id;
        
        glBindTexture(GL_TEXTURE_2D, id);
        auto texdata = LoadTexture(string("assets/") + v->Attribute("loc"));

        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, texdata.width, texdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata.data.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    });

    return ret;
}

ErrCodes ImportSceneObjectFromFile(const string &filename,
                                   vector<unique_ptr<Mesh>> &scene,
                                   map<uint32_t, Material> &material) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        return FAILED_TO_LOAD;
    }

    auto textures = LoadTextures(&doc);
    
    XMLIterChildren(&doc, "materials_lib/material", [&textures, &material](auto v) {
        if (v->FirstChildElement("diffuse")) {
            auto tex = v->FirstChildElement("diffuse")->FirstChildElement("color")->FirstChildElement("texture");
        
            uint32_t id = tex ? textures[tex->UnsignedAttribute("id")] : -1;

            material[v->UnsignedAttribute("id")].tex_id = id;
        } else {
            material[v->UnsignedAttribute("id")].tex_id = -1;
        }
    });

    cout << "Reading texture library {" << endl;

    XMLIterChildren(&doc, "textures_lib/texture", [] (auto tex) {
        cout << "    {" << endl;
        cout << "        Tex.id   = " << tex->UnsignedAttribute("id") << endl;
        cout << "        Tex.name = " << tex->Attribute("name") << endl;
        cout << "    }" << endl;
    });

    cout << "}" << endl;
    cout << "Reading geometry library {" << endl;

    map<size_t, size_t> meshid_to_sceneid;

    XMLIterChildren(&doc, "geometry_lib/mesh", [&scene, &meshid_to_sceneid](auto mesh) {
        cout << "    {" << endl;
        cout << "        Mesh.id       = " << mesh->UnsignedAttribute("id") << endl;
        cout << "        Mesh.name     = " << mesh->Attribute("name") << endl;
        cout << "        Mesh.filename = " << mesh->Attribute("loc") << endl;
        cout << "    }" << endl;

        HydraGeomData gd;
        gd.read(string("assets/") + mesh->Attribute("loc"));

        uint32_t vert_num      = gd.getVerticesNumber();
        const float *positions = gd.getVertexPositionsFloat4Array();
        const float *normals   = gd.getVertexNormalsFloat4Array();
        const float *texcoords = gd.getVertexTexcoordFloat2Array();
        const uint32_t *matids = gd.getTriangleMaterialIndicesArray();

        uint32_t ind_num        = gd.getIndicesNumber();
        const uint32_t* indices = gd.getTriangleVertexIndicesArray();


        unique_ptr<Mesh> m(new Mesh(vector<float>(positions, positions + 4 * vert_num),
            vector<float>(normals, normals + 4 * vert_num),
            vector<float>(texcoords, texcoords + 2 * vert_num),
            vector<uint32_t>(indices, indices + ind_num),
            matids[0],
            mesh->Attribute("name")));
        scene.push_back(move(m));
        meshid_to_sceneid[mesh->UnsignedAttribute("id")] = scene.size() - 1;
    });

    XMLIterChildren(&doc , "scenes/scene/instance", [&scene, &meshid_to_sceneid](auto inst) {
        cout << inst->Attribute("matrix") << endl;
        scene[meshid_to_sceneid[inst->UnsignedAttribute("mesh_id")]]->model = ParseFloat4(inst->Attribute("matrix"));
    });

    cout << "}" << endl;
}