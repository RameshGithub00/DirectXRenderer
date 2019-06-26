
#include <iostream>
#include <fstream>

#include "engine/XPlatform.h"

#include "Renderer.h"

#include "engine/camera/Camera.h"
#include "engine/model/Model.h"
#include "engine/model/Mesh.h"
#include "Material.h"
#include "engine/model/Terrain.h"
#include "engine/input/InputManager.h"

using namespace std;
using namespace DirectX;

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600


namespace core {

    u32 Renderer::RenderCommand::uiniqueId = 0;

    Terrain terrain;

    Renderer::Renderer(void)
    {
        matrices = (MatrixBufferType*)_aligned_malloc(sizeof(MatrixBufferType), 16);
    }


    Renderer::~Renderer(void)
    {
    }


    int Renderer::GetScreenWidth()
    {
        return SCREEN_WIDTH;
    }

    int Renderer::GetScreenHeight()
    {
        return SCREEN_HEIGHT;
    }

    bool Renderer::init(HWND hWnd)
    {

        TEXTUREMANAGER->create();

        HRESULT hr;

        DXGI_SWAP_CHAIN_DESC scd;

        ZeroMemory(&scd, sizeof(scd));

        scd.BufferCount = 1;                                             //1 backbuffer
        scd.BufferDesc.Width = SCREEN_WIDTH;
        scd.BufferDesc.Height = SCREEN_HEIGHT;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;          //use 32 bit color
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;          //how swap chain is to be used
        scd.OutputWindow = hWnd;
        scd.SampleDesc.Count = 2;                                        //how many multisamples
        scd.Windowed = TRUE;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching


        D3D11CreateDeviceAndSwapChain(NULL,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT| D3D11_CREATE_DEVICE_DEBUG,
            NULL,
            NULL,
            D3D11_SDK_VERSION,
            &scd,
            &m_pSwapchain,
            &m_pDevice,
            NULL,
            &m_pDeviceContext);


        ID3D11Texture2D *pBackBufferTexture;
        m_pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTexture);

