#include "engine/app/application.h"
#include "engine/Logger.h"


#include <string>
#include <vector>

#include "engine/core/nodes/Node_Model.h"
#include "engine/core/Core_SceneGraph.h"
#include "engine/framework/GameObject.h"

#include "engine/MathsWrapper.h"
#include "engine/Model/Terrain.h"

using namespace core;

class Game : public Application
{

    SceneGraph* scene;
    Terrain terrain;
public:
    bool init(HINSTANCE hInstance)
    {
        bool ret = Application::init(hInstance);        

        
        terrain.loadFromHeighmap();
        terrain.setup();

        scene = SceneGraph::Create();

        ////for (int i = 0;i < 100;i++)
        ////{
        ////    Node_Model* model = Node_Model::CreateWithModel("models/donut");
        ////    int range = 250;
        ////    int halfRange = range / 2;
        ////    float zFactor = 0.05f;
        ////    long tickCount = GetTickCount();
        ////    model->setPosition(-halfRange + rand() % range, -halfRange + rand() % range, 200 + (-halfRange + rand() % range) * zFactor);
        ////    model->setScale(1);
        ////    scene->addChild(model);
        ////}

        Node_Model* model = Node_Model::CreateWithModel((Model*)&terrain);
        model->setPosition(Vec3d(0, -50, 200));
        model->setScale(100);
        scene->addChild(model);


        //physics
        {
            Body* body = m_World->CreateBody();

            body->gravityMultiplier.y = 0.8f;

            BoxShape* box = new BoxShape(1, 1);

            body->AddShape(box);

            Node_Model* model = Node_Model::CreateWithModel("models/donut");
            int range = 250;
            int halfRange = range / 2;
            float zFactor = 0.05f;
            long tickCount = GetTickCount();
            model->setScale(1);

            body->SetPos(Vec2(5, 5));

            Node_GameObject* gameobject = Node_GameObject::CreateWithBody(body, model);

            scene->addChild(gameobject);
        }
        {
            Body* body = m_World->CreateBody();

            body->gravityMultiplier.y = 0.5f;

            BoxShape* box = new BoxShape(1, 1);

            body->AddShape(box);

            body->SetPos(Vec2(0, 10));

            Node_Model* model = Node_Model::CreateWithModel("models/donut");
            int range = 250;
            int halfRange = range / 2;
            float zFactor = 0.05f;
            long tickCount = GetTickCount();
            model->setScale(1);

            Node_GameObject* gameobject = Node_GameObject::CreateWithBody(body, model);

            scene->addChild(gameobject);
        }

        

        //physics

        runWithScene(scene);

        LOG("%d", 10);

        return ret;
    }
    void run()
    {
        Application::run();
    }
};