#pragma once
#include "../ModelComponent.h"
#include "../Camera/CameraControllerBase.h"

class BallComponent : public ModelComponent
{
public:
    BallComponent(Game* inGame, Camera* inCamera, CameraControllerBase* inCameraController, const std::string& modelPath,
                  const std::string& texturePath, DirectX::SimpleMath::Vector3 inScale, float inMovementSpeed,
                  float inRotationSpeed, float inBoundingBoxSize, float inBallResizeRate) : ModelComponent(inGame, inCamera, inCameraController, modelPath, texturePath)
    {
        SetScale(inScale);
        //scale = inScale;
        movementSpeed = inMovementSpeed;
        rotationSpeed = inRotationSpeed;
        boundingBoxSize = inBoundingBoxSize;
        ballResizeRate = inBallResizeRate;
    }

    float boundingBoxSize;
    void Update() override;
    void ResizeBoundingBox();

private:
    float ballResizeRate;
    float movementSpeed;
    float rotationSpeed;
};