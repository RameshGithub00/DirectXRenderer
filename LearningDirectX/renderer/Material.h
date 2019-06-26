#pragma once

#include "engine/MathsWrapper.h"
#include "engine/TextureManager.h"
#include <vector>
#include <string>

namespace core {

    class Shader;
    class Renderer;
    class Mesh;

    class alignas(16) Material
    {

    public:
        Material() 
        {
            m_transparent = false;
            m_mesh = nullptr;
            m_shader = nullptr;
            m_diffuseTextures.clear();
            //m_normalMaps.clear();
        }
        ~Material();

        struct MaterialDefines
        {
            std::string texturePath0;
            std::string texturePath1;
            std::string texturePath2;
            std::string texturePath3;
            
            std::vector<Vec2d> uv0;
            std::vector<Vec2d> uv1;
            std::vector<Vec2d> uv2;
            std::vector<Vec2d> uv3;

            std::string shaderName;
            MaterialDefines()
            {
                shaderName = "shaders/compiled/default";
                texturePath0 = "textures/default.png";
            }
        };

        TextureEntry* m_textureEntry;

        bool create(MaterialDefines&);
        void apply();
        Shader* GetShader() { return m_shader; }
    private:
        
        std::vector<Texture2D*> m_diffuseTextures;
        //std::vector<aPtr(NormalMap)> m_normalMaps;

        vec4d m_diffuseColor;
        Shader* m_shader;
        Mesh*   m_mesh;
        bool    m_transparent;
    };
}
