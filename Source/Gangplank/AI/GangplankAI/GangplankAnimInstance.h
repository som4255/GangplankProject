// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "GangplankAnimInstance.generated.h"

UCLASS()
class GANGPLANK_API UGangplankAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UGangplankAnimInstance();

private :
	bool mIsMove;

public :

	UFUNCTION(BlueprintCallable)
	bool GetIsMove()
	{
		return mIsMove;
	}

public:
	virtual void NativeInitializeAnimation();

	virtual void NativeUpdateAnimation(float DeltaSeconds);

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	virtual void NativePostEvaluateAnimation();

	virtual void NativeUninitializeAnimation();

	virtual void NativeBeginPlay();

public :
	UFUNCTION()
	void AnimNotify_GangplankFire();

	UFUNCTION()
	void AnimNotify_SpawnBarrel();

	UFUNCTION()
	void AnimNotify_AttackEnd();
};
