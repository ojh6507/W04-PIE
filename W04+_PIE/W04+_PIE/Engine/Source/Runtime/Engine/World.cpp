#include "Engine/Source/Runtime/Engine/World.h"

#include "Actors/Player.h"
#include "BaseGizmos/TransformGizmo.h"
#include "Camera/CameraComponent.h"
#include "LevelEditor/SLevelEditor.h"
#include "Engine/FLoaderOBJ.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/SkySphereComponent.h"
#include "Classes/Engine/Level.h"


void UWorld::Initialize()
{
    // TODO: Load Scene
    CreateBaseObject();

}

void UWorld::InitializePIE()
{
    //TODO: Player 지정해주기
    if (EditorPlayer == nullptr)
    {
        EditorPlayer = FObjectFactory::ConstructObject<AEditorController>();;
    }

    TSet<AActor*> Actors = GetActors();

    for (AActor* Actor : Actors)
    {
        Actor->BeginPlay();
    }
}

void UWorld::CreateBaseObject()
{
    if (EditorPlayer == nullptr)
    {
        EditorPlayer = FObjectFactory::ConstructObject<AEditorController>();;
    }

    if (camera == nullptr)
    {
        camera = FObjectFactory::ConstructObject<UCameraComponent>();
        camera->SetLocation(FVector(8.0f, 8.0f, 8.f));
        camera->SetRotation(FVector(0.0f, 45.0f, -135.0f));
    }

    if (LocalGizmo == nullptr)
    {
        LocalGizmo = FObjectFactory::ConstructObject<UTransformGizmo>();
    }

    if (SelectedLevel == nullptr)
    {
        SelectedLevel = FObjectFactory::ConstructObject<ULevel>();
        SelectedLevel->SetWorld(this);
        SelectedLevel->Initialize();
    }
}

void UWorld::ReleaseBaseObject()
{
    if (LocalGizmo)
    {
        delete LocalGizmo;
        LocalGizmo = nullptr;
    }

    if (worldGizmo)
    {
        delete worldGizmo;
        worldGizmo = nullptr;
    }

    if (camera)
    {
        delete camera;
        camera = nullptr;
    }

    if (EditorPlayer)
    {
        delete EditorPlayer;
        EditorPlayer = nullptr;
    }

    if (SelectedLevel)
    {
        SelectedLevel->Release();
        GUObjectArray.MarkRemoveObject(SelectedLevel);
        SelectedLevel = nullptr;
    }
}

void UWorld::PIETick(float DeltaTime)
{
    camera->TickComponent(DeltaTime); //TODO: MainPlayer가 있으면 이거 하면안됨 분기줘야함

    EditorPlayer->Tick(DeltaTime);

    SelectedLevel->Tick(DeltaTime);
}


void UWorld::Tick(float DeltaTime)
{
    camera->TickComponent(DeltaTime);
    EditorPlayer->Tick(DeltaTime);
    LocalGizmo->Tick(DeltaTime);

    SelectedLevel->Tick(DeltaTime);
}

void UWorld::Release()
{
    pickingGizmo = nullptr;
    ReleaseBaseObject();

    GUObjectArray.ProcessPendingDestroyObjects();
}

UWorld* UWorld::Duplicate()
{
    UWorld* newWorld = FObjectFactory::ConstructObject<UWorld>();

    newWorld->camera = camera;
   
    ULevel* DuplicatedLevel = SelectedLevel->Duplicate();
    DuplicatedLevel->SetWorld(newWorld);
    newWorld->SelectedLevel = DuplicatedLevel;
    return newWorld;
}

bool UWorld::DestroyActor(AActor* ThisActor)
{
    return SelectedLevel->DestroyActor(ThisActor);
}

const TSet<AActor*>& UWorld::GetActors() const
{
    return SelectedLevel->GetActors();
}

void UWorld::SetPickingGizmo(UObject* Object)
{
    pickingGizmo = Cast<USceneComponent>(Object);
}