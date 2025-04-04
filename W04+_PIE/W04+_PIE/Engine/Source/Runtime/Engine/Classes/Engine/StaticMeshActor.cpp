#include "StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"


AStaticMeshActor::AStaticMeshActor()
{
    StaticMeshComponent = AddComponent<UStaticMeshComponent>();
    RootComponent = StaticMeshComponent;
}

AStaticMeshActor* AStaticMeshActor::Duplicate()
{
    AStaticMeshActor* newActor = new AStaticMeshActor();
    AActor* DuplicatedActor = Super::Duplicate();

    newActor->SetUUID(DuplicatedActor->GetUUID());
    newActor->SetClass(StaticClass());
    newActor->SetFName(GetFName());
    
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