        hr = m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pBackbuffer);

        pBackBufferTexture->Release();

        ID3D11RasterizerState* m_pRSState;

        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.AntialiasedLineEnable = TRUE;
        desc.CullMode = D3D11_CULL_BACK;
        desc.DepthClipEnable = TRUE;
        desc.DepthBias = 0;
        desc.DepthBiasClamp = 0.0f;
        desc.FillMode = D3D11_FILL_SOLID;
        desc.FrontCounterClockwise = TRUE;
        desc.MultisampleEnable = true;
        desc.ScissorEnable = false;
        desc.SlopeScaledDepthBias = 0.0f;

        m_pDevice->CreateRasterizerState(&desc, &m_pRSState);
        m_pDeviceContext->RSSetState(m_pRSState);

        //depth
        ID3D11Texture2D* pDepthStencil = NULL;

        D3D11_TEXTURE2D_DESC backbufferDesc;
        pBackBufferTexture->GetDesc(&backbufferDesc);

        D3D11_TEXTURE2D_DESC descDepth;
        ZeroMemory(&descDepth, sizeof(descDepth));
        descDepth.Width = backbufferDesc.Width;
        descDepth.Height = backbufferDesc.Height;
        descDepth.MipLevels = backbufferDesc.MipLevels;
        descDepth.ArraySize = backbufferDesc.ArraySize;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = backbufferDesc.SampleDesc.Count;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;
        hr = m_pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);

        D3D11_DEPTH_STENCIL_DESC dsDesc;
        ZeroMemory(&dsDesc, sizeof(dsDesc));
        // Depth test parameters
        dsDesc.DepthEnable = true;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        // Stencil test parameters
        dsDesc.StencilEnable = false;
        dsDesc.StencilReadMask = 0xFF;
        dsDesc.StencilWriteMask = 0xFF;

        // Stencil operations if pixel is front-facing
        dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // Stencil operations if pixel is back-facing
        dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        ID3D11DepthStencilState* pDSState;

        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory(&descDSV, sizeof(descDSV));
        descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        descDSV.Texture2D.MipSlice = 0;

        m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState);
        m_pDeviceContext->OMSetDepthStencilState(pDSState, 1);

        // Create the depth stencil view
        hr = m_pDevice->CreateDepthStencilView(pDepthStencil, // Depth stencil texture
            &descDSV, // Depth stencil desc
            &m_pStencilBuffer);  // [out] Depth stencil view

        ID3D11BlendState* g_pBlendState = NULL;

        D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
        renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
        renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
        renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
        renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        D3D11_BLEND_DESC BlendState;
        ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
        BlendState.RenderTarget[0] = renderTargetBlendDesc;
        BlendState.RenderTarget[0].BlendEnable = !true;

        m_pDevice->CreateBlendState(&BlendState, &g_pBlendState);
        //m_pDeviceContext->OMSetBlendState(g_pBlendState, 0, 0xffffffff);

        m_pDeviceContext->OMSetRenderTargets(1, &m_pBackbuffer, m_pStencilBuffer);
      

        // Set the viewport
        auto viewPOrt = CD3D11_VIEWPORT(0.0f, 0.0f, 800.0f, 600.0f);
        m_pDeviceContext->RSSetViewports(1, &viewPOrt);


        m_camera =  new Camera();

        createAndLoadShader();
        return TRUE;
    }

    bool Renderer::createAndLoadShader()
    {
        D3D11_BUFFER_DESC matrixBufferDesc;
        matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
        matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        matrixBufferDesc.MiscFlags = 0;
        matrixBufferDesc.StructureByteStride = 0;

        HRESULT result = m_pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
        if (FAILED(result))
        {
            return false;
        }

        return true;
    }

    void Renderer::addRenderCommand(RenderCommand& command)
    {
        if (command.m_type == RENDER_TYPE_OPAQUE)
        {
            opaque.insert(std::make_pair(command.m_id,&command));
        }
        if (command.m_type == RENDER_TYPE_TRANSPARENT)
        {
            transparent.insert(std::make_pair(command.m_id, &command));
        }
    }

    void Renderer::removeRenderCommand(u32 u_id)
    {
        if (opaque.find(u_id) != opaque.end())
        {
            opaque.erase(u_id);
        }

        if (transparent.find(u_id) != transparent.end())
        {
            transparent.erase(u_id);
        }
    }

    void Renderer::beginFrame()
    {
        opaque.clear();
        transparent.clear();
        //bind the render target
        m_pDeviceContext->OMSetRenderTargets(1, &m_pBackbuffer, m_pStencilBuffer);

        float clearColor[] = { 0.1,0.1,0.1,1 };
        m_pDeviceContext->ClearRenderTargetView(m_pBackbuffer, clearColor);
        m_pDeviceContext->ClearDepthStencilView(m_pStencilBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);


    }

    void Renderer::drawFrame()
    {
        m_camera->render(this);
        matrices->view = XMMatrixTranspose(matrices->view);
        matrices->projection = XMMatrixTranspose(matrices->projection);

        RenderListIterator itr = opaque.begin();

        while (itr != opaque.end())
        {
            for each (Model* model in (*itr->second).m_models)
            {
                model->update();
                matrices->world = XMMatrixTranspose(matrices->world);
                if (m_pMatrixBuffer)
                {
                    D3D11_MAPPED_SUBRESOURCE mappedResource;
                    m_pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
                    MatrixBufferType*dataPtr = (MatrixBufferType*)mappedResource.pData;
                    dataPtr->world = matrices->world;
                    dataPtr->view = matrices->view;
                    dataPtr->projection = matrices->projection;
                    m_pDeviceContext->Unmap(m_pMatrixBuffer, 0);
                    m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pMatrixBuffer);
                }
                model->draw();
               // RENDERER->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

           //     RENDERER->getDeviceContext()->IASetVertexBuffers(0, 1, model->m_pVertexBuffer, &strides, &offset);
                //RENDERER->getDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
               // RENDERER->getDeviceContext()->DrawIndexedInstanced(0,30,)
            }
            ++itr;
        }
    }
    void Renderer::endFrame()
    {
        m_pSwapchain->Present(1, 0);
    }
}