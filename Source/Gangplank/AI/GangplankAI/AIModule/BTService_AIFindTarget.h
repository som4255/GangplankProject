// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AIFindTarget.generated.h"

/**
 * 
 */
UCLASS()
class GANGPLANK_API UBTService_AIFindTarget : public UBTService
{
	GENERATED_BODY()

public :
	UBTService_AIFindTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;
	
};
