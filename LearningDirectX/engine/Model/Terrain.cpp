#include <windows.h>
#include "Terrain.h"
#include <stdio.h>
#include "Model.h"
#include "Mesh.h"
#include "renderer/Material.h"
#include "engine/input/InputManager.h"
#include "engine/Logger.h"
namespace core
{

    using namespace DirectX;

    bool Terrain::loadFromHeighmap()
    {
        int error, imageSize, i, j, k, index;
        FILE* filePtr;
        unsigned long long count;
        BITMAPFILEHEADER bitmapFileHeader;
        BITMAPINFOHEADER bitmapInfoHeader;
        unsigned char* bitmapImage;
        unsigned char height;

        m_terrainWidth = 540;
        m_terrainHeight = 270;

        // Start by creating the array structure to hold the height map data.
        m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
        if (!m_heightMap)
        {
            return false;
        }
        m_terrainFilename = "textures/world_heigh_map.bmp";
        // Open the bitmap map file in binary.
        error = fopen_s(&filePtr, m_terrainFilename, "rb");
        if (error != 0)
        {
            return false;
        }

        // Read in the bitmap file header.
        count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
        if (count != 1)
        {
            return false;
        }

        // Read in the bitmap info header.
        count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
        if (count != 1)
        {
            return false;
        }

        // Make sure the height map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
        //if ((bitmapInfoHeader.biHeight != m_terrainHeight) || (bitmapInfoHeader.biWidth != m_terrainWidth))
        //{
        //    return false;
        //}

        // Calculate the size of the bitmap image data.  
        // Since we use non-divide by 2 dimensions (eg. 257x257) we need to add an extra byte to each line.
        imageSize = m_terrainHeight * ((m_terrainWidth * 3) + 1);

        // Allocate memory for the bitmap image data.
        bitmapImage = new unsigned char[imageSize];
        if (!bitmapImage)
        {
            return false;
        }

        // Move to the beginning of the bitmap data.
        fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

        // Read in the bitmap image data.
        count = fread(bitmapImage, 1, imageSize, filePtr);
        //if (count != imageSize)
        //{
        //    return false;
        //}

        // Close the file.
        error = fclose(filePtr);
        if (error != 0)
        {
            return false;
        }

        // Initialize the position in the image data buffer.
        k = 0;

        // Read the image data into the height map array.
        for (j = 0; j < m_terrainHeight; j++)
        {
            for (i = 0; i < m_terrainWidth; i++)
            {
                // Bitmaps are upside down so load bottom to top into the height map array.
                index = (m_terrainWidth * (m_terrainHeight - 1 - j)) + i;

                // Get the grey scale pixel value from the bitmap image data at this location.
                height = bitmapImage[k];

                // Store the pixel value as the height at this point in the height map array.
                m_heightMap[index].y = (float)height;

                // Increment the bitmap image data index.
                k += 3;
            }

            // Compensate for the extra byte at end of each line in non-divide by 2 bitmaps (eg. 257x257).
            k++;
        }


        m_material = new Material();
        Material::MaterialDefines def;
        def.shaderName = "shaders/compiled/default";
        def.texturePath0 = "textures/default.png";
        m_material->create(def);

        m_mesh = new Mesh();

        Mesh::VertexList& vertices = m_mesh->m_vertices;
        std::vector<Mesh::Normal>& normals = m_mesh->m_normals;
        std::vector<Mesh::Face>& faces = m_mesh->m_faces;
        std::vector<Mesh::FaceVertex>& indices = m_mesh->m_index;

        int terrainWidth = m_terrainWidth;
        int terrainHeight = m_terrainHeight;
        float terrainScale = 1;

        int hIndex = 0;

        //hIndex = m_terrainWidth * m_terrainHeight-1;

        for (int i = -terrainHeight / 2;i < terrainHeight / 2;i++)
        {
            for (int j = -terrainWidth / 2;j < terrainWidth / 2;j++)
            {
                Mesh::Vertex vertex;

                vertex.position.x = terrainScale * j;
                vertex.position.y = m_heightMap[hIndex++].y*.1;
                vertex.position.z = terrainScale * i;
                vertex.w = 1;

                vertices.push_back(vertex);

            }
        }

        for (int i = 0;i < (terrainWidth - 1) * (terrainHeight - 1);i++)
        {
            Mesh::FaceVertex faceVertex;

            faceVertex.tri[0] = i;
            faceVertex.tri[1] = i + 1;
            faceVertex.tri[2] = i + terrainWidth + 1;

            indices.push_back(faceVertex);

            faceVertex.tri[0] = i;
            faceVertex.tri[1] = i + terrainWidth + 1;
            faceVertex.tri[2] = i + terrainWidth;

            indices.push_back(faceVertex);
        }

        //Mesh::FaceVertex face;
        //face.tri_1[0] = 0;
        //face.tri_1[1] = terrainWidth-1;
        //face.tri_1[2] = terrainWidth * terrainHeight - 1;

        //indices.push_back(face);

        if (normals.empty())
        {
            //calculate normals
            for (int i = 0;i < indices.size();i++)
            {
                Mesh::FaceVertex triFace = indices[i];
                Mesh::Vertex& v1 = vertices[triFace.tri[0]];
                Mesh::Vertex& v2 = vertices[triFace.tri[1]];
                Mesh::Vertex& v3 = vertices[triFace.tri[2]];

                XMVECTOR arm1;
                arm1.m128_f32[0] = v2.x - v1.x;
                arm1.m128_f32[1] = v2.y - v1.y;
                arm1.m128_f32[2] = v2.z - v1.z;
                arm1.m128_f32[3] = 1;

                XMVECTOR arm2;
                arm2.m128_f32[0] = v3.x - v2.x;
                arm2.m128_f32[1] = v3.y - v2.y;
                arm2.m128_f32[2] = v3.z - v2.z;
                arm2.m128_f32[3] = 1;

                XMVECTOR cross = XMVector3Cross(arm1, arm2);

                v1.nx = cross.m128_f32[0];
                v1.ny = cross.m128_f32[1];
                v1.nz = cross.m128_f32[2];

                v2.normal = v3.normal = v1.normal;

            }
        }
        else
        {
            //use provided normals
            for (size_t i = 0; i < faces.size(); i++)
            {
                for (int j = 0;j < 3;j++)
                {
                    vertices[faces[i].v[j] - 1].nx = normals[faces[i].t[j] - 1].x;
                    vertices[faces[i].v[j] - 1].ny = normals[faces[i].t[j] - 1].y;
                    vertices[faces[i].v[j] - 1].nz = normals[faces[i].t[j] - 1].z;
                }
            }
        }

        setPosition(0, -50, 400);
        rotateX(-3.14*0.3);
        m_material = new Material();

        // Release the bitmap image data now that the height map array has been loaded.
        delete[] bitmapImage;
        bitmapImage = 0;

        // Release the terrain filename now that is has been read in.
        //delete[] m_terrainFilename;
        m_terrainFilename = 0;

        return true;
    }

