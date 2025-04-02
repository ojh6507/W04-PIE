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

    newActor->SetActorLocation(DuplicatedActor->GetActorLocation());
    newActor->SetActorRotation(DuplicatedActor->GetActorRotation());
    newActor->SetActorScale(DuplicatedActor->GetActorScale());
    
    newActor->RootComponent = DuplicatedActor->GetRootComponent()->Duplicate();
    
    newActor->StaticMeshComponent = StaticMeshComponent->Duplicate();

    for (UActorComponent* Comp : DuplicatedActor->OwnedComponents)
    {
        newActor->OwnedComponents.Add(Comp->Duplicate());
    }
    
    return newActor;
}