#include "LightComponent.h"
#include "UBillboardComponent.h"
#include "Math/JungleMath.h"
#include "UnrealEd/PrimitiveBatch.h"
#include "Classes/GameFramework/Actor.h"

ULightComponentBase::ULightComponentBase()
{
    // FString name = "SpotLight";
    // SetName(name);
    InitializeLight();
}

ULightComponentBase::~ULightComponentBase()
{
}
ULightComponentBase* ULightComponentBase::Duplicate()
{
    if (DuplicateObjects.Contains(GetUUID()))
    {
        return reinterpret_cast<ULightComponentBase*>(DuplicateObjects[GetUUID()]);
    }
    ULightComponentBase* NewObject1 = new ULightComponentBase();
    ULightComponentBase* NewObject = reinterpret_cast<ULightComponentBase*>(Super::Duplicate());
    NewObject1->color = color;
    NewObject1->radius = radius;
    NewObject1->AABB = AABB;
    NewObject1->SetUUID(NewObject->GetUUID());
    NewObject1->SetInternalIndex(NewObject->GetInternalIndex());
    NewObject1->SetClass(NewObject->GetClass());
    NewObject1->SetFName(NewObject->GetFName());

    return NewObject1;
}
void ULightComponentBase::SetColor(FVector4 newColor)
{
    color = newColor;
}

FVector4 ULightComponentBase::GetColor() const
{
    return color;
}

float ULightComponentBase::GetRadius() const
{
    return radius;
}

void ULightComponentBase::SetRadius(float r)
{
    radius = r;
}

void ULightComponentBase::InitializeLight()
{

    
    //texture2D = GetOwner()->AddComponent<UBillboardComponent>();
    //texture2D->SetTexture(L"Assets/Texture/spotLight.png");
    //texture2D->InitializeComponent();
    AABB.max = { 1.f,1.f,0.1f };
    AABB.min = { -1.f,-1.f,-0.1f };
    color = { 1,1,1,1 };
    radius = 5;
}

void ULightComponentBase::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);

    //texture2D->TickComponent(DeltaTime);
    //texture2D->SetLocation(GetWorldLocation());

}

int ULightComponentBase::CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance)
{
    bool res =AABB.Intersect(rayOrigin, rayDirection, pfNearHitDistance);
    return res;
}

