#include "Game.h"
#include "Components/TestComponent.h"
#include "Components/TriangleComponent.h"

int main()
{
    const auto inst = new Game();
    inst->AddGameComponent(new TriangleComponent(inst));
    inst->AddGameComponent(new TestComponent(inst));
    inst->Initialize();
    inst->Run();
}