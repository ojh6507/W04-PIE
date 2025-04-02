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

void UWorld::Initialize()
{
    // TODO: Load Scene
    CreateBaseObject();
    //SpawnObject(OBJ_CUBE);
    FManagerOBJ::CreateStaticMesh("Assets/Dodge/Dodge.obj");

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

    }
}

void UWorld::PIETick(float DeltaTime)
{
    PlayerController->Tick(DeltaTime);

    // SpawnActor()에 의해 Actor가 생성된 경우, 여기서 BeginPlay 호출
    for (AActor* Actor : PendingBeginPlayActors)
    {
        Actor->BeginPlay();
    }
    PendingBeginPlayActors.Empty();

    // 매 틱마다 Actor->Tick(...) 호출

    for (AActor* Actor : ActorsArray)
    {
        if (Actor && Actor->IsActorTickEnabled())
        {
            Actor->Tick(DeltaTime);
        }
    }
}


void UWorld::Tick(float DeltaTime)
{
    EditorPlayer->Tick(DeltaTime);
    LocalGizmo->Tick(DeltaTime);

    // SpawnActor()에 의해 Actor가 생성된 경우, 여기서 BeginPlay 호출
    for (AActor* Actor : PendingBeginPlayActors)
    {
        Actor->BeginPlay();
    }
    PendingBeginPlayActors.Empty();

    // 매 틱마다 Actor->Tick(...) 호출
    for (AActor* Actor : ActorsArray)
    {
        Actor->Tick(DeltaTime);
    }
}

void UWorld::Release()
{

    if (GEngineLoop.GetPlayWorldType() == EPlayWorldType::Edit) {
    for (AActor* Actor : ActorsArray)
    {
        Actor->EndPlay(EEndPlayReason::WorldTransition);
        TSet<UActorComponent*> Components = Actor->GetComponents();
        for (UActorComponent* Component : Components)
        {
            GUObjectArray.MarkRemoveObject(Component);
        }
        GUObjectArray.MarkRemoveObject(Actor);
    }
    ActorsArray.Empty();


        pickingGizmo = nullptr;
        ReleaseBaseObject();

        GUObjectArray.ProcessPendingDestroyObjects();
    }
}

UWorld* UWorld::Duplicate()
{

    UWorld* newWorld = FObjectFactory::ConstructObject<UWorld>();

    newWorld->SetInternalIndex(GetInternalIndex());


    for (auto Actor : ActorsArray) {

        newWorld->ActorsArray.Add(Actor->Duplicate());
    }

    return newWorld;
}

bool UWorld::DestroyActor(AActor* ThisActor)
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
    ActorsArray.Remove(ThisActor);

    // 제거 대기열에 추가
    GUObjectArray.MarkRemoveObject(ThisActor);
    return true;
}

void UWorld::SetPickingGizmo(UObject* Object)
{
    pickingGizmo = Cast<USceneComponent>(Object);
}