    void Terrain::update()
    {
        Model::update();
        scale(2.0f);

        if (INPUTMANAGER->isKeyPressed(InputManager::KEY_DOWN))
        {
            setPosition(x, y + 5, z, w);
        }
        if (INPUTMANAGER->isKeyPressed(InputManager::KEY_UP))
        {
            setPosition(x, y - 5, z, w);
        }
        if (INPUTMANAGER->isKeyPressed(InputManager::KEY_LEFT))
        {
            setPosition(x + 5, y, z, w);
        }
        if (INPUTMANAGER->isKeyPressed(InputManager::KEY_RIGHT))
        {
            setPosition(x - 5, y, z, w);
        }

        if (INPUTMANAGER->isMouseAction(InputManager::MOUSE_L_PRESSED))
        {
            const InputManager::MousePos& pos = INPUTMANAGER->getMousePosition();

            const InputManager::MouseDelta& delta = INPUTMANAGER->getMouseDelta();

            float sensitivity = 5.0f;

            if (delta.x != 0)
                x += delta.x > 0 ? sensitivity : -sensitivity;
            if (delta.y != 0)
                y += delta.y > 0 ? sensitivity : -sensitivity;

            setPosition(x, y, 400);
            LOG("\ndelta pos %d %d", delta.x, delta.y);
        }

        //rotateY(45);
    }


}