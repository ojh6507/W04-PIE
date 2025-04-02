#include "Engine/Source/Runtime/CoreUObject/UObject/Object.h"

#include "UClass.h"
#include "UObjectHash.h"


UClass* UObject::StaticClass()
{
    static UClass ClassInfo{TEXT("UObject"), sizeof(UObject), alignof(UObject), nullptr};
    return &ClassInfo;
}

UObject::UObject()
    : UUID(0)
    // TODO: Object를 생성할 때 직접 설정하기
    , InternalIndex(std::numeric_limits<uint32>::max())
    , NamePrivate("None")
{
}

UObject* UObject::Duplicate()
{
    if (DuplicateObjects.Contains(GetUUID()))
    {
        return DuplicateObjects[GetUUID()];
    }

    // 현재 객체가 T 타입인지 확인
    UObject* NewObject = new UObject(); // 안전하게 복사 생성자 호출

    NewObject->InternalIndex = this->InternalIndex;

    DuplicateObjects[NewObject->GetUUID()] = NewObject;

    return NewObject;
}

bool UObject::IsA(const UClass* SomeBase) const
{
    const UClass* ThisClass = GetClass();
    return ThisClass->IsChildOf(SomeBase);
}
