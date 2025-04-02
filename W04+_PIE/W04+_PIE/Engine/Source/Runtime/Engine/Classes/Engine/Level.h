#pragma once
#include "Define.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"

class FObjectFactory;
class AAcotr;
class UObject;
class UGizmoArrowComponent;
class UCameraComponent;
class AEditorPlayer;
class USceneComponent;
class UTransformGizmo;

class ULevel : public UObject
{
    DECLARE_CLASS(ULevel, UObject)

public:
    ULevel() = default;

    void Initialize();
    
    void Tick(float DeltaTime);
    void Release();

    virtual ULevel* Duplicate() override;

    /**
     * Level에 Actor를 Spawn합니다.
     * @tparam T AActor를 상속받은 클래스
     * @return Spawn된 Actor의 포인터
     */
    template <typename T>
        requires std::derived_from<T, AActor>
    T* SpawnActor();

    /** Level에 존재하는 Actor를 제거합니다. */
    bool DestroyActor(AActor* ThisActor);

    UWorld* GetWorld() const { return OwnerWorld; }
    void SetWorld(UWorld* World) { OwnerWorld = World; }

    const TSet<AActor*>& GetActors() const { return Actors; }

private:

    /** ULevel에서 관리되는 모든 Actor의 목록 */
    TSet<AActor*> Actors;

    /** Actor가 Spawn되었고, 아직 BeginPlay가 호출되지 않은 Actor들 */
    TArray<AActor*> PendingBeginPlayActors;

    UWorld* OwnerWorld = nullptr;
};

template <typename T>
    requires std::derived_from<T, AActor>
T* ULevel::SpawnActor()
{
    T* Actor = FObjectFactory::ConstructObject<T>();
    Actor->SetLevel(this);
    // TODO: 일단 AddComponent에서 Component마다 초기화
    // 추후에 RegisterComponent() 만들어지면 주석 해제
    // Actor->InitializeComponents();
    Actors.Add(Actor);
    PendingBeginPlayActors.Add(Actor);
    return Actor;
}