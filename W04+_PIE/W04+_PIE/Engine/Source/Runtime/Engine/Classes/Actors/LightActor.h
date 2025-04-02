#pragma once
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectTypes.h"

class USceneComponent;
class UPrimitiveComponent;

class ULightBaseActor : public AActor
{
DECLARE_CLASS(ULightBaseActor, AActor)
public:
    ULightBaseActor();

    virtual ULightBaseActor* Duplicate() override;
private:

    class ULightComponentBase* LightComponent;
        
// To do 에디터에서만 빌보드가 그려지게 
//#if WITH_EDITORONLY_DATA
    // 에디터에서 아이콘 표시를 위한 빌보드 컴포넌트

    class UBillboardComponent* SpriteComponent;
//#endif


};

