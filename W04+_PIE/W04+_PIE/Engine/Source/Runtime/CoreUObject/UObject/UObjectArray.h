#pragma once
#include "Container/Array.h"
#include "Container/Set.h"

class UClass;
class UObject;


class FUObjectArray
{
public:
    void AddObject(UObject* Object);
    void MarkRemoveObject(UObject* Object);

    void ProcessPendingDestroyObjects();

    TArray<UObject*>& GetObjectItemArrayUnsafe()
    {
        return ObjObjects;
    }

private:
    TArray<UObject*> ObjObjects;
    TArray<UObject*> PendingDestroyObjects;
};

extern FUObjectArray GUObjectArray;
extern TMap<uint32_t,UObject*> DuplicateObjects;