#include "ModelImporter.h"



#include "engine/model/Mesh.h"
#include <stdio.h>
#include <DirectXMath.h>
#include "engine/XPlatform.h"
#include "renderer/Renderer.h"
#include "renderer/Material.h"
#include <fstream>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace core {

    using namespace std;

    ModelImporter::MeshLookupMap ModelImporter::lookupMesh;

    bool ModelImporter::assimpImport(string path, Mesh** _mesh, Material** _mat)
    {

        const aiScene* asset = aiImportFile(path.c_str(), aiProcess_MakeLeftHanded);

        const char* error = aiGetErrorString();


        if (asset->HasMaterials())
        {
            *_mat = new Material();

            Material::MaterialDefines def;
            aiString shader;
            asset->mMaterials[0]->Get("Shader", 0, 0, shader);
            if (shader.length > 0)
                def.shaderName = shader.C_Str();

            aiString path;
            asset->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &path);

            if (path.length > 0)
                def.shaderName = path.C_Str();

            (*_mat)->create(def);
        }

        *_mesh = new Mesh();

        Mesh::VertexList& vertices = (*_mesh)->m_vertices;
        std::vector<Normal>& normals = (*_mesh)->m_normals;
        //std::vector<Mesh::Face>& faces = (*_mesh)->faces;
        std::vector<Mesh::FaceVertex>& faces = (*_mesh)->m_index;

        for (int j = 0;j < asset->mNumMeshes;j++)
        {
            for (int i = 0;i < asset->mMeshes[j]->mNumVertices;i++)
            {
                Mesh::Vertex vertex;
                memcpy(&vertex.position, &asset->mMeshes[j]->mVertices[i], sizeof(aiVector3D));
                vertices.push_back(vertex);

                //if (asset->mMeshes[j]->HasNormals())
                //{
                //    Mesh::Normal normal;
                //    memcpy(&normal, &asset->mMeshes[j]->mNormals[i], sizeof(aiVector3D));
                //    normals.push_back(normal);
                //}
            }

            for (int i = 0;i < asset->mMeshes[j]->mNumFaces;i++)
            {
                //Mesh::Face face;
                //memcpy(&face, &asset->mMeshes[j]->mFaces[i], sizeof(aiVector3D));
                //faces.push_back(face);

                if (asset->mMeshes[j]->mFaces[i].mNumIndices == 3)
                {
                    Mesh::FaceVertex faceVertex;
                    memcpy(&faceVertex, asset->mMeshes[j]->mFaces[i].mIndices, sizeof(faceVertex));
                    faces.push_back(faceVertex);
                }
            }
        }



        //normals
        if (normals.empty())
        {
            //calculate normals
            for (int i = 0;i < faces.size();i++)
            {
                Mesh::FaceVertex triFace = faces[i];
                Mesh::Vertex& v1 = vertices[triFace.tri[0]];
                Mesh::Vertex& v2 = vertices[triFace.tri[1]];
                Mesh::Vertex& v3 = vertices[triFace.tri[2]];

                XMVECTOR arm1;
                arm1.m128_f32[0] = v2.x - v1.x;
                arm1.m128_f32[1] = v2.y - v1.y;
                arm1.m128_f32[2] = v2.z - v1.z;
                arm1.m128_f32[3] = 1;

                XMVECTOR arm2;
                arm2.m128_f32[0] = v3.x - v2.x;
                arm2.m128_f32[1] = v3.y - v2.y;
                arm2.m128_f32[2] = v3.z - v2.z;
                arm2.m128_f32[3] = 1;

                XMVECTOR cross = XMVector3Cross(arm1, arm2);

                v1.nx = cross.m128_f32[0];
                v1.ny = cross.m128_f32[1];
                v1.nz = cross.m128_f32[2];

                v2.normal = v3.normal = v1.normal;

            }
        }

        char str[100];
        OutputDebugString("\n====mesh info====");

        sprintf_s(str, "\nvertex: %d", vertices.size());
        OutputDebugString(str);

        sprintf_s(str, "\nfaces: %d", faces.size());
        OutputDebugString(str);

        sprintf_s(str, "\nindices: %d", faces.size()*sizeof(Mesh::FaceVertex)/sizeof(u32));
        OutputDebugString(str);

        OutputDebugString("\n====mesh info====");

        return true;
    }

    bool ModelImporter::loadFromFile(string path, Mesh** _mesh, Material** _mat)
    {

        MeshLookupMap::iterator it = lookupMesh.find(path);
        if (it != lookupMesh.end())
        {
            *_mesh = it->second->mesh;
            *_mat = it->second->material;
            return true;
        }

        assimpImport(path, _mesh, _mat);

        if (*_mesh != nullptr)
        {
            lookupMesh.insert(make_pair(path, new MeshInfo(*_mesh, *_mat)));
            return true;
        }

        *_mesh = new Mesh();
        *_mat = new Material();

        Mesh::VertexList& vertices = (*_mesh)->m_vertices;
        std::vector<Normal>& normals = (*_mesh)->m_normals;
        std::vector<Mesh::Face>& faces = (*_mesh)->m_faces;
        std::vector<Mesh::FaceVertex>& index = (*_mesh)->m_index;

        ifstream obj_file(path, ifstream::in);
        if (obj_file.fail())
        {
            OutputDebugString("fuck u");
        }

        vector<string> lines;

        char* line = new char[100];
        std::streamsize count = 100;


        while (obj_file.getline(line, count))
        {
            Mesh::Vertex vertex;
            vertex.w = 1.0f;

            //int result = sscanf_s(line, "v  %f  %f  %f  %f  %f  %f", &vertex.x, &vertex.y, &vertex.z,&vertex.r,&vertex.g,&vertex.b);
            //int result = sscanf_s(line, "v  %f  %f  %f  ", &vertex.x, &vertex.y, &vertex.z);

            int result = sscanf_s(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);

            if (result == 3)
            {
                vertex.position = vertex.position + m_offset;
                vertices.push_back(vertex);
            }
            else
            {

                Mesh::Face face;
                int *d = new int;
                //int result = sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &face.v[0], &face.t[0], &face.n[0], &face.v[1], &face.t[1], &face.n[1], &face.v[2], &face.t[2], &face.n[2], &face.v[3], &face.t[3], &face.n[3]);
                //int result = sscanf_s(line, "f %d//%d  %d//%d  %d//%d", &face.v[0], &face.t[0], &face.v[1], &face.t[1], &face.v[2], &face.t[2]);
                int result = sscanf_s(line, "f %d %d %d", &face.v[0], &face.v[1], &face.v[2]);

                if (result == 3)
                {
                    faces.push_back(face);
                }
                else
                {
                    Normal normal;
                    int *d = new int;
                    //int result = sscanf_s(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                    int result = sscanf_s(line, "vn %f  %f %f", &normal.x, &normal.y, &normal.z);

                    if (result == 3)
                    {
                        normals.push_back(normal);
                    }
                }
            }
        }

        for (size_t i = 0; i < faces.size(); i++)
        {
            Mesh::FaceVertex faceVert;

            faceVert.tri[0] = faces[i].v[0] - 1;
            faceVert.tri[1] = faces[i].v[2] - 1;
            faceVert.tri[2] = faces[i].v[1] - 1;

            index.push_back(faceVert);
        }

        //normals
        if (normals.empty())
        {
            //calculate normals
            for (int i = 0;i < index.size();i++)
            {
                Mesh::FaceVertex triFace = index[i];
                Mesh::Vertex& v1 = vertices[triFace.tri[0]];
                Mesh::Vertex& v2 = vertices[triFace.tri[1]];
                Mesh::Vertex& v3 = vertices[triFace.tri[2]];

                XMVECTOR arm1;
                arm1.m128_f32[0] = v2.x - v1.x;
                arm1.m128_f32[1] = v2.y - v1.y;
                arm1.m128_f32[2] = v2.z - v1.z;
                arm1.m128_f32[3] = 1;

                XMVECTOR arm2;
                arm2.m128_f32[0] = v3.x - v2.x;
                arm2.m128_f32[1] = v3.y - v2.y;
                arm2.m128_f32[2] = v3.z - v2.z;
                arm2.m128_f32[3] = 1;

                XMVECTOR cross = XMVector3Cross(arm1, arm2);

                v1.nx = cross.m128_f32[0];
                v1.ny = cross.m128_f32[1];
                v1.nz = cross.m128_f32[2];

                v2.normal = v3.normal = v1.normal;

            }
        }
        else
        {
            //use provided normals
            for (size_t i = 0; i < faces.size(); i++)
            {
                for (int j = 0;j < 3;j++)
                {
                    vertices[faces[i].v[j] - 1].nx = normals[faces[i].t[j] - 1].x;
                    vertices[faces[i].v[j] - 1].ny = normals[faces[i].t[j] - 1].y;
                    vertices[faces[i].v[j] - 1].nz = normals[faces[i].t[j] - 1].z;
                }
            }
        }

        if (vertices.size() == 0 || faces.size() == 0 || index.size() == 0)
        {
            OutputDebugString("error importing model");
            delete _mesh;
            _mesh = 0;

            delete _mat;
            _mat = 0;
            return nullptr;
        }

        char str[100];
        OutputDebugString("\n====mesh info====");

        sprintf_s(str, "\nvertex: %d", vertices.size());
        OutputDebugString(str);

        sprintf_s(str, "\nfaces: %d", faces.size());
        OutputDebugString(str);

        sprintf_s(str, "\nindices: %d", index.size());
        OutputDebugString(str);

        OutputDebugString("\n====mesh info====");

        lookupMesh.insert(make_pair(path, new MeshInfo(*_mesh, *_mat)));

        return true;
    }

    Material* ModelImporter::loadMaterialFromFile(string path)
    {
        return 0;
    }
}