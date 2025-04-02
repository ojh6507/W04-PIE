#include "Level.h"

#include "Engine/FLoaderOBJ.h"
#include "Components/SkySphereComponent.h"

void ULevel::Initialize()
{   
    // TODO: Load Scene
    FManagerOBJ::CreateStaticMesh("Assets/Dodge/Dodge.obj");
    FManagerOBJ::CreateStaticMesh("Assets/SkySphere.obj");

    AActor* SpawnedActor = SpawnActor<AActor>();
    USkySphereComponent* SkySphere = SpawnedActor->AddComponent<USkySphereComponent>();
    SkySphere->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"SkySphere.obj"));
    SkySphere->GetStaticMesh()->GetMaterials()[0]->Material->SetDiffuse(FVector((float)32 / 255, (float)171 / 255, (float)191 / 255));
}

void ULevel::Tick(float DeltaTime)
{
    for (AActor* Actor : PendingBeginPlayActors)
    {
        Actor->BeginPlay();
    }
    PendingBeginPlayActors.Empty();

    for (AActor* Actor : Actors)
    {
        Actor->Tick(DeltaTime);
    }
}

void ULevel::Release()
{
    for (AActor* Actor : Actors)
    {
        Actor->EndPlay(EEndPlayReason::WorldTransition);
        TSet<UActorComponent*> Components = Actor->GetComponents();
        for (UActorComponent* Component : Components)
        {
            GUObjectArray.MarkRemoveObject(Component);
        }
        GUObjectArray.MarkRemoveObject(Actor);
    }
    Actors.Empty();
    GUObjectArray.ProcessPendingDestroyObjects();
}

bool ULevel::DestroyActor(AActor* ThisActor)
{
    if (ThisActor->GetWorld() == nullptr)
    {
        return false;
    }

    if (ThisActor->IsActorBeingDestroyed())
    {
        return true;
    }

    // 액터의 Destroyed 호출
    ThisActor->Destroyed();

    if (ThisActor->GetOwner())
    {
        ThisActor->SetOwner(nullptr);
    }

    TSet<UActorComponent*> Components = ThisActor->GetComponents();
    for (UActorComponent* Component : Components)
    {
        Component->DestroyComponent();
    }

    // World에서 제거
    Actors.Remove(ThisActor);

    // 제거 대기열에 추가
    GUObjectArray.MarkRemoveObject(ThisActor);
    return true;
}
