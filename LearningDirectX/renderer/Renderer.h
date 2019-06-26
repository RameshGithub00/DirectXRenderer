#pragma once

#include <vector>
#include <map>
#include "engine/XPlatform.h"
#include "framework/Singleton.h"

using namespace DirectX;

#pragma comment(lib,"d3d11")
#pragma comment(lib,"d3dx11")

/*
  ==> brain storming things for renderer
            perspectiveMatrix
            viewMatrix
            camera
            clear color
            post processing
            screen pixels effects
            
*/


namespace core {

    class Camera;
    class Model;

    class Renderer : public Singleton<Renderer>
    {
    public:
        Renderer(void);
        ~Renderer(void);

        int GetScreenWidth();
        int GetScreenHeight();
        
        enum RenderType{
            RENDER_TYPE_OPAQUE,
            RENDER_TYPE_TRANSPARENT,
        };

        class RenderCommand
        {
        private:
            static u32 uiniqueId;
        public:
            RenderCommand() :m_id(++uiniqueId){}
            const u32 m_id;
            std::vector<Model*> m_models;

            RenderType m_type;
        };


        typedef std::map<u32,RenderCommand*> RenderList;
        typedef std::map<u32, RenderCommand*>::iterator RenderListIterator;

        RenderList opaque;
        RenderList transparent;

        void addRenderCommand(RenderCommand& command);
        void removeRenderCommand(u32 u_id);

        /*
        */
        bool init(HWND);

        /*
        */
        void update();

        /*
        */
        void beginFrame();
        void drawFrame();
        void endFrame();


        ID3D11Device* getDevice() { return m_pDevice; }
        ID3D11DeviceContext* getDeviceContext() { return m_pDeviceContext; }
        bool createAndLoadShader();

        Camera* m_camera;

        void transpose()
        {
            matrices->world = XMMatrixTranspose(matrices->world);
            matrices->view = XMMatrixTranspose(matrices->view);
            matrices->projection = XMMatrixTranspose(matrices->projection);
        }

        struct MatrixBufferType
        {
            DirectX::XMMATRIX world;
            DirectX::XMMATRIX view;
            DirectX::XMMATRIX projection;
        };

        MatrixBufferType& getMatricesForEdit() { return *matrices; }

    private:
        //Pointer to a Direct3D device

        ID3D11Device            *m_pDevice;
        ID3D11DeviceContext     *m_pDeviceContext;
        IDXGISwapChain          *m_pSwapchain;
        ID3D11RenderTargetView  *m_pBackbuffer;
        ID3D11DepthStencilView  *m_pStencilBuffer;
        ID3D11RasterizerState* m_pRSState;
        ID3D11DepthStencilState* pDSState;
        ID3D11InputLayout *m_pLayout;    // global

        MatrixBufferType* matrices;

        D3D11_VIEWPORT viewport;

        ID3D11Buffer* m_pMatrixBuffer;

        std::vector<Model*> m_models;

    };

    #define RENDERER Renderer::getPtr()
}

