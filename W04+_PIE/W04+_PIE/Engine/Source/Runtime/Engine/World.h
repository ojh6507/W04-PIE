#pragma once
#include "Define.h"
#include "Actors/Player.h"
#include "Container/Set.h"
#include "UObject/ObjectFactory.h"
#include "UObject/ObjectMacros.h"
#include "Classes/Engine/Level.h"

class FObjectFactory;
class AActor;
class UObject;
class UGizmoArrowComponent;
class UCameraComponent;
class AEditorController;
class USceneComponent;
class UTransformGizmo;

class UWorld : public UObject
{
public:
    DECLARE_CLASS(UWorld, UObject)

    UWorld() = default;
    
    void Initialize();
    void InitializePIE();
    void CreateBaseObject();
    void ReleaseBaseObject();
    void Tick(float DeltaTime);
    void PIETick(float DeltaTime);
    void Release();
    UWorld* Duplicate();
    /**
     * World에 Actor를 Spawn합니다.
     * @tparam T AActor를 상속받은 클래스
     * @return Spawn된 Actor의 포인터
     */
    template <typename T>
        requires std::derived_from<T, AActor>
    T* SpawnActor();

    /** World에 존재하는 Actor를 제거합니다. */
    bool DestroyActor(AActor* ThisActor);
    
private: 
    const FString defaultMapName = "Default";

    AActor* SelectedActor = nullptr;

    class UActorComponent* SelectedComponent = nullptr;

    USceneComponent* pickingGizmo = nullptr;
    UCameraComponent* camera = nullptr;
    AEditorController* EditorPlayer = nullptr;
    APlayerController* PlayerController = nullptr;

    ULevel* SelectedLevel = nullptr;

public:
    UObject* worldGizmo = nullptr;

    const TSet<AActor*>& GetActors() const;

    UTransformGizmo* LocalGizmo = nullptr;
    UCameraComponent* GetCamera() const { return camera; }
    AEditorPlayer* GetEditorPlayer() const { return EditorPlayer; }
    ULevel* GetLevel() const { return SelectedLevel; }


    // EditorManager 같은데로 보내기
    AActor* GetSelectedActor() const { return SelectedActor; }
    void SetPickedActor(AActor* InActor)
    {
        SelectedActor = InActor;
    }

    class UActorComponent* GetSelectedComponent() const { return SelectedComponent; }
    void SetSelectedComponent(class UActorComponent* InComponent)
    {
        SelectedComponent = InComponent;
    }

    UObject* GetWorldGizmo() const { return worldGizmo; }
    USceneComponent* GetPickingGizmo() const { return pickingGizmo; }
    void SetPickingGizmo(UObject* Object);
};

template <typename T>
    requires std::derived_from<T, AActor>
T* UWorld::SpawnActor()
{
    T* Actor = SelectedLevel->SpawnActor<T>();
    return Actor;
}