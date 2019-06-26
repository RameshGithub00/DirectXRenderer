#pragma once
#include "engine/XPlatform.h"

namespace core {

    class Renderer;

    class alignas(16) Camera
    {
    public:
        Camera();
        virtual ~Camera();

        void setPosition(vec4d _pos) { m_position = _pos; }

        void setPosition(float x = 0, float y = 0, float z = 0, float w = 1)
        {
            vec4d move;

            move.m128_f32[0] = x;
            move.m128_f32[1] = y;
            move.m128_f32[2] = z;
            move.m128_f32[3] = w;
            setPosition(move);
        }

        vec4d getPosition() { return m_position; }

        void setUp(vec4d _up) { m_up = _up; }
        vec4d getUp() { return m_up; }

        void setForward(vec4d _dir) { m_forward = _dir; }
        vec4d getForward() { return m_forward; }

        void lookAt(vec4d _pos)
        {
            m_lookAt = _pos;
        }

        void lookAt(float x = 0, float y = 0, float z = 0, float w = 1)
        {
            vec4d move;

            move.m128_f32[0] = x;
            move.m128_f32[1] = y;
            move.m128_f32[2] = z;
            move.m128_f32[3] = w;
            lookAt(move);
        }

        void render(Renderer*);
        void moveForward()
        {
            vec4d move;

            move.m128_f32[0] = 0;
            move.m128_f32[1] = 0;
            move.m128_f32[2] = 0.05f;
            move.m128_f32[3] = 0;

            m_position = vec4d_add(m_position, move);
            m_lookAt = vec4d_add(m_position, m_forward);
        }

        void Update();

    private:
        mat4 m_viewMatrix;
        mat4 m_perspectiveMatrix;
        mat4 m_orthoMatrix;

        vec4d m_position;
        vec4d m_up;
        vec4d m_forward;
        vec4d m_right;
        vec4d m_lookAt;

        float m_fov;
        float m_near;
        float m_far;
        float m_aspectRatio;

    };
}

