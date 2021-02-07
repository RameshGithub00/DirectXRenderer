#pragma once

#include <vector>
#include "engine/MathsWrapper.h"
#include <map>
#include <string>

namespace core {

    class Material;
    class Mesh;

    class ModelImporter{
      
    public:
        ModelImporter(){}

    public:
        
        typedef Vec3d Normal;
        struct MeshInfo
        {
            MeshInfo(Mesh* _mesh, Material* _mat)
            {
                mesh = _mesh;
                material = _mat;
            }

            Mesh* mesh;
            Material* material;
        };

    public:

        Vec3d m_offset;
        bool loadFromFile(std::string path,Mesh**,Material**);
        Material* loadMaterialFromFile(std::string path);

    private: 
        typedef std::map<std::string, MeshInfo*> MeshLookupMap;
        static MeshLookupMap lookupMesh;
        bool assimpImport(std::string path,Mesh**,Material**);
    };
}

