#pragma once
#pragma once

#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <string>
#include <vector>
#include "engine/MathsWrapper.h"
#include "engine/XPlatform.h"

#include "Model.h"

namespace core {

    class alignas(16)Mesh
    {

    public:
        struct Vertex {

            Vertex()
            {
                x = y = z = w = 0;
                nx = ny = nz = 0;
                r = g = b = 0;
            }

            union
            {
                Vec3d position;
                struct
                {
                    float x, y, z, w;
                };
            };


            union
            {
                Vec3d normal;
                struct
                {
                    float nx;
                    float ny;
                    float nz;
                };
            };

            union
            {
                Vec3d color;
                struct
                {
                    float r;
                    float g;
                    float b;
                };
            };
        };
        struct InstanceData {
            Vec3d position;
        };

    private:
        Model* m_pModel;

        ID3D11Buffer* m_pVertexBuffer;
        ID3D11Buffer* m_pConstantBuffer;
        ID3D11Buffer* m_pIndexBuffer;
        ID3D11Buffer* m_pInstanceBuffer;

        __declspec(align(16))
            struct VS_CONSTANT_BUFFER
        {
            float fTime;
        };

        VS_CONSTANT_BUFFER vs_constant_buffer;


    public:

        void SetModel(Model* model) { m_pModel = model; }

        typedef std::vector<Mesh::Vertex> VertexList;
        VertexList m_vertices;

        typedef Vec3d Normal;

        std::vector<Normal> m_normals;

        InstanceData* m_pInstanceData;

        struct Face {
            int v[4];
            int t[4];
            int n[4];
        };
        std::vector<Face> m_faces;

        struct FaceVertex {
            //break quads in 2 triangles
            i32 tri[3];

        };

        std::vector<FaceVertex> m_index;

    public:
        Mesh();
        ~Mesh();

        float m_offset;

        void setup();
        void draw();
        float getOffset() { return m_offset; };
        void cleanUp();

        std::string descStr() 
        {
        }
    };
}

