#pragma once
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
class ULevel : public UObject
{
    DECLARE_CLASS(ULevel, UObject)

public:
    TArray <AActor*> Actors;

};