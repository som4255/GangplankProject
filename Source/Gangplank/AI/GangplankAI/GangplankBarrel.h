// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GangplankBarrelAnimInstance.h"
#include "GameFramework/Actor.h"
#include "GangplankBarrel.generated.h"

UCLASS()
class GANGPLANK_API AGangplankBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGangplankBarrel();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent>	mCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent>	mMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDecalComponent>	mAttackPreview;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem>	mExplosionTraceParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem>	mExplosionParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase>	mExplodeSound;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float		mHP;

public :
	void Explode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void OnConstruction(const FTransform& Transform);
	virtual void Tick(float DeltaTime) override;

	

};
