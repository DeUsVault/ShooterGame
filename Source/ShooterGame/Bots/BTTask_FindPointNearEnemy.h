// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPointNearEnemy.generated.h"

// Bot AI task that tries to find a location near the current enemy
UCLASS()
class SHOOTERGAME_API UBTTask_FindPointNearEnemy : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
