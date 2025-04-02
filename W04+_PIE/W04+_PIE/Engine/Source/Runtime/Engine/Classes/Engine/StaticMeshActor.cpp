#include "StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"


AStaticMeshActor::AStaticMeshActor()
{
    StaticMeshComponent = AddComponent<UStaticMeshComponent>();
    RootComponent = StaticMeshComponent;
}

AStaticMeshActor* AStaticMeshActor::Duplicate()
{
    AStaticMeshActor* newActor = reinterpret_cast<AStaticMeshActor*>(Super::Duplicate());
    newActor->StaticMeshComponent = StaticMeshComponent->Duplicate();
    return newActor;
}