#pragma once

#include "Display/D3DApplication.h"
#include "Input/InputDevice.h"

class BallComponent;
class RacketComponent;

class Game : public D3DApplication
{
public:
    Game();
    ~Game();
    InputDevice* input_device;
    void Initialize();
    void Run();
    void AddGameComponent(GameComponent* inComponent);
    void Exit();

    int firstPlayerScore = 0;
    int secondPlayerScore = 0;
    RacketComponent* firstRacket;
    RacketComponent* secondRacket;
    BallComponent* ball;

private:
    float deltaTime;

    void HandleSystemMessages(MSG& msg, bool& isExitRequested);
    void Update();
    void DestroyResources();

    bool CheckBallCollision(const RacketComponent* other) const;
    void PerformBallCollision(const RacketComponent* other) const;
    void PerformBallBoundCheck();
    void ResetScene() const;
    void NotifyScore() const;
    void ProcessUserInput() const;

};