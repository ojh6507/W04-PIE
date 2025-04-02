#pragma once
#include "GameFramework/Actor.h"


class AStaticMeshActor : public AActor
{
    DECLARE_CLASS(AStaticMeshActor, AActor)

public:
    AStaticMeshActor();
    virtual AStaticMeshActor* Duplicate() override;
    UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

private:
    UStaticMeshComponent* StaticMeshComponent = nullptr;
};
