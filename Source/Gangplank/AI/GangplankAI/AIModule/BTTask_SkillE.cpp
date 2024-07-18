// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SkillE.h"

UBTTask_SkillE::UBTTask_SkillE()
{
	NodeName = TEXT("GangplankSkillE");

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

EBTNodeResult::Type UBTTask_SkillE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//BehaviorTreeComponent를 이용하여 AIController를 얻어올 수 있다.
	mController = OwnerComp.GetAIOwner();

	mAIPawn = Cast<AGangplankPawn>(mController->GetPawn());
	if (!IsValid(mAIPawn))
		return EBTNodeResult::Failed;

	mAnimInst = mAIPawn->GetAnimInstance();
	if (!IsValid(mAnimInst))
		return EBTNodeResult::Failed;

	mTarget = Cast<AActor>(mController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(mTarget))
	{
		mController->StopMovement();

		return EBTNodeResult::Failed;
	}

	mAnimInst->Montage_Play(mSkillEAnim);
	mAIPawn->SetCanAttack(false);
	mAIPawn->SetCoolTime(EGangplankSkill::SkillE);
	mAIPawn->MoveStop();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_SkillE::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type();
}

void UBTTask_SkillE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsValid(mAIPawn) || !IsValid(mTarget) || !IsValid(mAnimInst))
	{	
		mController->StopMovement();

		// Task를 종료시킨다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}

	if (mAIPawn->GetCanAttack())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_SkillE::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
