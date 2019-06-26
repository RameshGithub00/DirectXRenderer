#pragma once
#include "framework/Singleton.h"
#include "engine/core/Core_Node.h"
namespace core
{
    class SceneGraph : public Node
    {
    public:
        SceneGraph();
        ~SceneGraph();
        static SceneGraph* Create() { return(SceneGraph*)Node::Create(); }
    };
}

