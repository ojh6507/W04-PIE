#include "Components/SceneComponent.h"
#include "World.h"
#include "Math/JungleMath.h"
#include "UObject/ObjectFactory.h"
#include "UTextUUID.h"
#include "UObject/ObjectFactory.h"

USceneComponent* USceneComponent::Duplicate()
{
    if (DuplicateObjects.Contains(GetUUID()))
    {
        return reinterpret_cast<USceneComponent*>(DuplicateObjects[GetUUID()]);
    }
   
    USceneComponent* NewObject = FObjectFactory::ConstructObject<USceneComponent>();

    NewObject->RelativeLocation = RelativeLocation;
    NewObject->RelativeRotation = RelativeRotation;
    NewObject->RelativeScale3D = RelativeScale3D;
    NewObject->QuatRotation = QuatRotation;

    NewObject->AttachParent = AttachParent;

    DuplicateObjects[NewObject->GetUUID()] = NewObject;

    return NewObject;
}
USceneComponent::USceneComponent() :RelativeLocation(FVector(0.f, 0.f, 0.f)), RelativeRotation(FVector(0.f, 0.f, 0.f)), RelativeScale3D(FVector(1.f, 1.f, 1.f))
{
}

USceneComponent::~USceneComponent()
{
	if (uuidText) delete uuidText;
}
void USceneComponent::InitializeComponent()
{
    Super::InitializeComponent();

}

void USceneComponent::TickComponent(float DeltaTime)
{
	Super::TickComponent(DeltaTime);
}


int USceneComponent::CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance)
{
    int nIntersections = 0;
    return nIntersections;
}

FVector USceneComponent::GetForwardVector()
{
	FVector Forward = FVector(1.f, 0.f, 0.0f);
	Forward = JungleMath::FVectorRotate(Forward, QuatRotation);
	return Forward;
}

FVector USceneComponent::GetRightVector()
{
	FVector Right = FVector(0.f, 1.f, 0.0f);
	Right = JungleMath::FVectorRotate(Right, QuatRotation);
	return Right;
}

FVector USceneComponent::GetUpVector()
{
	FVector Up = FVector(0.f, 0.f, 1.0f);
	Up = JungleMath::FVectorRotate(Up, QuatRotation);
	return Up;
}


void USceneComponent::AddLocation(FVector _added)
{
	RelativeLocation = RelativeLocation + _added;

}

void USceneComponent::AddRotation(FVector _added)
{
	RelativeRotation = RelativeRotation + _added;

}

void USceneComponent::AddScale(FVector _added)
{
	RelativeScale3D = RelativeScale3D + _added;

}

bool USceneComponent::IsAttachedTo(const USceneComponent* PossibleParent) const
{
    if (PossibleParent == nullptr) return false;
    for (const USceneComponent* TestParent = AttachParent; TestParent != nullptr; TestParent = TestParent->AttachParent)
    {
        if (TestParent == PossibleParent)
        {
            return true;
        }
    }
    return false;
}

FVector USceneComponent::GetWorldRotation()
{
	if (AttachParent)
	{
		return FVector(AttachParent->GetLocalRotation() + GetLocalRotation());
	}
	else
		return GetLocalRotation();
}

FVector USceneComponent::GetWorldScale()
{
	if (AttachParent)
	{
		return FVector(AttachParent->GetWorldScale() + GetLocalScale());
	}
	else
		return GetLocalScale();
}

FVector USceneComponent::GetWorldLocation()
{
	if (AttachParent)
	{
		return FVector(AttachParent->GetWorldLocation() + GetLocalLocation());
	}
	else
		return GetLocalLocation();
}

FVector USceneComponent::GetLocalRotation()
{
	return JungleMath::QuaternionToEuler(QuatRotation);
}

void USceneComponent::SetRotation(FVector _newRot)
{
	RelativeRotation = _newRot;
	QuatRotation = JungleMath::EulerToQuaternion(_newRot);
}


/**
 * 부모 SceneComponent에 이 컴포넌트를 연결(Attach)합니다.
 * 기존에 부모가 있었다면 자동으로 분리(Detach)됩니다.
 * nullptr을 InParent로 전달하면 현재 부모로부터 분리됩니다.
 *
 * @param InParent      이 컴포넌트를 연결할 새로운 부모 SceneComponent. nullptr이면 월드에 직접 연결(분리)됩니다.
 */
void USceneComponent::SetupAttachment(USceneComponent* InParent)
{

    // 1. 자기 자신에게 붙이려고 하거나, 이미 원하는 부모에 붙어있는 경우 아무것도 하지 않음.
    if (InParent == this and InParent == nullptr)
    {

        UE_LOG(LogLevel::Warning, "Cannot attach component to itself.");
        return;
    }
    if (InParent == AttachParent)
    {
        // 이미 이 부모에 연결되어 있으므로 변경 필요 없음.
        return;
    }

    //순환 종속성 검사 InParent가 this의 자식인지 확인
    if (InParent != nullptr && InParent->IsAttachedTo(this))
    {
        UE_LOG(LogLevel::Warning, "Cannot attach component to one of its children.");
        return;
    }

    // 2. 기존 부모로부터 분리 (Detach)
    if (AttachParent != nullptr) // 기존 부모가 있는 경우
    {
        // 기존 부모의 자식 목록에서 'this'를 제거
        AttachParent->AttachChildren.Remove(this);
    }

    // 3. 새로운 부모로 설정 및 연결 (Attach)
    AttachParent = InParent; // 새로운 부모 포인터 업데이트

    if (AttachParent != nullptr) // 새로운 부모가 유효한 경우 (nullptr이 아닌 경우)
    {
        // 새로운 부모의 자식 목록에 'this'를 추가
        AttachParent->AttachChildren.AddUnique(this); 

    }

    //if (
    //    InParent != AttachParent                                  // 설정하려는 Parent가 기존의 Parent와 다르거나
    //    && InParent != this                                       // InParent가 본인이 아니고
    //    && InParent != nullptr                                    // InParent가 유효한 포인터 이며
    //    && (
    //        AttachParent == nullptr                               // AttachParent도 유효하며
    //        || !AttachParent->AttachChildren.Contains(this)  // 이미 AttachParent의 자식이 아닌 경우
    //    ) 
    //) {
    //    AttachParent = InParent;
    //    InParent->AttachChildren.AddUnique(this);
    //}
}