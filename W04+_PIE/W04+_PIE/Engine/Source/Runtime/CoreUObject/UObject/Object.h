#pragma once
#include "EngineLoop.h"
#include "NameTypes.h"
#include "UObjectArray.h"

extern FEngineLoop GEngineLoop;

class UClass;
class UWorld;

class UObject
{
public:
    using Super = UObject;
    using ThisClass = UObject;

    static UClass* StaticClass();
private:
    friend class FObjectFactory;
    friend class FSceneMgr;
    friend class UClass;

    uint32 UUID;
    uint32 InternalIndex; // Index of GUObjectArray

    FName NamePrivate;
    UClass* ClassPrivate = nullptr;

public:
    UObject();
    virtual ~UObject() = default;

    UWorld* GetWorld()
    {
        return GEngineLoop.GetWorld();
    }

    FEngineLoop& GetEngine()
    {
        return GEngineLoop;
    }
    
    template<typename T>
        requires std::derived_from<T, UObject>
    void DuplicateSubObjects()
    {
        for (auto& SubObject : SubObjects.GetObjectItemArrayUnsafe())
        {
             SubObject = SubObject->Duplicate<T>();
        }
    }
    
    template<typename T>
        requires std::derived_from<T, UObject>
    T* Duplicate()
    {
        // 현재 객체가 T 타입인지 확인
        if (T* ThisAsT = dynamic_cast<T*>(this)) {
            T* NewObject = new T(); // 안전하게 복사 생성자 호출
            NewObject->CopyFrom(*ThisAsT); // 사용자 정의 메서드로 데이터 복사
            NewObject->DuplicateSubObjects<T>();
            return NewObject;
        }
        
        return nullptr;
    }

    virtual void CopyFrom(const UObject& Other) {
        *this = Other;
    }
    
    FName GetFName() const { return NamePrivate; }
    FString GetName() const { return NamePrivate.ToString(); }

    uint32 GetUUID() const { return UUID; }
    uint32 GetInternalIndex() const { return InternalIndex; }

    UClass* GetClass() const { return ClassPrivate; }


    /** this가 SomeBase인지, SomeBase의 자식 클래스인지 확인합니다. */
    bool IsA(const UClass* SomeBase) const;

    template <typename T>
        requires std::derived_from<T, UObject>
    bool IsA() const
    {
        return IsA(T::StaticClass());
    }

public:
    void* operator new(size_t size)
    {
        UE_LOG(LogLevel::Display, "UObject Created : %d", size);

        void* RawMemory = FPlatformMemory::Malloc<EAT_Object>(size);
        UE_LOG(
            LogLevel::Display,
            "TotalAllocationBytes : %d, TotalAllocationCount : %d",
            FPlatformMemory::GetAllocationBytes<EAT_Object>(),
            FPlatformMemory::GetAllocationCount<EAT_Object>()
        );
        return RawMemory;
    }

    void operator delete(void* ptr, size_t size)
    {
        UE_LOG(LogLevel::Display, "UObject Deleted : %d", size);
        FPlatformMemory::Free<EAT_Object>(ptr, size);
    }

    FVector4 EncodeUUID() const {
        FVector4 result;

        result.x = UUID % 0xFF;
        result.y = UUID >> 8 & 0xFF;
        result.z = UUID >> 16 & 0xFF;
        result.a = UUID >> 24 & 0xFF;

        return result;
    }
private:
};
