#pragma once
#include "GameComponent.h"

class Camera;

class SphereComponent : public GameComponent
{
public:
    SphereComponent(Game* inGame, Camera* inCamera, GameComponent* inParent = nullptr,
                    DirectX::SimpleMath::Vector3 inPosition = {0.0f, 0.0f, 0.0f}, float radius = 5.0f)
    {
        this->game = inGame;
        this->camera = inCamera;
        colour = ConstantBufferColour{{1.0f, 1.0f, 1.0f, 1.0f}};

        if (inParent != nullptr)
        {
            parent = inParent;
        }

        position = inPosition;

        const int numSegments = 20;
        const int numRings = 20;

        for (int ring = 0; ring <= numRings; ++ring)
        {
            float phi = DirectX::XM_PI * static_cast<float>(ring) / numRings;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            for (int segment = 0; segment <= numSegments; ++segment)
            {
                float theta = 2.0f * DirectX::XM_PI * static_cast<float>(segment) / numSegments;
                float sinTheta = sin(theta);
                float cosTheta = cos(theta);

                float x = radius * sinPhi * cosTheta;
                float y = radius * cosPhi;
                float z = radius * sinPhi * sinTheta;

                DirectX::SimpleMath::Vector4 vertex(x, y, z, 1.0f);
                vertices.push_back(vertex);
            }
        }

        for (int ring = 0; ring < numRings; ++ring)
        {
            for (int segment = 0; segment < numSegments; ++segment)
            {
                int nextRing = ring + 1;
                int nextSegment = segment + 1;

                indices.push_back(ring * (numSegments + 1) + segment);
                indices.push_back(nextRing * (numSegments + 1) + segment);
                indices.push_back(ring * (numSegments + 1) + nextSegment);

                indices.push_back(ring * (numSegments + 1) + nextSegment);
                indices.push_back(nextRing * (numSegments + 1) + segment);
                indices.push_back(nextRing * (numSegments + 1) + nextSegment);
            }
        }

        /*int rings = 40;
        int sectors = 40;
        float const R = 1. / (float)(rings - 1);
        float const S = 1. / (float)(sectors - 1);
        int r, s;

        vertices.resize(rings * sectors * 3);
        auto v = vertices.begin();
        for (r = 0; r < rings; r++)
            for (s = 0; s < sectors; s++)
            {
                float const y = sin(-DirectX::XM_PI / 2 + DirectX::XM_PI * r * R);
                float const x = cos(2 * DirectX::XM_PI * s * S) * sin(DirectX::XM_PI * r * R);
                float const z = sin(2 * DirectX::XM_PI * s * S) * sin(DirectX::XM_PI * r * R);

                *v++ = DirectX::SimpleMath::Vector4(x, y, z, 1.0f);
            }

        indices.resize(rings * sectors * 4);
        auto i = indices.begin();
        for (r = 0; r < rings; r++)
            for (s = 0; s < sectors; s++)
            {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s + 1);
                *i++ = (r + 1) * sectors + (s + 1);
                *i++ = (r + 1) * sectors + s;
            }*/


        /*double latitudeBands = 20;
        double longitudeBands = 20;

        for (double latNumber = 0; latNumber <= latitudeBands; latNumber++)
        {
            double theta = latNumber * DirectX::XM_PI / latitudeBands;
            double sinTheta = sin(theta);
            double cosTheta = cos(theta);

            for (double longNumber = 0; longNumber <= longitudeBands; longNumber++)
            {
                double phi = longNumber * 2 * DirectX::XM_PI / longitudeBands;
                double sinPhi = sin(phi);
                double cosPhi = cos(phi);

                float x = cosPhi * sinTheta; // x
                float y = cosTheta; // y
                float z = sinPhi * sinTheta; // z

                vertices.push_back(DirectX::SimpleMath::Vector4(x, y, z, 1.0f));
            }

            for (int latNumber = 0; latNumber < latitudeBands; latNumber++)
            {
                for (int longNumber = 0; longNumber < longitudeBands; longNumber++)
                {
                    int first = (latNumber * (longitudeBands + 1)) + longNumber;
                    int second = first + longitudeBands + 1;

                    indices.push_back(first);
                    indices.push_back(second);
                    indices.push_back(first + 1);

                    indices.push_back(second);
                    indices.push_back(second + 1);
                    indices.push_back(first + 1);
                }
            }
        }*/

        InitializeResources();
    }

    void InitializeResources() override;
    void DestroyResources() override;
    void Update() override;
    void Draw() override;
};