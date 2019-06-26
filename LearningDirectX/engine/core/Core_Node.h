#pragma once
#include "engine/Logger.h"
#include <string>
#include <vector>
namespace core 
{   
    class Node
    {
    public:
        static Node* Create() { return new Node(); }
        virtual void init() { LOG("no init",""); }
       
        Node::Node()
        {
        }

        Node::~Node()
        {
        }

        bool Node::isLeaf()
        {

        }

        int Node::getChildCount()
        {
            return _childs.size();
        }

        Node** Node::getChildrens()
        {
            return &_childs[0];
        }

        Node * Node::getChild(int index)
        {
            return nullptr;
        }

        virtual void Node::addChild(Node* _child)
        {
            if (_child)
            {
                _childs.push_back(_child);
            }
        }

        virtual void Node::update()
        {
            
            for(int i=0;i<_childs.size();++i)
            {
                Node* node = _childs[i];
                node->update();
                node->draw();
            }
        }

        virtual void Node::draw()
        {
        }

        virtual void finalize() {};

    private:
        std::vector<Node*>  _childs;
    };
}

