// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GangplankJumpAttak.h"

UBTTask_GangplankJumpAttak::UBTTask_GangplankJumpAttak()
{
	NodeName = TEXT("GangplankJumpAttack");

	// TickTask 함수를 호출한다.
	bNotifyTick = true;

	// OnTaskFinished 함수를 호출한다.
	bNotifyTaskFinished = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage(TEXT("/Script/Engine.AnimMontage'/Game/Gangplank/Anim/AnimMonta/AM_GangplankSkillE.AM_GangplankSkillE'"));
	if (Montage.Succeeded())
	{
		mSkillEAnim = Montage.Object;
	}
}

EBTNodeResult::Type UBTTask_GangplankJumpAttak::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type();
}

EBTNodeResult::Type UBTTask_GangplankJumpAttak::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type();
}

void UBTTask_GangplankJumpAttak::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

}

void UBTTask_GangplankJumpAttak::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

}
