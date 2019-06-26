
#include "TextureManager.h"
#include "engine/XPlatform.h"
#include <D3DX11tex.h>
#include "renderer/Renderer.h"

using namespace std;

namespace core
{
    TextureManager::TextureManager()
    {
    }


    TextureManager::~TextureManager()
    {
    }

    TextureEntry* TextureManager::getTextureEntry(string path)
    {
        TextureDatabase::iterator it = m_database.find(path);
        if (it != m_database.end())
        {
            return it->second;
        }
        return loadTexture(path);
    }

    TextureEntry* TextureManager::loadTexture(string path)
    {
        TextureEntry* entry = new TextureEntry();
        HRESULT result = D3DX11CreateShaderResourceViewFromFile(RENDERER->getDevice(), path.c_str(), NULL, NULL, &entry->m_texture, NULL);

        CD3D11_SAMPLER_DESC desc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.MipLODBias = 1;

        ID3D11SamplerState* samplerState;
        result = RENDERER->getDevice()->CreateSamplerState(&desc, &entry->m_samplerState);

        if (result == S_OK)
        {
            m_database.insert(make_pair(path, entry));
        }
        return entry;
    }
}