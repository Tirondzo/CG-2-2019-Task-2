#pragma once

#include <string>
#include <vector>
#include <LiteMath.h>
using LiteMath::float4x4;

class Mesh
{
  float4x4 identity;
public:
  virtual ~Mesh() {}
  virtual const std::string &GetName() const = 0;
  virtual const float4x4 &GetTransform() const { return identity; };
  virtual void Draw() = 0;
};

class Model
{
public:
  virtual ~Model() {}
  virtual const std::vector<Mesh*> &GetMeshes() const = 0;
  virtual void Draw() = 0;
};