#pragma once
#include "Engine/Source/Runtime/Engine/Classes/Engine/Level.h"

class UWorld;

template <typename T>
    requires std::derived_from<T, AActor>
T* UWorld::SpawnActor()
{
    T* Actor = SelectedLevel->SpawnActor<T>();
    return Actor;
}