// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "../GangplankPawn.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GangplankSetBlackBoard.generated.h"

/**
 * 
 */
UCLASS()
class GANGPLANK_API UBTService_GangplankSetBlackBoard : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_GangplankSetBlackBoard();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;
};
