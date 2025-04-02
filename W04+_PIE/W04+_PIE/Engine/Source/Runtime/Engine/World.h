#pragma once
#include "Define.h"
#include "Container/Set.h"
#include "UObject/ObjectFactory.h"
#include "UObject/ObjectMacros.h"
#include "Classes/Engine/Level.h"

class FObjectFactory;
class AActor;
class UObject;
class UGizmoArrowComponent;
class UCameraComponent;
class AEditorPlayer;
class USceneComponent;
class UTransformGizmo;

class UWorld : public UObject
{
    DECLARE_CLASS(UWorld, UObject)

public:
    UWorld() = default;

    void Initialize();
    void CreateBaseObject();
    void ReleaseBaseObject();
    void Tick(float DeltaTime);
    void Release();

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

    USceneComponent* pickingGizmo = nullptr;
    UCameraComponent* camera = nullptr;
    AEditorPlayer* EditorPlayer = nullptr;

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