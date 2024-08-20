#pragma once

#include "../../GameInfo.h"
#include "GangplankBarrel.h"
#include "GameFramework/Actor.h"
#include "GangplankBarrelConnection.generated.h"

UCLASS()
class GANGPLANK_API AGangplankBarrelConnection : public AActor
{
	GENERATED_BODY()
	
public :
	AGangplankBarrelConnection();

private :
	static const float ExplosionTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent>	mNiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AGangplankBarrel>	mBarrelStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AGangplankBarrel>	mBarrelEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> mIgnitionParticle;

	FVector StartLoc;

	FVector EndLoc;

	float ElapsedTime = 0.f;

	bool HasbeenActivate = false;

	FTimerHandle TrigglerTimer;

	FTimerHandle IgnitionTimer;

	FTimerDelegate TimerDel;

private:
	void TrigglerStartBarrel();
	void TrigglerEndBarrel();

	UFUNCTION()
	void Activate(const FVector& Start, const FVector& End);

public :
	void Init(TObjectPtr<AActor> Start,TObjectPtr<AActor> End);
	virtual void Tick(float DeltaTime) override;
};
