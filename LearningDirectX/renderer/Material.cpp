
#include "ShaderManager.h"
#include "Material.h"
#include "renderer/Renderer.h"
#include "engine/Model/Mesh.h"

namespace core {
    using namespace std;

    Material::~Material()
    {
    }


    bool Material::create(MaterialDefines& materialDefines)
    {
        materialDefines.shaderName;
        materialDefines.texturePath0;
        m_shader = SHADER_MANAGER->loadShader(materialDefines.shaderName);
        m_textureEntry = TEXTUREMANAGER->getTextureEntry(materialDefines.texturePath0);
        return true;
    }

    void Material::apply()
    {
        RENDERER->getDeviceContext()->PSSetShaderResources(0, 1, &m_textureEntry->m_texture);
        RENDERER->getDeviceContext()->PSSetSamplers(0, 1, &m_textureEntry->m_samplerState);
        m_shader->load();
    }
}