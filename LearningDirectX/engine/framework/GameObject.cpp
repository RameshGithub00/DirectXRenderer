
#include "GameObject.h"


Node_GameObject::Node_GameObject(Body* body, core::Node_Model* model)
{
    m_Body = body;
    m_ModelNode = model;
}

Node_GameObject* Node_GameObject::CreateWithBody(Body*body, core::Node_Model* model)
{
    Node_GameObject* node = new Node_GameObject(body, model);
    node->addChild(model);
    return node;
}

void Node_GameObject::update()
{
    Node::update();
    m_ModelNode->setPosition(m_Body->GetPos().x*10,m_Body->GetPos().y*10,200);//zero
}

void Node_GameObject::draw()
{
    //Node::draw();
}