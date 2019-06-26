#pragma once

#include "Model.h"

namespace core
{

    class Model;
    class Mesh;
    class Renderer;

    class Terrain:public Model
    {
    public:
        Terrain() :Model("") 
        {}

    protected:
        struct HeightMapType
        {
            float x, y, z;
        };

        struct ModelType
        {
            float x, y, z;
        };

    public:
        bool loadFromHeighmap();
        void setup() override
        {
            Model::setup();
        }

    private:
        int m_terrainHeight, m_terrainWidth;
        float m_heightScale;
        char* m_terrainFilename;
        HeightMapType* m_heightMap;
        ModelType* m_terrainModel;

        void update() override;

        void draw() override
        {
            Model::draw();
        }
    };
}