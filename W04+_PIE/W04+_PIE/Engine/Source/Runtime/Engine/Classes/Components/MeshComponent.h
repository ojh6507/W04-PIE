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
        if (DuplicateObjects.Contains(GetUUID()))
        {
            return static_cast<UMeshComponent*>(DuplicateObjects[GetUUID()]);
        }
        
        UMeshComponent* NewObject = PushValue(Super::Duplicate());
        
        NewObject->OverrideMaterials = this->OverrideMaterials;
        
        DuplicateObjects[GetUUID()] = NewObject;
        return NewObject;
    }

private:
    UMeshComponent* PushValue(UPrimitiveComponent* Other) 
    {
        UMeshComponent* NewObject = new UMeshComponent();
        NewObject->AABB = Other->AABB;

        return NewObject;
    }

    TArray<UMaterial*> OverrideMaterials;
#pragma endregion
protected:
public:
    TArray<UMaterial*>& GetOverrideMaterials() { return OverrideMaterials; }
};

