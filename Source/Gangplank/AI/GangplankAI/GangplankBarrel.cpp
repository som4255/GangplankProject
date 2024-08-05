// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankBarrel.h"
#include "GangplankBarrelConnection.h"

const float BARRELHEIGHT = 107.f;

// Sets default values
AGangplankBarrel::AGangplankBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mAttackPreview = CreateDefaultSubobject<UDecalComponent>(TEXT("AttackPreview"));


	SetRootComponent(mCollision);
	mMesh->SetupAttachment(mCollision);
	mAttackPreview->SetupAttachment(mCollision);

	mCollision->SetCollisionProfileName(TEXT("EnemySpawnObj"));
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mCollision->SetWorldScale3D(FVector(1.6f, 1.6f, 1.6f));
	mCollision->SetCapsuleHalfHeight(70.f);
	mCollision->SetCapsuleRadius(55.f);
	mMesh->SetRelativeLocation(FVector(0.f, 0.f, -69.f));
	mAttackPreview->SetWorldScale3D(FVector(0.05f, 1.3f, 1.3f));
	mAttackPreview->SetRelativeLocation(FVector(0.f, 0.f, -70.f));
	mAttackPreview->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Gangplank/Barrel/Gangplank_barrel.Gangplank_barrel'"));
	if (Mesh.Succeeded())
	{
		mMesh->SetSkeletalMesh(Mesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Gangplank/Barrel/AB_GangplankBarrel.AB_GangplankBarrel_C'"));
	if (AnimAsset.Succeeded())
	{
		mMesh->SetAnimInstanceClass(AnimAsset.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance>
		DecalAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Gangplank/Barrel/Materials/MI_BarrelAttackPreview.MI_BarrelAttackPreview'"));
	if (DecalAsset.Succeeded())
	{
		mAttackPreview->SetDecalMaterial(DecalAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ExplosionParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A'"));
	if (ExplosionParticle.Succeeded())
	{
		mExplosionParticle = ExplosionParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
		SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Gangplank/Sound/SkillSound/Spell3_Explode-4.Spell3_Explode-4'"));
	if (SoundAsset.Succeeded())
	{
		mExplodeSound = SoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ExplosionTraceParticle(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/death/P_Death_BlastMark.P_Death_BlastMark'"));
	if (ExplosionTraceParticle.Succeeded())
	{
		mExplosionTraceParticle = ExplosionTraceParticle.Object;
	}
}

void AGangplankBarrel::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void AGangplankBarrel::Explode()
{
	if (IsValid(mExplosionParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), 
			mExplosionParticle,
			GetActorLocation(),
			FRotator::ZeroRotator,
			FVector(1.5f, 1.5f, 1.5f)
		);
	}

	if (IsValid(mExplodeSound))
	{
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			mExplodeSound,
			0.5f
		);
	}

	if (IsValid(mExplosionTraceParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			mExplosionTraceParticle,
			GetActorLocation() + FVector(180.f, 0.f, -BARRELHEIGHT),
			FRotator::ZeroRotator,
			FVector(1.8f, 1.8f, 1.f)
		);
	}

	TArray<TObjectPtr<AActor>> IgnoreActors;
	IgnoreActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		30.f,
		GetActorLocation(),
		450.f,
		nullptr,
		IgnoreActors,
		this,
		nullptr,
		false,
		ECollisionChannel::ECC_GameTraceChannel8
	);

	Destroy();
}

// Called when the game starts or when spawned
void AGangplankBarrel::BeginPlay()
{
	Super::BeginPlay();	

	TArray<FHitResult> result;
	FCollisionQueryParams	param(NAME_None, false, this);
	param.AddIgnoredActor(this);

	FActorSpawnParameters	ActorParam;
	ActorParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	bool Collision = GetWorld()->SweepMultiByChannel(
		result,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel9,
		FCollisionShape::MakeSphere(900.f),
		param
	);

	if (Collision)
	{
		for (auto& barrel : result)
		{
			TObjectPtr<AGangplankBarrelConnection> BarrelCon =
				GetWorld()->SpawnActor<AGangplankBarrelConnection>(
					AGangplankBarrelConnection::StaticClass(),
					GetActorLocation(),
					FRotator::ZeroRotator,
					ActorParam
				);

			BarrelCon->Init(this, barrel.GetActor());
		}
	}
}

float AGangplankBarrel::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(Cast<class AGangplankBullet>(DamageCauser))
		Explode();

	return 0.0f;
}

// Called every frame
void AGangplankBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


