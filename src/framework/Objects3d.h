#pragma once

#include <string>
#include <vector>

class Mesh
{
public:
  virtual ~Mesh() {}
  virtual const std::string &GetName() const = 0;
  virtual void Draw() = 0;
};

class Model
{
public:
  virtual ~Model() {}
  virtual void Draw() = 0;
  virtual const std::vector<Mesh*> GetMeshes() const = 0;
};