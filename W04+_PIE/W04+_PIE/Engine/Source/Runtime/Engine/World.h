#pragma once
#include "Define.h"
#include "Actors/Player.h"
#include "UObject/ObjectFactory.h"
#include "UObject/ObjectMacros.h"

class FObjectFactory;
class AActor;
class UObject;
class UGizmoArrowComponent;
class UCameraComponent;
class AEditorController;
class USceneComponent;
class UTransformGizmo;
class ULevel;

template <typename T, typename Hasher , typename Allocator>
class TSet;

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
    AEditorController* EditorPlayer = nullptr;
    APlayerController* PlayerController = nullptr;

    ULevel* SelectedLevel = nullptr;

public:
    UObject* worldGizmo = nullptr;

    const TSet<AActor*>& GetActors() const;

    UTransformGizmo* LocalGizmo = nullptr;
    AEditorController* GetEditorPlayer() const;
    ULevel* GetLevel() const;


    // EditorManager 같은데로 보내기
    AActor* GetSelectedActor() const;
    void SetPickedActor(AActor* InActor);

    class UActorComponent* GetSelectedComponent() const;
    void SetSelectedComponent(class UActorComponent* InComponent);
    UObject* GetWorldGizmo() const { return worldGizmo; }
    USceneComponent* GetPickingGizmo() const { return pickingGizmo; }
    void SetPickingGizmo(UObject* Object);
};

#include "UWorld_Impl.inl"