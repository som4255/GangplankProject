// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../GameInfo.h"
#include "../AIPawnBase.h"
#include "GangplankCooldownTimer.h"
#include "GangplankController.h"
#include "GangplankAnimInstance.h"
#include "GangplankBarrel.h"
#include "GangplankBullet.h"
#include "NavigationSystem.h"
#include "GangplankPawn.generated.h"

//기본제공이라 안해줘도된다.
//DECLARE_DELEGATE(FTimerDelegate);

USTRUCT(BlueprintType)
struct FCooldownTimer
{
	GENERATED_BODY()

public:
	FTimerHandle TimerHandle;
	float CoolTime;
	FTimerDelegate TimerDele;
};

enum EGangplankSkill : uint8
{
	SkillQ = 0,
	SkillW,
	SkillE, 
	SkillR, 
};

enum EExVoice : uint8
{
	Passive = 0,
	Death,
	Barrel
};


UCLASS()
class GANGPLANK_API AGangplankPawn : public AAIPawnBase
{
	GENERATED_BODY()

private :
	AGangplankPawn();

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGangplankBarrel> mBarrelClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGangplankBullet> mBulletClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGangplankAnimInstance> mAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNavigationSystemV1> mNavMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGangplankCooldownTimer> mTimerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FCooldownTimer> mCooldownTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator mSkillQTargetRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector mAdditionalBarrelLoc = FVector::ZeroVector;

	//스킬 사용 가능 유무를 2진수 스위치로 표기 EGangplankSkill enum값을 빼거나 더하는 것으로 키고 끌 수 있음
	int mSkillCanUse = 0b10000000101;

public :
	void SetCoolTime(EGangplankSkill Skill);
	void AddCooldownTimer(float Time, FName Name);
	void Attack_SkillQ();
	void Attack_SkillE();
	void Attack_SkillE_Additional();

	void SetSkillQTargetRot(const FRotator& TargetRot, const FVector& TargetLoc = FVector::ZeroVector)
	{
		mSkillQTargetRot = TargetRot;
		mAdditionalBarrelLoc = TargetLoc;
	}

	int GetSkillCanUse()
	{
		return mSkillCanUse;
	}

	const TObjectPtr<class UGangplankAnimInstance>& GetAnimInstance() const
	{
		return mAnim;
	}

	void OnSkillCanUse(EGangplankSkill Skill)
	{
		float i = Skill;
		mSkillCanUse += pow(2, i);
	}

protected:
	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
