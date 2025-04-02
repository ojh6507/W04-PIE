#pragma once
#include "PrimitiveComponent.h"
#include "Material/Material.h"

class UMeshComponent : public UPrimitiveComponent
{
    DECLARE_CLASS(UMeshComponent, UPrimitiveComponent)
public:
    UMeshComponent() = default;

#pragma region Material
    virtual uint32 GetNumMaterials() const { return 0; }
    virtual UMaterial* GetMaterial(uint32 ElementIndex) const;
    virtual uint32 GetMaterialIndex(FName MaterialSlotName) const;
    virtual UMaterial* GetMaterialByName(FName MaterialSlotName) const;
    virtual TArray<FName> GetMaterialSlotNames() const;
    virtual void SetMaterial(uint32 ElementIndex, UMaterial* Material);
    virtual void SetMaterialByName(FName MaterialSlotName, class UMaterial* Material);
    virtual void GetUsedMaterials(TArray<UMaterial*>& Out) const;
    virtual UMeshComponent* Duplicate() override
    {
        UMeshComponent* NewObject = FObjectFactory::ConstructObject<UMeshComponent>();

        UPrimitiveComponent* Comp = Super::Duplicate();

        NewObject->SetLocation(Comp->GetWorldLocation());
        NewObject->SetRotation(Comp->GetWorldRotation());
        NewObject->SetScale(Comp->GetWorldScale());
        
        NewObject->AABB = Comp->AABB;

        NewObject->SetType(Comp->GetType());
  
        NewObject->OverrideMaterials = this->OverrideMaterials;
        
        return NewObject;
    }

private:
    UMeshComponent* PushValue(UPrimitiveComponent* Other) 
    {
        UMeshComponent* NewObject = new UMeshComponent();
        NewObject->AABB = Other->AABB;

        return NewObject;
    }
protected:
    TArray<UMaterial*> OverrideMaterials;
#pragma endregion
protected:
public:
    TArray<UMaterial*>& GetOverrideMaterials() { return OverrideMaterials; }
};

