
#include "Camera.h"
#include "renderer/Renderer.h"
#include "engine/input/InputManager.h"

namespace core {

    Camera::Camera()
    {
        m_fov = 45.0f;
        m_near = 0.1f;
        m_far = 1000000.0f;
        m_aspectRatio = 1.5f;


        XMFLOAT3 up, position, lookAt;
        XMVECTOR upVector, positionVector, lookAtVector;
        float yaw, pitch, roll;
        XMMATRIX rotationMatrix;


        // Setup the vector that points upwards.
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;

        // Load it into a XMVECTOR structure.
        upVector = XMLoadFloat3(&up);

        // Setup the position of the camera in the world.
        position.x = 0;
        position.y = 10;
        position.z = -100.0f;

        // Load it into a XMVECTOR structure.
        m_position = XMLoadFloat3(&position);

        // Setup where the camera is looking by default.
        lookAt.x = 0.0f;
        lookAt.y = 0.0f;
        lookAt.z = 1.0f;

        // Load it into a XMVECTOR structure.
        lookAtVector = XMLoadFloat3(&lookAt);

        // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
        pitch = 0 * 0.0174532925f;
        yaw = 0 * 0.0174532925f;
        roll = 0 * 0.0174532925f;

        // Create the rotation matrix from the yaw, pitch, and roll values.
        rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

        // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
        //lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
        m_up = XMVector3TransformCoord(upVector, rotationMatrix);

        // Translate the rotated camera position to the location of the viewer.
        m_lookAt = XMVectorAdd(m_position, lookAtVector);
    }


    Camera::~Camera()
    {
    }

    void Camera::Update()
    {
        //if (InputManager::sharedInputManager()->isKeyPressed(InputManager::KEY_DOWN))
        //{
        //    setPosition(x, y + 5, z, w);
        //}
        //if (InputManager::sharedInputManager()->isKeyPressed(InputManager::KEY_UP))
        //{
        //    setPosition(x, y - 5, z, w);
        //}
        //if (InputManager::sharedInputManager()->isKeyPressed(InputManager::KEY_LEFT))
        //{
        //    setPosition(x + 5, y, z, w);
        //}
        //if (InputManager::sharedInputManager()->isKeyPressed(InputManager::KEY_RIGHT))
        //{
        //    setPosition(x - 5, y, z, w);
        //}
    }


    void Camera::render(Renderer* _renderer)
    {
        Renderer::MatrixBufferType& matrices = _renderer->getMatricesForEdit();

        matrices.view = XMMatrixLookAtLH(m_position, m_lookAt, m_up);

        matrices.projection = XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_near, m_far);
    }
}