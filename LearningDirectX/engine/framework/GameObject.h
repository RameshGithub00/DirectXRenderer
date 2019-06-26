#pragma once

#include "engine/core/Core_Node.h"
#include "engine/core/Nodes/Node_Model.h"
#include "Physics.h"

class Node_GameObject :public core::Node
{
    core::Node_Model* m_ModelNode;
    World* m_Physics;
    Body* m_Body;
private:
    Node_GameObject(Body* body, core::Node_Model* model);
public:
    static Node_GameObject* CreateWithBody(Body*, core::Node_Model* model);

    virtual void update() override;
    virtual void draw() override;

};