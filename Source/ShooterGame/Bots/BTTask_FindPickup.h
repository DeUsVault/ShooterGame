// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPickup.generated.h"

// Bot AI Task that attempts to locate a pickup 
UCLASS()
class SHOOTERGAME_API UBTTask_FindPickup : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
