#include "ActorComponent.h"

#include "StaticMeshComponent.h"
#include "GameFramework/Actor.h"


void UActorComponent::InitializeComponent()
{
    assert(!bHasBeenInitialized);

    bHasBeenInitialized = true;
}

void UActorComponent::UninitializeComponent()
{
    assert(bHasBeenInitialized);

    bHasBeenInitialized = false;
}

void UActorComponent::BeginPlay()
{
    bHasBegunPlay = true;
}

void UActorComponent::TickComponent(float DeltaTime)
{
}

void UActorComponent::OnComponentDestroyed()
{
}

void UActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    assert(bHasBegunPlay);

    bHasBegunPlay = false;
}

void UActorComponent::DestroyComponent()
{
    if (bIsBeingDestroyed)
    {
        return;
    }

    bIsBeingDestroyed = true;

    // Owner에서 Component 제거하기
    if (AActor* MyOwner = GetOwner())
    {
        MyOwner->RemoveOwnedComponent(this);
        if (MyOwner->GetRootComponent() == this)
        {
            MyOwner->SetRootComponent(nullptr);
        }
    }

    if (bHasBegunPlay)
    {
        EndPlay(EEndPlayReason::Destroyed);
    }

    if (bHasBeenInitialized)
    {
        UninitializeComponent();
    }

    OnComponentDestroyed();

    GUObjectArray.MarkRemoveObject(this);
}

void UActorComponent::Activate()
{
    // TODO: Tick 다시 재생
    bIsActive = true;
}

void UActorComponent::Deactivate()
{
    // TODO: Tick 멈추기
    bIsActive = false;
}

UActorComponent* UActorComponent::Duplicate()
{
    UActorComponent* NewObject = new UActorComponent();

    UObject* SuperObject = Super::Duplicate();

    NewObject->SetUUID(SuperObject->GetUUID());
    NewObject->SetClass(StaticClass());
    NewObject->SetFName(GetFName());

    NewObject->SetInternalIndex(SuperObject->GetInternalIndex());
    
    NewObject->Owner = Owner;

    NewObject->bIsActive = bIsActive;

    NewObject->bAutoActive = bAutoActive;

    return NewObject;
}