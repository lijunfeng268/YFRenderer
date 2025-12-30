#include"../include/model.h"
#include<fstream>
#include<iostream>
#include <sstream>
#include"../include/DataType.h"

Model::Model(const std::string& filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::cout<< "Cannot open model file " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string head;
        iss >> head;
        if(head == "v") {
            Vector3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            Position.push_back(pos);
        } 
        else if(head == "vn") {
            Vector3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            Normal.push_back(normal);
        } 
        else if(head == "vt") {
            Vector2 uv;
            iss >> uv.x >> uv.y;
            UV.push_back(uv);
        } 
        else if(head == "f") {
            OBJFace face;
            std::string vertexStr;
            while (iss >> vertexStr) {
                std::istringstream viss(vertexStr);
                std::string idxStr;
                Int32 idxCount = 0;
                Int32 vIdx = -1, uvIdx = -1, nIdx = -1;
                while (std::getline(viss, idxStr, '/')) {
                    if (!idxStr.empty()) {
                        Int32 idx = std::stoi(idxStr);
                        if (idxCount == 0) vIdx = idx - 1; // OBJ索引从1开始
                        else if (idxCount == 1) uvIdx = idx - 1;
                        else if (idxCount == 2) nIdx = idx - 1;
                    }
                    idxCount++;
                }
                face.vtxIdx.push_back(vIdx);
                face.uvIdx.push_back(uvIdx);
                face.normalIdx.push_back(nIdx);
            }
            Faces.push_back(face);
        }
    }
}