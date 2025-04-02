#include "Engine/Source/Runtime/Engine/World.h"

#include "Actors/Player.h"
#include "BaseGizmos/TransformGizmo.h"
#include "Camera/CameraComponent.h"
#include "LevelEditor/SLevelEditor.h"
#include "Engine/FLoaderOBJ.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/SkySphereComponent.h"
#include "UnrealEd/EditorViewportClient.h"
#include "Classes/Engine/Level.h"

void UWorld::Initialize()
{
    // TODO: Load Scene
    CreateBaseObject();

    FManagerOBJ::CreateStaticMesh("Assets/SkySphere.obj");
    AActor* SpawnedActor = SpawnActor<AActor>();
    USkySphereComponent* skySphere = SpawnedActor->AddComponent<USkySphereComponent>();

    skySphere->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"SkySphere.obj"));

    skySphere->GetStaticMesh()->GetMaterials()[0]->Material->SetDiffuse(FVector((float)32 / 255, (float)171 / 255, (float)191 / 255));
}

void UWorld::InitializePIE()
{
    if (PlayerController == nullptr)
    {
        PlayerController = FObjectFactory::ConstructObject<APlayerController>();
    }

    GEngineLoop.GetLevelEditor()->GetActiveViewportClient()->ViewTransformPerspective.SetLocation(FVector(8.0f, 8.0f, 8.f));
    GEngineLoop.GetLevelEditor()->GetActiveViewportClient()->ViewTransformPerspective.SetRotation(FVector(0.0f, 45.0f, -135.0f));

    TSet<AActor*> Actors = GetActors();

    for (AActor* Actor : Actors)
    {
        if (Actor)
            Actor->BeginPlay();
    }
}

void UWorld::CreateBaseObject()
{
    if (EditorPlayer == nullptr)
    {
        EditorPlayer = FObjectFactory::ConstructObject<AEditorController>();
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
    if (GEngineLoop.GetPlayWorldType() == EPlayWorldType::Edit) {

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
    PlayerController->Tick(DeltaTime);

    SelectedLevel->Tick(DeltaTime);
}


void UWorld::Tick(float DeltaTime)
{
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
    UWorld* newWorld = new UWorld();
   
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