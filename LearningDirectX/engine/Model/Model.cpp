
#include "engine/XPlatform.h"
#include "Model.h"
#include "Mesh.h"
#include "renderer/Material.h"
#include "renderer/Renderer.h"
#include "ModelImporter.h"
#include <vector>
#include <time.h>

using namespace std;

namespace core {

    Model::Model(const char* _modelpath) : m_modelPath(_modelpath)
    {
        
    }

    void Model::load()
    {
        ModelImporter importer;

        m_material = new Material();

        if (!importer.loadFromFile(m_modelPath + ".obj", &m_mesh, &m_material))
        {
            delete m_mesh;
            m_mesh = nullptr;

            delete m_material;
            m_material = nullptr;
        }
        if (m_mesh)
        {
            m_mesh->SetModel(this);
        }
    }

    void Model::setOffset(float offset) { m_mesh->m_offset = offset; }

    Model::~Model(void)
    {
        if (m_mesh)
        {
            m_mesh->cleanUp();
            delete m_mesh;
            m_mesh = nullptr;
        }
    }

    void Model::setup()
    {
        if (!m_mesh)
            return;

        Material::MaterialDefines mat;

        m_mesh->setup();
        m_material->create(mat);
        
        Renderer::MatrixBufferType& matrices = RENDERER->getMatricesForEdit();
        matrices.world = DirectX::XMMatrixIdentity();
    }

    void Transform::scale(float _scale)
    {
        m_scale = _scale;
    }

    void Transform::rotateX(float value)
    {
        m_rotX = value;
    }

    void Transform::rotateY(float value)
    {
        m_rotY = value;
    }

    void Model::update()
    {
        if (!m_mesh)
            return;

        //float phase = ((float)(rand() % 360)*(3.14 / 180.0f));
        //translate(0.008f*sin(phase), 0.008f*cos(phase));

        Renderer::MatrixBufferType& matrices = RENDERER->getMatricesForEdit();
        matrices.world = XMMatrixIdentity();

        matrices.world.r[3] = m_position;

        matrices.world.r[0].m128_f32[0] = m_scale;
        matrices.world.r[1].m128_f32[1] = m_scale;
        matrices.world.r[2].m128_f32[2] = m_scale;

        
        matrices.world = DirectX::XMMatrixRotationX(m_rotX)*matrices.world;
        matrices.world = DirectX::XMMatrixRotationY(m_rotY)*matrices.world;

        m_material->apply();
    }

    void Model::draw()
    {
        if (!m_mesh)
            return;

        m_mesh->draw();
    }
}