#pragma once
#include "Components/MeshComponent.h"
#include "Mesh/StaticMesh.h"

class UStaticMeshComponent : public UMeshComponent
{
    DECLARE_CLASS(UStaticMeshComponent, UMeshComponent)

public:
    UStaticMeshComponent() = default;

    PROPERTY(int, selectedSubMeshIndex);

    virtual uint32 GetNumMaterials() const override;
    virtual UMaterial* GetMaterial(uint32 ElementIndex) const override;
    virtual uint32 GetMaterialIndex(FName MaterialSlotName) const override;
    virtual TArray<FName> GetMaterialSlotNames() const override;
    virtual void GetUsedMaterials(TArray<UMaterial*>& Out) const override;

    virtual int CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance) override;

    virtual UStaticMeshComponent* Duplicate() override
    {
        if (DuplicateObjects.Contains(GetUUID()))
        {
            return reinterpret_cast<UStaticMeshComponent*>(DuplicateObjects[GetUUID()]);
        }
       
        UStaticMeshComponent* NewObject = reinterpret_cast<UStaticMeshComponent*>(Super::Duplicate());
        
        NewObject->staticMesh = reinterpret_cast<UStaticMesh*>(staticMesh->Duplicate());

        NewObject->selectedSubMeshIndex = selectedSubMeshIndex;

        DuplicateObjects[GetUUID()] = NewObject;

        return NewObject;
    }

    UStaticMesh* GetStaticMesh() const { return staticMesh; }
    void SetStaticMesh(UStaticMesh* value)
    {
        staticMesh = value;
        OverrideMaterials.SetNum(value->GetMaterials().Num());
        AABB = FBoundingBox(staticMesh->GetRenderData()->BoundingBoxMin, staticMesh->GetRenderData()->BoundingBoxMax);
    }

protected:
    UStaticMesh* staticMesh = nullptr;
    int selectedSubMeshIndex = -1;
};