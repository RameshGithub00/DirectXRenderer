
#include "engine/XPlatform.h"
#include "Mesh.h"
#include "Model.h"
#include "renderer/Material.h"
#include "renderer/ShaderManager.h"
#include <stdio.h>
#include "renderer/Renderer.h"
#include <fstream>


namespace core {

    using namespace std;

    Mesh::Mesh()
    {
    }


    Mesh::~Mesh(void)
    {
    }

    void Mesh::cleanUp()
    {
        if (m_pConstantBuffer)
            m_pVertexBuffer->Release();

        if(m_pConstantBuffer)
            m_pConstantBuffer->Release();
    }

    void Mesh::setup()
    {
        CD3D11_BUFFER_DESC buffer = CD3D11_BUFFER_DESC(sizeof(Vertex)*m_vertices.size(), D3D11_BIND_VERTEX_BUFFER);
        D3D11_SUBRESOURCE_DATA subdata;
        subdata.pSysMem = &m_vertices[0];
        RENDERER->getDevice()->CreateBuffer(&buffer, &subdata, &m_pVertexBuffer);

        CD3D11_BUFFER_DESC buffer1 = CD3D11_BUFFER_DESC(sizeof(FaceVertex)*m_index.size(), D3D11_BIND_INDEX_BUFFER);
        D3D11_SUBRESOURCE_DATA subdata1;
        subdata1.pSysMem = &m_index[0];
        RENDERER->getDevice()->CreateBuffer(&buffer1, &subdata1, &m_pIndexBuffer);

    }

    void Mesh::draw()
    {   
        RENDERER->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        RENDERER->getDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer,&stride, &offset);
        RENDERER->getDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        RENDERER->getDeviceContext()->DrawIndexed(m_index.size() * 3, 0, 0);
    }
}