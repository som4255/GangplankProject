// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankBarrelAnimInstance.h"

UGangplankBarrelAnimInstance::UGangplankBarrelAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage(TEXT("/Script/Engine.AnimMontage'/Game/Gangplank/Barrel/Anim/GangplankBarrel_Spawn.GangplankBarrel_Spawn'"));
	if (Montage.Succeeded())
	{
		mSpawnMontage = Montage.Object;
	}
}

void UGangplankBarrelAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UGangplankBarrelAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UGangplankBarrelAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UGangplankBarrelAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UGangplankBarrelAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UGangplankBarrelAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Montage_Play(mSpawnMontage, 1.f);
}
