#pragma once
#include <string.h>

#define USE_DIRECX_11
#define USE_INTRINSIC

#if defined (USE_DIRECX_11)
#include <DirectXMath.h>
typedef DirectX::XMMATRIX mat4;
typedef DirectX::XMVECTOR vec4d;
#define vec4d_normalize(V1) DirectX::XMVector4Normalize(V1)
#define vec4d_add(V1, V2) DirectX::XMVectorAdd(V1,V2) 
#define vec4d_sub(V1, V2) DirectX::XMVectorSubtract(V1,V2) 
#define vec4d_dot(V1, V2) DirectX::XMVector4Dot(V1,V2);
#define vec4d_cross(V1, V2) DirectX::XMVector4Cross(V1,V2);
#define vec4d_scale(V1, SCALE) DirectX::XMVectorScale(V1,SCALE);
#endif


struct Vec2d
{

    Vec2d()
    {
        x = y = 0;
    }

    float x, y;
    Vec2d(float _x, float _y) :x(_x), y(_y) 
    {
    }

    Vec2d operator+(Vec2d& rhs)
    {
        return Vec2d(x + rhs.x, y + rhs.y);
    }

    Vec2d operator-(Vec2d& rhs)
    {
        return Vec2d(x - rhs.x, y - rhs.y);
    }

    Vec2d operator*(int _scale)//scaling
    {
        return Vec2d(x * _scale, y *_scale);
    }

    float dot(Vec2d& rhs)
    {
        return x*rhs.x + y*rhs.y;
    }

    const char* debugStr()
    {
        char* msg = new char[100];
        sprintf_s(msg, 0, "X: %f, Y: %f", x, y);
        return msg;
    }
};

struct Vec3d : public Vec2d
{
    Vec3d()
    {
        Vec2d();
        z = 0;
    }

    explicit Vec3d(float x, float y, float z)
    {
        Vec2d::x = x;
        Vec2d::y = y;
        this->z = z;
    }

    float z;

    Vec3d operator+(Vec3d& rhs)
    {
        Vec3d result;
        result.x = x + rhs.x;
        result.y = y + rhs.y;
        result.z = z + rhs.z;
        return result;
    }


    Vec3d& operator*(float scale)
    {
        x = x*scale;
        y = y*scale;
        z = z*scale;
        return *this;
    }
};

struct Vec4d : public Vec3d
{

    Vec4d()
    {
        Vec3d();
        w = 0;
    }

    union{
        float x, y, z;
        float w;
    
#if defined(USE_DIRECX_11) && defined(USE_INTRINSIC)
        DirectX::XMVECTOR intrinsic;
#endif
    };
};
