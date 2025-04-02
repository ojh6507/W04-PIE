#pragma once
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectTypes.h"
class UGizmoBaseComponent;
class UGizmoArrowComponent;
class USceneComponent;
class UPrimitiveComponent;

class AController : public AActor
{
    DECLARE_CLASS(AController, AActor)
    
    AController() = default;
    
    virtual void Tick(float DeltaTime) override = 0;

    virtual void Input(float DeltaTime) = 0;

protected:
    virtual int RayIntersectsObject(const FVector& pickPosition, USceneComponent* obj, float& hitDistance, int& intersectCount);
    virtual void ScreenToViewSpace(int screenX, int screenY, const FMatrix& viewMatrix, const FMatrix& projectionMatrix, FVector& rayOrigin);

    bool bLeftMouseDown = false;
    bool bRightMouseDown = false;
    bool bSpaceDown = false;
    POINT m_LastMousePos;
};

class AEditorController : public AController
{
    DECLARE_CLASS(AEditorController, AController)

    AEditorController() = default;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void Input(float DeltaTime) override;

    bool PickGizmo(FVector& rayOrigin);
    void PickActor(const FVector& pickPosition);
    void AddControlMode();
    void AddCoordiMode();

private:
    void PickedObjControl();
    void ControlRotation(USceneComponent* pObj, UGizmoBaseComponent* Gizmo, int32 deltaX, int32 deltaY);
    void ControlTranslation(USceneComponent* pObj, UGizmoBaseComponent* Gizmo, int32 deltaX, int32 deltaY);
    void ControlScale(USceneComponent* pObj, UGizmoBaseComponent* Gizmo, int32 deltaX, int32 deltaY);

    ControlMode cMode = CM_TRANSLATION;
    CoordiMode cdMode = CDM_WORLD;

public:
    void SetMode(ControlMode _Mode) { cMode = _Mode; }
    ControlMode GetControlMode() const { return cMode; }
    CoordiMode GetCoordiMode() const { return cdMode; }
};

class APlayerController : public AController
{
    DECLARE_CLASS(APlayerController, AController)

    APlayerController() = default;

    virtual void Tick(float DeltaTime) override;

    virtual void Input(float DeltaTime) override;

    void SetPlayer(AActor* PlayerActor){ Player = PlayerActor; }
    AActor* GetPlayer() const { return Player; } //TODO: PlayerActor를 만들어야함
    
private:
    AActor* Player = nullptr;
    
};