#pragma once
#include "Engine/Source/Runtime/Engine/Classes/Components/SceneComponent.h"

class UPrimitiveComponent : public USceneComponent
{
    DECLARE_CLASS(UPrimitiveComponent, USceneComponent)

public:
    UPrimitiveComponent();
    virtual ~UPrimitiveComponent() override;

    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;
    virtual int CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance);
    bool IntersectRayTriangle(
        const FVector& rayOrigin, const FVector& rayDirection,
        const FVector& v0, const FVector& v1, const FVector& v2, float& hitDistance
    );
    FBoundingBox AABB;

    virtual UPrimitiveComponent* Duplicate() override
    {

        USceneComponent* SuperComp = Super::Duplicate();

        UPrimitiveComponent* NewObject = FObjectFactory::ConstructObject<UPrimitiveComponent>();

        NewObject->SetLocation(SuperComp->GetWorldLocation());
        NewObject->SetScale(SuperComp->GetWorldScale());
        NewObject->SetRotation(SuperComp->GetWorldRotation());
        
        NewObject->AABB = AABB;

        NewObject->m_Type = m_Type;


        return NewObject;
    }

private:
    FString m_Type;

public:
    FString GetType() { return m_Type; }
    
    void SetType(const FString& _Type)
    {
        m_Type = _Type;
        //staticMesh = FEngineLoop::resourceMgr.GetMesh(m_Type);
    }
    FBoundingBox GetBoundingBox() { return AABB; }
};

