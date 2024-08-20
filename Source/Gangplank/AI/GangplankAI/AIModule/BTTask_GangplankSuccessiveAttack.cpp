// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GangplankSuccessiveAttack.h"

UBTTask_GangplankSuccessiveAttack::UBTTask_GangplankSuccessiveAttack()
{
}

EBTNodeResult::Type UBTTask_GangplankSuccessiveAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type();
}

EBTNodeResult::Type UBTTask_GangplankSuccessiveAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type();
}

void UBTTask_GangplankSuccessiveAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

}

void UBTTask_GangplankSuccessiveAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

}
