
#include "Node_Model.h"
#include "engine/Model/Model.h"
#include "renderer/Renderer.h"
namespace core
{

    Node_Model* Node_Model::CreateWithModel(std::string _model_path)
    {
        Node_Model* model = new Node_Model;
        model->m_sModelPath = _model_path;
        model->init();
        model->m_bVisible = true;
        return model;
    }

    Node_Model* Node_Model::CreateWithModel(Model* model)
    {
        
        Node_Model* node_model = new Node_Model;
        node_model->m_Model = model;
        node_model->initWithModel(model);
        node_model->m_bVisible = true;
        return node_model;
    }

    void Node_Model::init()
    {
        m_Model = new Model(m_sModelPath.c_str());
        m_Model->load();
        m_Model->scale(1.0f);
        m_Model->setup();
        initWithModel(m_Model);
        
    }

    void Node_Model::initWithModel(Model* model)
    {
        m_pRenderCommand = new Renderer::RenderCommand();
        m_pRenderCommand->m_type = Renderer::RENDER_TYPE_OPAQUE;
        m_pRenderCommand->m_models.push_back(model);
    }

    void Node_Model::update()
    {
    }

    void Node_Model::draw()
    {
        if(m_bVisible)
            RENDERER->addRenderCommand(*m_pRenderCommand);
    }

    void Node_Model::setScale(float scale)
    {
        m_Model->scale(scale);
    }

    void Node_Model::setPosition(float x, float y, float z)
    {
        m_Model->setPosition(x, y, z);
    }

    void Node_Model::setPosition(Vec3d pos)
    {
        m_Model->setPosition(pos.x, pos.y, pos.z);
    }

    Node_Model::Node_Model()
    {
    }


    Node_Model::~Node_Model()
    {
    }
}
