// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankAnimInstance.h"
#include "GangplankPawn.h"

UGangplankAnimInstance::UGangplankAnimInstance()
{
	
}

void UGangplankAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UGangplankAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(TryGetPawnOwner()))
		return;

	if (TryGetPawnOwner()->GetVelocity().Normalize())
		mIsMove = true;

	else
		mIsMove = false;
	

}

void UGangplankAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UGangplankAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UGangplankAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UGangplankAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UGangplankAnimInstance::AnimNotify_GangplankFire()
{
	TObjectPtr<AGangplankPawn> AIPawn = Cast<AGangplankPawn>(TryGetPawnOwner());

	if (IsValid(AIPawn))
	{
		AIPawn->Attack_SkillQ();
	}
}

void UGangplankAnimInstance::AnimNotify_SpawnBarrel()
{
	TObjectPtr<AGangplankPawn> AIPawn = Cast<AGangplankPawn>(TryGetPawnOwner());

	if(IsValid(AIPawn))
	{
		AIPawn->Attack_SkillE();
	}
}

void UGangplankAnimInstance::AnimNotify_AttackEnd()
{
	TObjectPtr<AGangplankPawn> AIPawn = Cast<AGangplankPawn>(TryGetPawnOwner());

	AIPawn->SetCanAttack(true);

	if (IsValid(AIPawn))
	{
		AIPawn->MoveRecovery();
	}
}
