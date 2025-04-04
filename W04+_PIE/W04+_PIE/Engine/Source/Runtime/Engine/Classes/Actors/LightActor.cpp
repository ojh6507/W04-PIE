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
    ULightBaseActor* newActor = new ULightBaseActor();

    AActor* SActor = Super::Duplicate();

    newActor->SetUUID(SActor->GetUUID());
    newActor->SetClass(StaticClass());
    newActor->SetFName(GetFName());
    
    newActor->RootComponent = SActor->GetRootComponent()->Duplicate();
    /** 현재 Actor가 삭제 처리중인지 여부 */
    newActor->SetActorLocation(SActor->GetActorLocation());
    newActor->SetActorRotation(SActor->GetActorRotation());
    newActor->SetActorScale(SActor->GetActorScale());

    for (UActorComponent* Comp : SActor->OwnedComponents)
    {
        newActor->OwnedComponents.Add(Comp->Duplicate());
    }
    
    newActor->SetInternalIndex(SActor->GetInternalIndex());
    
    newActor->LightComponent = LightComponent->Duplicate();
    newActor->SpriteComponent = SpriteComponent->Duplicate();

    return newActor;
}
