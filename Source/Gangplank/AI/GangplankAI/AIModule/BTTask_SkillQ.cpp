// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SkillQ.h"


UBTTask_SkillQ::UBTTask_SkillQ()
{
	NodeName = TEXT("GangplankSkillQ");

	// TickTask 함수를 호출한다.
	bNotifyTick = true;

	// OnTaskFinished 함수를 호출한다.
	bNotifyTaskFinished = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage(TEXT("/Script/Engine.AnimMontage'/Game/Gangplank/Anim/AnimMonta/AM_GangplankSKillQ.AM_GangplankSKillQ'"));
	if (Montage.Succeeded())
	{
		mSkillQAnim = Montage.Object;
	}
}

EBTNodeResult::Type UBTTask_SkillQ::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//BehaviorTreeComponent를 이용하여 AIController를 얻어올 수 있다.
	mController = OwnerComp.GetAIOwner();
	
	mAIPawn = Cast<AGangplankPawn>(mController->GetPawn());
	if(!IsValid(mAIPawn))
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

	TArray<FHitResult> result;
	FCollisionQueryParams	param(NAME_None, false, mAIPawn);
	FRotator mFireRot = FRotator::ZeroRotator;

	bool Collision = GetWorld()->SweepMultiByChannel(
		result,
		mTarget->GetActorLocation(),
		mTarget->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel9,
		FCollisionShape::MakeSphere(400.f),
		param
	);

	if (Collision)
	{
		mFireRot = UKismetMathLibrary::FindLookAtRotation(
			mAIPawn->GetMesh()->GetSocketLocation("MuzzleSocket"),
			result[0].GetActor()->GetActorLocation()
		);
	}

	else
	{
		mFireRot = UKismetMathLibrary::FindLookAtRotation(
			mAIPawn->GetMesh()->GetSocketLocation("MuzzleSocket"),
			mTarget->GetActorLocation()
		);
	}

	mAIPawn->SetSkillQTargetRot(mFireRot);
	mAIPawn->SetActorRotation(mFireRot, ETeleportType::None);
	mAnimInst->Montage_Play(mSkillQAnim);
	mAIPawn->SetCanAttack(false);
	mAIPawn->SetCoolTime(EGangplankSkill::SkillQ);
	mAIPawn->MoveStop();
	
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_SkillQ::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type();
}

void UBTTask_SkillQ::TickTask(UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory, float DeltaSeconds)
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

void UBTTask_SkillQ::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::TickTask(OwnerComp, NodeMemory, TaskResult);
}
