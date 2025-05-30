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
    

    virtual UObject* Duplicate();

    FName GetFName() const { return NamePrivate; }
    void SetFName(FName InFName) {NamePrivate = InFName;}
    FString GetName() const { return NamePrivate.ToString(); }

    uint32 GetUUID() const { return UUID; }
    void SetUUID(uint32 INUUID) {UUID = INUUID;}
    uint32 GetInternalIndex() const { return InternalIndex; }
    void SetInternalIndex(uint32 InInternalIndex){InternalIndex = InInternalIndex;}
    UClass* GetClass() const { return ClassPrivate; }
    void SetClass(UClass* InClass) { ClassPrivate = InClass; }
    
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
