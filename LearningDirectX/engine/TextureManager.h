#pragma once
#include "framework/Singleton.h"
#include "engine/XPlatform.h"
#include <map>

namespace core 
{
    struct TextureEntry
    {
        ID3D11ShaderResourceView* m_texture;
        ID3D11SamplerState* m_samplerState;
    };

    typedef std::map<std::string, TextureEntry*> TextureDatabase;

    class TextureManager : public Singleton<TextureManager>
    {

        
        TextureDatabase m_database;

        TextureEntry* loadTexture(std::string path);

    public:
        TextureManager();
        ~TextureManager();

        TextureEntry* getTextureEntry(std::string path);
    };
    #define TEXTUREMANAGER TextureManager::getPtr()
}