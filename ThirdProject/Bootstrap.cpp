#include "Game.h"
#include "Components/RectComponent.h"

int main()
{
    const auto inst = new Game();
    inst->Initialize();
    inst->Run();
    inst->Exit();
}