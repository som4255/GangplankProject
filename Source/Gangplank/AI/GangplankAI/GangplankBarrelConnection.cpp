// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankBarrelConnection.h"

const float AGangplankBarrelConnection::ExplosionTime = 0.4f;

AGangplankBarrelConnection::AGangplankBarrelConnection()
{
	PrimaryActorTick.bCanEverTick = true;

	mNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	mIgnitionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Niagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/Gangplank/Barrel/NS_BarrelConnection.NS_BarrelConnection'"));
	if (Niagara.Succeeded())
	{
		mNiagaraComponent->SetAsset(Niagara.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Forging/P_ForgeDip_Firey.P_ForgeDip_Firey'"));
	if (Particle.Succeeded())
	{
		mIgnitionParticle->SetTemplate(Particle.Object);
	}

	SetRootComponent(mNiagaraComponent);
	mIgnitionParticle->SetupAttachment(mNiagaraComponent);
	mIgnitionParticle->SetWorldScale3D(FVector(3.f, 3.f, 3.f));
}

void AGangplankBarrelConnection::TrigglerStartBarrel()
{
	if (IsValid(mBarrelStart))
	{
		mBarrelStart->Explode();
	}

	Destroy();
}

void AGangplankBarrelConnection::TrigglerEndBarrel()
{
	if (IsValid(mBarrelEnd))
	{
		mBarrelEnd->Explode();
	}

	Destroy();
}

void AGangplankBarrelConnection::Activate(const FVector& Start, const FVector& End)
{
	if (ElapsedTime < ExplosionTime)
	{
		ElapsedTime += 0.02f;
		float LerpRatio = ElapsedTime / ExplosionTime;
		FVector NewLocation = FMath::Lerp(Start, End, LerpRatio);
		mIgnitionParticle->SetWorldLocation(NewLocation);
	}
}

void AGangplankBarrelConnection::Init(TObjectPtr<AActor> Start, TObjectPtr<AActor> End)
{
	SetActorLocation(GetActorLocation() - FVector(0.f, 0.f, 110.f));
	mNiagaraComponent->SetVectorParameter(FName("EndLoc"), End->GetActorLocation() - FVector(0.f,0.f,110.f));

	mBarrelStart = Cast<AGangplankBarrel>(Start);
	mBarrelEnd = Cast<AGangplankBarrel>(End);

	StartLoc = mBarrelStart->GetActorLocation() - FVector(0.f,0.f,BARRELHEIGHT);
	EndLoc = mBarrelEnd->GetActorLocation() - FVector(0.f, 0.f, BARRELHEIGHT);

	mIgnitionParticle->SetActive(false);
}

void AGangplankBarrelConnection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(mBarrelStart) && HasbeenActivate == false)
	{
		HasbeenActivate = true;
		mNiagaraComponent->SetVisibility(false);
		mIgnitionParticle->SetActive(true);
		mIgnitionParticle->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(EndLoc, StartLoc));
		
		GetWorld()->GetTimerManager().SetTimer(
			TrigglerTimer,
			this,
			&AGangplankBarrelConnection::TrigglerEndBarrel,
			1.f,
			false,
			ExplosionTime
		);

		if (mBarrelEnd->GetCanIgnition()) 
		{
			//이미 터진 화약통에서 화약에 붙이 붙지 않도록 설정
			mBarrelEnd->Ignition();

			TimerDel.BindUFunction(this, FName("Activate"), StartLoc, EndLoc);
			mIgnitionParticle->ActivateSystem();

			GetWorld()->GetTimerManager().SetTimer(
				IgnitionTimer,
				TimerDel,
				0.02f,
				true
			);
		}
	}

	else if (!IsValid(mBarrelEnd) && HasbeenActivate == false)
	{
		HasbeenActivate = true;
		mNiagaraComponent->SetVisibility(false);
		mIgnitionParticle->SetActive(true);
		mIgnitionParticle->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc));

		GetWorld()->GetTimerManager().SetTimer(
			TrigglerTimer,
			this,
			&AGangplankBarrelConnection::TrigglerStartBarrel,
			1.f,
			false,
			ExplosionTime
		);

		if (mBarrelStart->GetCanIgnition())
		{
			mBarrelStart->Ignition();

			TimerDel.BindUFunction(this, FName("Activate"), EndLoc, StartLoc);
			mIgnitionParticle->ActivateSystem();

			GetWorld()->GetTimerManager().SetTimer(
				IgnitionTimer,
				TimerDel,
				0.02f,
				true
			);
		}
	}
}


