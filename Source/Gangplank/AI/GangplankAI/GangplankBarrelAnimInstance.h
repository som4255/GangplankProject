// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "GangplankBarrelAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GANGPLANK_API UGangplankBarrelAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UGangplankBarrelAnimInstance();

private:
	//��Ÿ�ִ� VisibleAnywhere�̴ϱ� �۵��� �ȵȴ�. �����¸𸣰ڴ�
	UPROPERTY(Category = Component, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* mSpawnMontage;

public:
	virtual void NativeInitializeAnimation();

	virtual void NativeUpdateAnimation(float DeltaSeconds);

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	virtual void NativePostEvaluateAnimation();

	virtual void NativeUninitializeAnimation();

	virtual void NativeBeginPlay();
};
