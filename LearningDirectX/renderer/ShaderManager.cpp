
#include "ShaderManager.h"
#include <fstream>
#include <vector>
#include "Renderer.h"

using namespace std;

namespace core {

    ShaderManager::ShaderManager(void)
    {
        _instance = this;
    }


    ShaderManager::~ShaderManager(void)
    {
    }


    bool Shader::create()
    {

        ifstream vsFile(m_vertexShaderFilePath, ios::binary);
        ifstream psFile(m_pixelShaderFilePath, ios::binary);

        vector<char> vsData = { istreambuf_iterator<char>(vsFile),istreambuf_iterator<char>() };
        vector<char> psData = { istreambuf_iterator<char>(psFile),istreambuf_iterator<char>() };

        RENDERER->getDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &pVS);
        RENDERER->getDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pPS);

        LAYOUT_V4_N3_C3(layout);

        HRESULT hr = RENDERER->getDevice()->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &m_pLayout);

        return true;
    }

    bool Shader::load()
    {
        RENDERER->getDeviceContext()->VSSetShader(pVS, nullptr, 0);
        RENDERER->getDeviceContext()->PSSetShader(pPS, nullptr, 0);
        RENDERER->getDeviceContext()->IASetInputLayout(m_pLayout);
        return true;
    }

    void Shader::SetInputLayout()
    {
        RENDERER->getDeviceContext()->IASetInputLayout(m_pLayout);
    }

    ShaderManager* ShaderManager::_instance = 0;

    Shader* ShaderManager::loadShader(string&  shaderpath)
    {
        if (m_shaders.find(shaderpath) != m_shaders.end())
        {
            return m_shaders.find(shaderpath)->second;
        }

        Shader* shader = new Shader();
        shader->setShaderpath(shaderpath);
        shader->create();
        m_shaders.insert(make_pair(shaderpath, shader));

        return shader;
    }
}