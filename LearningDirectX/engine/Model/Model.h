#pragma once
#include "engine/XPlatform.h"
#include <string>

namespace core 
{
    class Mesh;
    class Material;
    class Terrain;

    class Transform
    {
        void setOffset(float offset);
        void setPosition(float x = 0, float y = 0, float z = 0, float w = 1)
        {
            m_position.m128_f32[0] = x;
            m_position.m128_f32[1] = y;
            m_position.m128_f32[2] = z;
            m_position.m128_f32[3] = w;
        }

        void translate(float x = 0, float y = 0, float z = 0, float w = 0)
        {
            m_position.m128_f32[0] += x;
            m_position.m128_f32[1] += y;
            m_position.m128_f32[2] += z;
            m_position.m128_f32[3] += w;
        }

        float m_scale;
        float m_rotX;
        float m_rotY;
        union
        {
            vec4d m_position;
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
        };
        float m_angle = 0.0f;


        virtual void setup();
        virtual void update();
        virtual void draw();

        void scale(float scale);
        void rotateX(float value);
        void rotateY(float value);
    };

    class PrimitiveRect
    {
        Mesh* m_Mesh;
        Material * m_Material;

        void setPosition(float x = 0, float y = 0, float z = 0, float w = 1)
        {
            m_position.m128_f32[0] = x;
            m_position.m128_f32[1] = y;
            m_position.m128_f32[2] = z;
            m_position.m128_f32[3] = w;
        }

        void translate(float x = 0, float y = 0, float z = 0, float w = 0)
        {
            m_position.m128_f32[0] += x;
            m_position.m128_f32[1] += y;
            m_position.m128_f32[2] += z;
            m_position.m128_f32[3] += w;
        }


        virtual void setup();
        virtual void update();
        virtual void draw();

        void scale(float scale);
        void rotateX(float value);
        void rotateY(float value);
    };

    class Model
    {

        friend class Terrain;

        const std::string m_modelPath;

        Mesh* m_mesh;
        Material * m_material;

    public:
        Model(const char*);
        ~Model(void);

        Material* GetMaterial() { return m_material; }
        void load();
    };
}

