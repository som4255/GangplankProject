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

DECLARE_DELEGATE(FTimerDelegate);

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

	TObjectPtr<UGangplankAnimInstance> mAnim;

	TObjectPtr<UNavigationSystemV1> mNavMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGangplankCooldownTimer> mTimerComponent;

	TArray<FCooldownTimer> mCooldownTimer;

	TObjectPtr<APawn> mTargetActor;

	//스킬 사용 가능 유무를 2진수 스위치로 표기 EGangplankSkill enum값을 빼거나 더하는 것으로 키고 끌 수 있음
	int mSkillCanUse = 0b10000000101;

	bool mCanAttack = true;

protected :
	void LoadAIData(FName Name) override;

public :
	void SetCoolTime(EGangplankSkill Skill);
	void AddCooldownTimer(float Time, FName Name);
	void Attack_SkillE();
	void Attack_SkillQ();

	int GetSkillCanUse()
	{
		return mSkillCanUse;
	}

	void MoveStop()
	{
		mMovement->MaxSpeed = 0.f;
	}

	void MoveRecovery()
	{
		mMovement->MaxSpeed = mMoveSpeed;
	}

	const TObjectPtr<class UGangplankAnimInstance>& GetAnimInstance() const
	{
		return mAnim;
	}

	bool GetCanAttack()
	{
		return mCanAttack;
	}

	void SetCanAttack(bool CanDo)
	{
		mCanAttack = CanDo;
	}

	void OnSkillCanUse(EGangplankSkill Skill)
	{
		float i = Skill;
		mSkillCanUse += pow(2, i);
	}

	TObjectPtr<USkeletalMeshComponent> GetMesh()
	{
		return mMesh;
	}

	void SetTarget(TObjectPtr<APawn> Target)
	{
		mTargetActor = Target;
	}

protected:
	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
