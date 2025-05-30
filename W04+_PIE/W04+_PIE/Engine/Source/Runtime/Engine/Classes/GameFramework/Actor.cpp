#include "Actor.h"
#include "World.h"
#include "Components/SceneComponent.h"
#include "Components/ActorComponent.h"

AActor* AActor::Duplicate()
{
    AActor* NewObject = FObjectFactory::ConstructObject<AActor>();

    UObject* SuperObject = Super::Duplicate();

    NewObject->SetInternalIndex(SuperObject->GetInternalIndex());
    
    NewObject->RootComponent = RootComponent->Duplicate();
    /** 현재 Actor가 삭제 처리중인지 여부 */
    NewObject->bActorIsBeingDestroyed = bActorIsBeingDestroyed;
    
    NewObject->SetActorLocation(GetActorLocation());
    NewObject->SetActorRotation(GetActorRotation());
    NewObject->SetActorScale(GetActorScale());

    for (UActorComponent* Comp : OwnedComponents)
    {
        NewObject->OwnedComponents.Add(Comp->Duplicate());
    }

    return NewObject;
}

void AActor::BeginPlay()
{
    // TODO: 나중에 삭제를 Pending으로 하던가 해서 복사비용 줄이기
    const auto CopyComponents = OwnedComponents;
    for (UActorComponent* Comp : CopyComponents)
    {
        Comp->BeginPlay();
    }
}

void AActor::Tick(float DeltaTime)
{
    // TODO: 임시로 Actor에서 Tick 돌리기
    // TODO: 나중에 삭제를 Pending으로 하던가 해서 복사비용 줄이기
    const auto CopyComponents = OwnedComponents;
    for (UActorComponent* Comp : CopyComponents)
    {
        Comp->TickComponent(DeltaTime);
    }
}

void AActor::Destroyed()
{
    // Actor가 제거되었을 때 호출하는 EndPlay
    EndPlay(EEndPlayReason::Destroyed);
}

void AActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 본인이 소유하고 있는 모든 컴포넌트의 EndPlay 호출
    for (UActorComponent* Component : GetComponents())
    {
        if (Component->HasBegunPlay())
        {
            Component->EndPlay(EndPlayReason);
        }
    }
    UninitializeComponents();
}

bool AActor::Destroy()
{
    if (!IsActorBeingDestroyed())
    {
        if (UWorld* World = GetWorld())
        {
            World->DestroyActor(this);
            bActorIsBeingDestroyed = true;
        }
    }

    return IsActorBeingDestroyed();
}

void AActor::RemoveOwnedComponent(UActorComponent* Component)
{
    OwnedComponents.Remove(Component);
}

void AActor::InitializeComponents()
{
    TSet<UActorComponent*> Components = GetComponents();
    for (UActorComponent* ActorComp : Components)
    {
        if (ActorComp->bAutoActive && !ActorComp->IsActive())
        {
            ActorComp->Activate();
        }

        if (!ActorComp->HasBeenInitialized())
        {
            ActorComp->InitializeComponent();
        }
    }
}

void AActor::UninitializeComponents()
{
    TSet<UActorComponent*> Components = GetComponents();
    for (UActorComponent* ActorComp : Components)
    {
        if (ActorComp->HasBeenInitialized())
        {
            ActorComp->UninitializeComponent();
        }
    }
}

bool AActor::SetRootComponent(USceneComponent* NewRootComponent)
{
    if (NewRootComponent == nullptr || NewRootComponent->GetOwner() == this)
    {
        if (RootComponent != NewRootComponent)
        {
            USceneComponent* OldRootComponent = RootComponent;
            RootComponent = NewRootComponent;

            OldRootComponent->SetupAttachment(RootComponent);
        }
        return true;
    }
    return false;
}

FVector  AActor::GetActorLocation() const { return RootComponent ? RootComponent->GetWorldLocation() : FVector::ZeroVector; }
FVector  AActor::GetActorRotation() const { return RootComponent ? RootComponent->GetWorldRotation() : FVector::ZeroVector; }
FVector  AActor::GetActorScale() const { return RootComponent ? RootComponent->GetWorldScale() : FVector::ZeroVector; }

FVector  AActor::GetActorForwardVector() const { return RootComponent ? RootComponent->GetForwardVector() : FVector::ForwardVector; }
FVector  AActor::GetActorRightVector() const { return RootComponent ? RootComponent->GetRightVector() : FVector::RightVector; }
FVector  AActor::GetActorUpVector() const { return RootComponent ? RootComponent->GetUpVector() : FVector::UpVector; }


bool AActor::SetActorLocation(const FVector& NewLocation)
{
    if (RootComponent)
    {
        RootComponent->SetLocation(NewLocation);
        return true;
    }
    return false;
}

bool AActor::SetActorRotation(const FVector& NewRotation)
{
    if (RootComponent)
    {
        RootComponent->SetRotation(NewRotation);
        return true;
    }
    return false;
}

bool AActor::SetActorScale(const FVector& NewScale)
{
    if (RootComponent)
    {
        RootComponent->SetScale(NewScale);
        return true;
    }
    return false;
}
