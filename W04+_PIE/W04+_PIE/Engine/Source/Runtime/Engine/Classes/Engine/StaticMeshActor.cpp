#include "StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"


AStaticMeshActor::AStaticMeshActor()
{
    StaticMeshComponent = AddComponent<UStaticMeshComponent>();
    RootComponent = StaticMeshComponent;
}

AStaticMeshActor* AStaticMeshActor::Duplicate()
{
    AStaticMeshActor* newActor = FObjectFactory::ConstructObject<AStaticMeshActor>();
    AActor* DuplicatedActor = Super::Duplicate();

    newActor->RootComponent = DuplicatedActor->GetRootComponent()->Duplicate();
    
    newActor->StaticMeshComponent = StaticMeshComponent->Duplicate();
    return newActor;
}