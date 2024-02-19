#pragma once

class Game;

class GameComponent
{
public:
    virtual void Initialize();
    virtual void Update();
    virtual void Draw();
    virtual void Reload();
    virtual void DestroyResources();

protected:
    Game* game;
};
