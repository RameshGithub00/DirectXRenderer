#pragma once

#include "engine/XPlatform.h"
#include <map>
#include <string>


namespace core {

    class Shader
    {
        friend class ShaderManager;
    public:
        Shader(): m_vertexShaderFilePath("")
                , m_pixelShaderFilePath("")
        {}

        Shader(std::string vertexFile, std::string pixelFile) :
            m_vertexShaderFilePath(vertexFile)
            , m_pixelShaderFilePath(pixelFile)
        {}

    public:

        bool load();
        void SetInputLayout();
        void setShaderpath(std::string& shader_name) 
        { 
            m_vertexShaderFilePath = shader_name + "_vs.cso";
            m_pixelShaderFilePath = shader_name + "_ps.cso";
        }
        int id;
        void setId(int _id) { id = _id; }

    private:
        ID3D11VertexShader      *pVS;    // the vertex shader
        ID3D11PixelShader       *pPS;     // the pixel shader
        ID3D11InputLayout       *m_pLayout;    // global

        std::string m_vertexShaderFilePath;
        std::string m_pixelShaderFilePath;

        bool create();
    };

#define LAYOUT_V2_C3(layout)  D3D11_INPUT_ELEMENT_DESC layout[] = {\
                            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                            { "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                        }

#define LAYOUT_V4_C3(layout)  D3D11_INPUT_ELEMENT_DESC layout[] = {\
                            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                            { "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                        }

#define LAYOUT_V4_N3_C3(layout)  D3D11_INPUT_ELEMENT_DESC layout[] = {\
                            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                            { "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                        }

#define LAYOUT_V4_N3_C3_I3(layout)  D3D11_INPUT_ELEMENT_DESC layout[] = {\
                            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                            { "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },\
                            { "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT,   1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},\
                       }

    class ShaderManager
    {

    public:
        ShaderManager(void);
        ~ShaderManager(void);

        Shader* loadShader(std::string& shaderpath);

    private:

        typedef std::map<std::string, Shader*> ShaderMap;
#define ShaderMapPair(name,ptr) std::make_pair(name,ptr)
#define ShaderMapIterator ShaderMap::iterator

        ShaderMap m_shaders;

        static ShaderManager* _instance;
    public:
        static ShaderManager* getPtr() { return _instance; }

#define SHADER_MANAGER ShaderManager::getPtr()
    };
}

