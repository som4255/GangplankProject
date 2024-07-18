// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Components/ActorComponent.h"
#include "GangplankCooldownTimer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GANGPLANK_API UGangplankCooldownTimer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGangplankCooldownTimer();

public :
	void Init();

	//델리게이트에 바인딩할 함수는 UFUNCTION 매크로를 붙여줘야함
	UFUNCTION()
	void SkillQTimer();

	UFUNCTION()
	void SkillWTimer();

	UFUNCTION()
	void SkillETimer();

	UFUNCTION()
	void SkillRTimer();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
