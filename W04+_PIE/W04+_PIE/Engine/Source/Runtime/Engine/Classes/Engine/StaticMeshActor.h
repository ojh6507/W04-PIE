#pragma once
#include "GameFramework/Actor.h"


class AStaticMeshActor : public AActor
{
    DECLARE_CLASS(AStaticMeshActor, AActor)

public:
    AStaticMeshActor();
   virtual AStaticMeshActor* Duplicate() override {
       AStaticMeshActor* newActor = reinterpret_cast<AStaticMeshActor*>(Super::Duplicate());
       newActor->StaticMeshComponent = StaticMeshComponent->Duplicate();
       return newActor;
    }
    UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

private:
    UStaticMeshComponent* StaticMeshComponent = nullptr;
};
