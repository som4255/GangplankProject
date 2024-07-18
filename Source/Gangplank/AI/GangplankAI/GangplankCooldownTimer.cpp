// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankCooldownTimer.h"
#include "GangplankPawn.h"

// Sets default values for this component's properties
UGangplankCooldownTimer::UGangplankCooldownTimer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UGangplankCooldownTimer::Init()
{
	
}

void UGangplankCooldownTimer::SkillQTimer()
{
	TObjectPtr<AGangplankPawn> GangplankPawn = Cast<AGangplankPawn>(GetOwner());

	if (!IsValid(GangplankPawn))
		return;

	GangplankPawn->OnSkillCanUse(EGangplankSkill::SkillQ);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ReloadQ"));
}

void UGangplankCooldownTimer::SkillWTimer()
{
	TObjectPtr<AGangplankPawn> GangplankPawn = Cast<AGangplankPawn>(GetOwner());

	if (!IsValid(GangplankPawn))
		return;

	GangplankPawn->OnSkillCanUse(EGangplankSkill::SkillW);
}

void UGangplankCooldownTimer::SkillETimer()
{
	TObjectPtr<AGangplankPawn> GangplankPawn = Cast<AGangplankPawn>(GetOwner());

	if (!IsValid(GangplankPawn))
		return;

	GangplankPawn->OnSkillCanUse(EGangplankSkill::SkillE);
}

void UGangplankCooldownTimer::SkillRTimer()
{
}


// Called when the game starts
void UGangplankCooldownTimer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGangplankCooldownTimer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

