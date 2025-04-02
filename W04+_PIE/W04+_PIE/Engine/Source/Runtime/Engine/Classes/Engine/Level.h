#pragma once
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
class ULevel : public UObject
{
    DECLARE_CLASS(ULevel, UObject)

public:

    /** ULevel에서 관리되는 모든 Actor의 목록 */
    TSet<AActor*> Actors;

};