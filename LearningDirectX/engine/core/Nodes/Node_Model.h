#pragma once
#include "engine/core/Core_Node.h"
#include "renderer/Renderer.h"
#include "engine/MathsWrapper.h"
#include "engine/core/Containers.h"
namespace core 
{
    class Model;

    class Node_Model : public Node
    {
        Model* m_Model;
        Renderer::RenderCommand* m_pRenderCommand;
        std::string m_sModelPath;
        bool m_bVisible;
        bool m_bInstanced;

    public:
        static Node_Model* CreateWithModel(std::string model_path);
        static Node_Model* CreateWithModel(Model* model);

        virtual void init() override;
        void initWithModel(Model* model);

        virtual void update()override;
        virtual void draw()override;

        void setPosition(float x,float y,float z);
        void setPosition(Vec3d pos);
        void setScale(float scale);

        bool IsInstanced() { return m_bInstanced; }

        Node_Model();
        ~Node_Model();
    };
}

