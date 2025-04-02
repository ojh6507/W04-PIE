#pragma once
#include "PrimitiveComponent.h"
#include "UTexture.h"

class UBillboardComponent : public UPrimitiveComponent
{
    DECLARE_CLASS(UBillboardComponent, UPrimitiveComponent)
    
public:
    UBillboardComponent();
    virtual ~UBillboardComponent() override;


    virtual UBillboardComponent* Duplicate() override;

    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;
    virtual int CheckRayIntersection(
        FVector& rayOrigin,
        FVector& rayDirection, float& pfNearHitDistance
    ) override;

    void SetSprite(FWString _fileName); 
    void SetSprite(std::shared_ptr<FTexture> _Sprite);
    std::shared_ptr<FTexture> GetSprite() const
    {
        return Sprite;
    }

    UBillboardComponent* GetValue(UPrimitiveComponent* comp);
    void SetUUIDParent(USceneComponent* _parent);
    FMatrix CreateBillboardMatrix();

    ID3D11Buffer* vertexTextureBuffer;
    ID3D11Buffer* indexTextureBuffer;
    UINT numVertices;
    UINT numIndices;
    float finalIndexU = 0.0f;
    float finalIndexV = 0.0f;
    std::shared_ptr<FTexture> Sprite;
protected:



    //USceneComponent* m_parent = nullptr;

    bool CheckPickingOnNDC(const TArray<FVector>& checkQuad, float& hitDistance);

    void CreateQuadTextureVertexBuffer();
private:
};
