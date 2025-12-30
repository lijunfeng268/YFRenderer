#pragma once
#include<string>
#include"math/VectorBase.h"
#include<vector>

class Model
{
public:
    Model(const std::string& filename);
    Int32 nfaces() const { return Faces.size(); }
//private:
    std::vector<Vector3> Position;
    std::vector<Vector3> Normal;
    std::vector<Vector2> UV;
    std::vector<OBJFace> Faces;
};
