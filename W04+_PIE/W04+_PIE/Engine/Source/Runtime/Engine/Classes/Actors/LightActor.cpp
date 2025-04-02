#include "LightActor.h"
#include "Runtime/Engine/Classes/Engine/Texture.h"
#include "Runtime/Engine/Classes/Components/UBillboardComponent.h"  
#include "Runtime/Engine/Classes/Components/LightComponent.h"  


ULightBaseActor::ULightBaseActor()
{
    // 1. 라이트 컴포넌트 생성 및 루트로 설정
    LightComponent = AddComponent<ULightComponentBase>();
    RootComponent = LightComponent; // 라이트 컴포넌트를 루트로 설정


//#if WITH_EDITORONLY_DATA
    SpriteComponent = AddComponent<UBillboardComponent>();

    SpriteComponent->SetSprite(L"Assets/Editor/Icon/SpotLight_64x.png");

    SpriteComponent->SetupAttachment(LightComponent);

    //SpriteComponent->SetTexture(L"Assets/Texture/spotLight.png");

}

ULightBaseActor* ULightBaseActor::Duplicate()
{
    ULightBaseActor* newActor = reinterpret_cast<ULightBaseActor*>(Super::Duplicate());

    newActor->LightComponent = LightComponent->Duplicate();
    newActor->SpriteComponent = SpriteComponent->Duplicate();
    return nullptr;
}
